#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "misc.h"

#include "fifo.h"
#include "iface.h"
#include "bsp_io.h"
#include "bsp_usart.h"
#include "fifo_con.h"
#include "units_config.h"
#include "bsp_con.h"

bsp_con_config_t bsp_con::default_sett =
{
    /* .baudrate  = */CON_BAUD,
    /* .parity    = */CON_PARITY,
    /* .stop_bits = */USART_StopBits_1,
    /* .echo      = */false,
    /* .color     = */true,
};

// Прерывание по приему/передаче
void bsp_con::callback(void * msg, uint32_t flags)
{
    if (flags & USART_FLAG_TXE)
    {
        if (!bufer->tx.is_empty())
        {
            static uint16_t s_msg;
            s_msg = bufer->tx.extract();
            send_msg((void *) &s_msg);
        }
    }
    
    if (flags & USART_FLAG_RXNE)
    {
        uint32_t data = (uint32_t)msg;
        
        bufer->rx.add((char)data);
    }
}

bsp_con::bsp_con(USART_TypeDef *_unit_ptr, fifo_con * buf, bsp_con_config_t * _setting):
    bsp_usart(_unit_ptr, IFACE_TYPE_CON, IFACE_NAME_CON),
    bufer(buf)
{
    set_setting(_setting);
}

// Переслать данные
bool bsp_con::send(const char *buf)
{
    __disable_irq();
#warning Гарантирует, что мы не затрем данные, но, возможно, что мы пропустим начало передачи.
        bool tx_complete = bufer->tx.is_empty();
    __enable_irq();
    
    bool result = bufer->tx.send_str(buf, setting.color);
    
    if (result == true
        && tx_complete == true
        )
    {
        static uint16_t msg;
        msg = bufer->tx.extract();
        send_msg((void *) &msg);
    }

    return result;
}

bsp_con_config_t * bsp_con::get_setting(void)
{
    static bsp_con_config_t _sett;
    
    _sett = setting;
    
    return &_sett;
}

void bsp_con::set_setting(bsp_con_config_t * sett)
{
    setting = *sett;
    
    bsp_usart_setting_t tmp_sett = 
    {
        // Хардварные настройки
        /*.USART_BaudRate                   = */ setting.baudrate,
        /*.USART_WordLength                 = */ USART_WordLength_8b,
        /*.USART_StopBits                   = */ setting.stop_bits,
        /*.USART_Parity                     = */ setting.parity,
        /*.USART_Mode                       = */ USART_Mode_Rx | USART_Mode_Tx,
        /*.USART_LIN_Break_Detection_Length = */ USART_LINBreakDetectLength_10b,
        // Софтварные настройки
        /*.USART_LIN_Enable = */ false,
        /*.USART_Enable     = */ true,
    };
    
    send_sett((void *)&tmp_sett);
}

uint32_t bsp_con::round_baud(uint32_t baud)
{
    return bsp_con::round_baud(baud);
}
