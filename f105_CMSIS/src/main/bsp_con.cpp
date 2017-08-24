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
};

// ���������� �� ������/��������
void bsp_con::bsp_usart_callback(uint16_t data, uint16_t flags)
{
    if (flags & USART_FLAG_TXE)
    {
        if (!bufer->tx.is_empty())
        {
            static uint16_t msg;
            msg = bufer->tx.extract();
            send_msg((void *) &msg);
        }
    }
    
    if (flags & USART_FLAG_RXNE)
    {
        bufer->rx.add(data);
    }
}

bsp_con::bsp_con(USART_TypeDef *_unit_ptr, fifo_con * buf, bsp_con_config_t * _setting):
#warning �����!!!
    bsp_usart(_unit_ptr, this->bsp_usart_callback),
    bufer(buf)
{
    set_setting(_setting);
}

// ��������� ������
bool bsp_con::send(const char *buf)
{
    bool result = bufer->tx.send_str(buf);
    
    if (result == true)
    {
#warning ��� ����� ��������� ��������, ����� ����� ���������� �� ���� � �������� ��� ����
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
        // ���������� ���������
        /*.USART_BaudRate                   = */ setting.baudrate,
        /*.USART_WordLength                 = */ USART_WordLength_8b,
        /*.USART_StopBits                   = */ setting.stop_bits,
        /*.USART_Parity                     = */ setting.parity,
        /*.USART_Mode                       = */ USART_Mode_Rx | USART_Mode_Tx,
        /*.USART_LIN_Break_Detection_Length = */ USART_LINBreakDetectLength_10b,
        // ���������� ���������
        /*.USART_LIN_Enable = */ false,
        /*.USART_Enable     = */ true,
    };
    
    send_sett((void *)&tmp_sett);
}
