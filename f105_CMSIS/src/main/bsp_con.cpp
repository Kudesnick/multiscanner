#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "misc.h"
#include "bsp_con_config.h"
#include "fifo.h"
#include "bsp_con.h"
#include "bsp_io.h"

static bsp_con_config_t bsp_con_init_struct_default =
{
    /* .baudrate  = */BSP_CON_BAUDRATE,
    /* .parity    = */BSP_CON_PARITY,
    /* .stop_bits = */USART_StopBits_1,
    /* .echo      = */false,
};

bsp_con_config_t *bsp_con_get_setting(void)
{
    return &bsp_con_init_struct_default;
}

// »нициализаци€ модул€ консоли
// con_rx_handler - внешний парсер прин€той строки
void bsp_con_init(bsp_con_rx_handler_t * con_rx_handler)
{
    USART_InitTypeDef usart_init_struct =
    {
        /*.USART_BaudRate            = */bsp_con_init_struct_default.baudrate,
        /*.USART_WordLength          = */USART_WordLength_8b,
        /*.USART_StopBits            = */bsp_con_init_struct_default.stop_bits,
        /*.USART_Parity              = */bsp_con_init_struct_default.parity,
        /*.USART_Mode                = */USART_Mode_Rx | USART_Mode_Tx,
        /*.USART_HardwareFlowControl = */USART_HardwareFlowControl_None,
    };

    RCC_APB1PeriphClockCmd(BSP_CON_UNIT_CLK, ENABLE);
    
    cpp_port tx(BSP_CON_TX_PORT, BSP_CON_TX_PIN, GPIO_Mode_AF_PP);
    cpp_port rx(BSP_CON_RX_PORT, BSP_CON_RX_PIN, GPIO_Mode_IPU, true);

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

// ѕереслать данные
bool bsp_con_send(const char *buf)
{
    bool result = ((bsp_con_tx_buffer.get_full_count() + strlen(buf)) <= bsp_con_tx_buffer.get_count());
    
    if (result == true)
    {
        for (uint_fast16_t i = 0; i < strlen(buf); i++)
        {
            bsp_con_tx_buffer.add(buf[i]);
        }
        
        USART_ITConfig(BSP_CON_UNIT, USART_IT_TXE, ENABLE);
    }
    
    return result;
}

// ѕрерывание по приему/передаче
extern "C" void bsp_con_handler(void)
{
    if (USART_GetITStatus(BSP_CON_UNIT, USART_IT_TXE) == SET)
    {
        if (!bsp_con_tx_buffer.is_empty())
        {
            USART_SendData(BSP_CON_UNIT, bsp_con_tx_buffer.extract());
        }
        
        if (bsp_con_tx_buffer.is_empty())
        {
            USART_ITConfig(BSP_CON_UNIT, USART_IT_TXE, DISABLE);
        }
    }
    
    if (USART_GetITStatus(BSP_CON_UNIT, USART_IT_RXNE) == SET)
    {
        uint_fast8_t data = USART_ReceiveData(BSP_CON_UNIT);
        
        if ((data == 0x0A || data == 0x0D)
            && (!bsp_con_rx_buffer.is_empty())
            )
        {
            bsp_con_rx_buffer.add(0x00);
            
            if (bsp_con_rx_handler != NULL)
            {
                bsp_con_rx_handler(bsp_con_rx_buffer.get_head(), bsp_con_rx_buffer.get_full_count());
            }
            
            bsp_con_rx_buffer.clear();
        }
        
        else if (!bsp_con_rx_buffer.is_full())
        {
            if (data == ' ')
            {
                if (bsp_con_rx_buffer.read_end() != 0x00)
                {
                    bsp_con_rx_buffer.add(0x00);
                }
            }
            else
            {
                bsp_con_rx_buffer.add(data);
            }
        }
    }
}
