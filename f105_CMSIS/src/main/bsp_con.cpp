#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "misc.h"

#include "fifo.h"
#include "bsp_io.h"
#include "bsp_usart.h"
#include "bsp_con.h"
#include "units_config.h"

#define RX_BUFFER_SIZE FIFO_SIZE_256
#define TX_BUFFER_SIZE FIFO_SIZE_256

static cpp_fifo<char, RX_BUFFER_SIZE> bsp_con_rx_buffer;
static cpp_fifo<char, TX_BUFFER_SIZE> bsp_con_tx_buffer;

bsp_con_rx_handler_t * bsp_con_rx_handler = NULL;

static bsp_con_config_t bsp_con_init_struct_default =
{
    /* .baudrate  = */CON_BAUD,
    /* .parity    = */CON_PARITY,
    /* .stop_bits = */USART_StopBits_1,
    /* .echo      = */false,
};

static bsp_unit *bsp_con;

bsp_con_config_t *bsp_con_get_setting(void)
{
    return &bsp_con_init_struct_default;
}

// Переслать данные
bool bsp_con_send(const char *buf)
{
    bool result = ((bsp_con_tx_buffer.get_full_count() + strlen(buf)) <= bsp_con_tx_buffer.get_count());
    
    if (result == true)
    {
        for (uint_fast16_t i = 0; i < strlen(buf); i++)
        {
            bsp_con_tx_buffer.add(buf[i]);
        }

#warning вот здесь продумать ситуацию, когда буфер изначально не пуст и передача уже идет
        static uint16_t msg;
        msg = bsp_con_tx_buffer.extract();
        bsp_con->send_msg((void *) &msg);
    }
    
    return result;
}

// Прерывание по приему/передаче
void bsp_con_handler(uint16_t data, uint16_t flags)
{
    if (flags & USART_FLAG_TXE)
    {
        if (!bsp_con_tx_buffer.is_empty())
        {
            static uint16_t msg;
            msg = bsp_con_tx_buffer.extract();
            bsp_con->send_msg((void *) &msg);
        }
    }
    
    if (flags & USART_FLAG_RXNE)
    {
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
            if ((char)data == ' ')
            {
                if (bsp_con_rx_buffer.read_end() != 0x00)
                {
                    bsp_con_rx_buffer.add(0x00);
                }
            }
            else
            {
                bsp_con_rx_buffer.add((char)data);
            }
        }
    }
}

// Инициализация модуля консоли
// con_rx_handler - внешний парсер принятой строки
void bsp_con_init(bsp_con_rx_handler_t * con_rx_handler)
{
    bsp_usart_setting_t sett = 
    {
        // Хардварные настройки
        /*.USART_BaudRate                   = */ bsp_con_init_struct_default.baudrate,
        /*.USART_WordLength                 = */ USART_WordLength_8b,
        /*.USART_StopBits                   = */ bsp_con_init_struct_default.stop_bits,
        /*.USART_Parity                     = */ bsp_con_init_struct_default.parity,
        /*.USART_Mode                       = */ USART_Mode_Rx | USART_Mode_Tx,
        /*.USART_LIN_Break_Detection_Length = */ USART_LINBreakDetectLength_10b,
        // Софтварные настройки
        /*.USART_LIN_Enable = */ false,
        /*.USART_Enable     = */ true,
    };

    bsp_con = new bsp_usart(CON_UNIT, bsp_con_handler);
    bsp_con->send_sett((void *)&sett);
    
    bsp_con_rx_handler = con_rx_handler;
}
