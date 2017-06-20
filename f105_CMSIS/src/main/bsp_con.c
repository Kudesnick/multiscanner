#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "misc.h"
#include "bsp_con_config.h"
#include "fifo.h"
#include "bsp_con.h"

// Константы длины обязательно должны быть степенью двойки
#define RX_BUFFER_SIZE 256
#define TX_BUFFER_SIZE 256

FIFO_T(char, RX_BUFFER_SIZE) bsp_con_rx_buffer;
FIFO_T(char, TX_BUFFER_SIZE) bsp_con_tx_buffer;

bsp_con_rx_handler_t * bsp_con_rx_handler = NULL;

static bsp_con_config_t bsp_usart_init_struct_default =
{
    .baudrate        = BSP_CON_BAUDRATE,
    .parity          = BSP_CON_PARITY,
    .stop_bits       = USART_StopBits_1,
};

bsp_con_config_t *bsp_con_get_setting(void)
{
    return &bsp_usart_init_struct_default;
}

// Инициализация модуля консоли
// con_rx_handler - внешний парсер принятой строки
// buf - буфер данных
// size - количество принятых данных
void bsp_con_init(bsp_con_rx_handler_t * con_rx_handler)
{
    USART_InitTypeDef usart_init_struct =
    {
        .USART_BaudRate            = bsp_usart_init_struct_default.baudrate,
        .USART_WordLength          = USART_WordLength_8b,
        .USART_StopBits            = bsp_usart_init_struct_default.stop_bits,
        .USART_Parity              = bsp_usart_init_struct_default.parity,
        .USART_Mode                = USART_Mode_Rx | USART_Mode_Tx,
        .USART_HardwareFlowControl = USART_HardwareFlowControl_None,
    };
    GPIO_InitTypeDef gpio_init_struct =
    {
        .GPIO_Speed = GPIO_Speed_50MHz,
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
    
    if (con_rx_handler != NULL)
    {
        bsp_con_rx_handler = con_rx_handler;
    }
}

// Переслать данные
bool bsp_con_send(const char *buf)
{
    bool result = ((FIFO_GET_FULL(bsp_con_tx_buffer) + strlen(buf)) <= FIFO_GET_SIZE(bsp_con_tx_buffer));
    
    if (result == true)
    {
        for (uint_fast16_t i = 0; i < strlen(buf); i++)
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
            )
        {
            FIFO_ADD(bsp_con_rx_buffer, 0x00);
            
            if (bsp_con_rx_handler != NULL)
            {
                bsp_con_rx_handler(&bsp_con_rx_buffer.data[bsp_con_rx_buffer.begin_idx], FIFO_GET_FULL(bsp_con_rx_buffer));
            }
            
            FIFO_CLEAR(bsp_con_rx_buffer);
        }
        
        else if (FIFO_GET_FULL(bsp_con_rx_buffer) < FIFO_GET_SIZE(bsp_con_rx_buffer))
        {
            if (data == ' ')
            {
                if (FIFO_READ_END(bsp_con_rx_buffer) != 0x00)
                {
                    FIFO_ADD(bsp_con_rx_buffer, 0x00);
                }
            }
            else
            {
                FIFO_ADD(bsp_con_rx_buffer, data);
            }
        }
    }
}
