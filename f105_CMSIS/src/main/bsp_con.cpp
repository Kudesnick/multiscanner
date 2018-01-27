#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "misc.h"

#include "fifo.h"
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
void bsp_con::callback(void)
{
    bsp_usart::callback();
    
    if (clbk_data.flags & USART_FLAG_TXE)
    {
        if (bufer->tx.is_not_empty())
        {
            message_t s_msg = bufer->tx.extract();
            send_msg(&s_msg);
        }
        else
        {
            tx_empty = true;
        }
    }
    
    if (clbk_data.flags & USART_FLAG_RXNE)
    {
        bufer->rx.add(clbk_data.data);
    }
}

bsp_con::bsp_con(unit_t *_unit_ptr, fifo_con * buf, bsp_con_config_t * _setting):
    bsp_usart(_unit_ptr, IFACE_TYPE_CON, IFACE_NAME_CON),
    bufer(buf)
{
    set_setting(_setting);
}

// Переслать данные
bool bsp_con::send(const char *buf)
{   
    bool result = bufer->tx.send_str(buf, setting.color);
    
    if (result == true)
    {
        __disable_irq();
        
        if (   !bufer->tx.is_empty()
            && tx_empty)
        {
            tx_empty = false;
            message_t s_msg = bufer->tx.extract();
            send_msg(&s_msg);
        }
        
        __enable_irq();
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
    
    settings_t tmp_sett = 
    {
        // Хардварные настройки
        /*.USART_BaudRate                   = */ sett->baudrate,
        /*.USART_WordLength                 = */ (sett->parity == USART_Parity_No) ? USART_WordLength_8b : USART_WordLength_9b,
        /*.USART_StopBits                   = */ sett->stop_bits,
        /*.USART_Parity                     = */ sett->parity,
        /*.USART_Mode                       = */ USART_Mode_Rx | USART_Mode_Tx,
        /*.USART_LIN_Break_Detection_Length = */ USART_LINBreakDetectLength_10b,
        // Софтварные настройки
        /*.USART_LIN_Enable = */ false,
        /*.USART_Enable     = */ true,
    };
    
    send_sett(&tmp_sett);
}

uint32_t bsp_con::round_baud(uint32_t baud)
{
    settings_t tmp_sett = * get_sett();
    tmp_sett.USART_BaudRate = baud;
    
    return bsp_usart::round_baud(&tmp_sett);
}
