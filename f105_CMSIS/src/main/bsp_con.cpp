#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "misc.h"

#include "fifo.h"
#include "bsp_io.h"
#include "bsp_usart.h"
#include "bsp_con.h"
#include "fifo_con.h"
#include "units_config.h"

static bsp_con_config_t bsp_con_init_struct_default =
{
    /* .baudrate  = */CON_BAUD,
    /* .parity    = */CON_PARITY,
    /* .stop_bits = */USART_StopBits_1,
    /* .echo      = */false,
};

static bsp_unit *bsp_con;
static fifo_con *buf_con;

bsp_con_config_t *bsp_con_get_setting(void)
{
    return &bsp_con_init_struct_default;
}

// Переслать данные
bool bsp_con_send(const char *buf)
{
    bool result = buf_con->tx.send_str(buf);
    
    if (result == true)
    {
#warning вот здесь продумать ситуацию, когда буфер изначально не пуст и передача уже идет
        static uint16_t msg;
        msg = buf_con->tx.extract();
        bsp_con->send_msg((void *) &msg);
    }
    
    return result;
}

// Прерывание по приему/передаче
void bsp_con_handler(uint16_t data, uint16_t flags)
{
    if (flags & USART_FLAG_TXE)
    {
        if (!buf_con->tx.is_empty())
        {
            static uint16_t msg;
            msg = buf_con->tx.extract();
            bsp_con->send_msg((void *) &msg);
        }
    }
    
    if (flags & USART_FLAG_RXNE)
    {
        buf_con->rx.add(data);
    }
}

// Инициализация модуля консоли
// con_rx_handler - внешний парсер принятой строки
void bsp_con_init(fifo_con * buf)
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

#warning реализовать статически
    bsp_con = new bsp_usart(CON_UNIT, bsp_con_handler);
    buf_con = buf;
    bsp_con->send_sett((void *)&sett);
}
