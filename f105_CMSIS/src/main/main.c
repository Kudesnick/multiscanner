/********************************************************************
 * Основная программа
 ********************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/********************************************************************
 * bsp_console
 ********************************************************************/

#warning Вынести эти функции в отдельный модуль.
 
#include "misc.h"
#include "bsp_con_config.h"
#include "fifo.h"

// Константы длины обязательно должны быть степенью двойки
#define RX_BUFFER_SIZE 256
#define TX_BUFFER_SIZE 256

FIFO_T(uint8_t,RX_BUFFER_SIZE) bsp_con_rx_buffer;
FIFO_T(uint8_t,TX_BUFFER_SIZE) bsp_con_tx_buffer;

void (*bsp_con_rx_handler)(uint8_t *buf, uint16_t size) = NULL;

// Инициализация модуля консоли
// con_rx_handler - внешний парсер принятой строки
// buf - буфер данных
// size - количество принятых данных
void bsp_con_init(void(*con_rx_handler)(uint8_t *buf, uint8_t size))
{
    USART_InitTypeDef usart_init_struct =
    {
        .USART_BaudRate            = BSP_CON_BAUDRATE,
        .USART_WordLength          = USART_WordLength_8b,
        .USART_StopBits            = USART_StopBits_1,
        .USART_Parity              = BSP_CON_PARITY,
        .USART_Mode                = USART_Mode_Rx | USART_Mode_Tx,
        .USART_HardwareFlowControl = USART_HardwareFlowControl_None,
    };
    GPIO_InitTypeDef gpio_init_struct =
    {
        .GPIO_Speed = GPIO_Speed_2MHz,
    };
    
    RCC_ClocksTypeDef rcc_clocks;

    RCC_GetClocksFreq(&rcc_clocks);

    RCC_APB1PeriphClockCmd(BSP_CON_UNIT_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2ENR_AFIOEN, ENABLE);
    RCC_APB2PeriphClockCmd(BSP_CON_TX_PORT_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(BSP_CON_RX_PORT_RCC, ENABLE);

    gpio_init_struct.GPIO_Mode  = GPIO_Mode_AF_PP;
    gpio_init_struct.GPIO_Pin = BSP_CON_TX_PIN;
    GPIO_Init(BSP_CON_TX_PORT, &gpio_init_struct);
    
    GPIO_WriteBit(BSP_CON_RX_PORT, BSP_CON_RX_PIN, Bit_SET);
    gpio_init_struct.GPIO_Mode  = GPIO_Mode_IPU;
    gpio_init_struct.GPIO_Pin = BSP_CON_RX_PIN;
    GPIO_Init(BSP_CON_RX_PORT, &gpio_init_struct);
    
    USART_Init(BSP_CON_UNIT, &usart_init_struct);
    
    NVIC_SetPriority(BSP_CON_IRQn, BSP_CON_IRQ_PRIORITY);
    NVIC_EnableIRQ(BSP_CON_IRQn);
    USART_ITConfig(BSP_CON_UNIT, USART_IT_TXE, DISABLE);
    USART_ITConfig(BSP_CON_UNIT, USART_IT_RXNE, ENABLE);
    
    USART_Cmd(BSP_CON_UNIT, ENABLE);
}

// Переслать данные
bool bsp_con_send(uint8_t *buf, uint8_t size)
{
    bool result = ((FIFO_GET_FULL(bsp_con_tx_buffer) + size) <= FIFO_GET_SIZE(bsp_con_tx_buffer));
    
    if (result == true)
    {
        for (uint_fast16_t i = 0; i < size; i++)
        {
            FIFO_ADD(bsp_con_tx_buffer, buf[i]);
        }
        
        USART_ITConfig(BSP_CON_UNIT, USART_IT_TXE, ENABLE);
    }
    
    return result;
}

// Прерывание по приему/передаче
void bsp_con_handler(void)
{
    if (USART_GetITStatus(BSP_CON_UNIT, USART_IT_TXE) == SET)
    {
        if (FIFO_GET_FULL(bsp_con_tx_buffer) != 0)
        {
            USART_SendData(BSP_CON_UNIT, FIFO_EXTRACT(bsp_con_tx_buffer));
        }
        
        if (FIFO_GET_FULL(bsp_con_tx_buffer) == 0)
        {
            USART_ITConfig(BSP_CON_UNIT, USART_IT_TXE, DISABLE);
        }
    }
    
    if (USART_GetITStatus(BSP_CON_UNIT, USART_IT_RXNE) == SET)
    {
        uint_fast8_t data = USART_ReceiveData(BSP_CON_UNIT);
        
        if ((data == 0x0A || data == 0x0D)
            && (FIFO_GET_FULL(bsp_con_rx_buffer) != 0)
            && (bsp_con_rx_handler != NULL)
            )
        {
            bsp_con_rx_handler(&bsp_con_rx_buffer.data[bsp_con_rx_buffer.begin_idx], FIFO_GET_FULL(bsp_con_rx_buffer));
        }
        
        else if (FIFO_GET_FULL(bsp_con_rx_buffer) < FIFO_GET_SIZE(bsp_con_rx_buffer))
        {
            FIFO_ADD(bsp_con_rx_buffer, data);
        }
    }
}

int main (void)
{
    static volatile uint8_t tmp = 0;

    while(true)
    {
        tmp = ~tmp;
    }
}
