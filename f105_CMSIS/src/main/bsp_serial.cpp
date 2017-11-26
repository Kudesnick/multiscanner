#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "misc.h"

#include "fifo.h"
#include "msg_types.h"
#include "bsp_io.h"
#include "bsp_usart.h"
#include "fifo_con.h"
#include "units_config.h"
#include "bsp_serial.h"

bsp_serial_config_t bsp_serial::default_sett =
{
    /* .baudrate      = */ CON_BAUD,
    /* .parity        = */ CON_PARITY,
    /* .stop_bits     = */ USART_StopBits_1,
    /* .timeout       = */ 500,
    /* .byte_of_begin = */ -1,
    /* .byte_of_end   = */ -1,
    /* .max_len       = */ 0,
    /* .echo          = */ false,
    /* .enable        = */ true,
};

// Прерывание по приему/передаче
void bsp_serial::callback(void * msg, uint32_t flags)
{
    // Передача байта закончена
    if (flags & USART_FLAG_TXE)
    {
#warning реализовать загрузку следующего сообщения, но только через таймаут, чтобы сообщения не сливались
        if (!internal_tx.is_empty())
        {
            static uint16_t s_msg;
            s_msg = internal_tx.extract();
            send_msg((void *) &s_msg);
        }
    }

    // Принят байт
    if (flags & USART_FLAG_RXNE)
    {
        uint8_t data = (uint32_t)(uint32_t *)msg;

        static msg_t msg_buf;
        
        // Байт продетектирован как начало пакета
        if (data == setting.byte_of_begin)
        {
            msg_buf.uart.len = internal_rx.get_full_count();
            
            if(msg_buf.uart.len > 0)
            {
                for(uint8_t i = 0; i < msg_buf.uart.len; msg_buf.uart.data[i++] = internal_rx.extract());
                
                msg_buf.uart.reason = MSG_BRK_UART_FIRST_ID;
            
                if (!bufer->rx.is_full())
                {
                    bufer->rx.add(msg_buf);
                }
                
                internal_rx.clear();
            }
        }
        
        // Принятый байт - первый
        if (internal_rx.is_empty())
        {
            msg_buf.msg_type = IFACE_TYPE_UART;
            msg_buf.route = (iface_name_t)get_object_name();
            msg_buf.direct = MSG_RX;
#warning вот здесь воткнуть метку времени (из системного таймера, например)
            msg_buf.rx_timestamp = 12345;
        }
        
        internal_rx.add(data);
        
        // Байт продетектирован как конец пакета или длина пакета 
        if (   flags & (USART_FLAG_FE | USART_FLAG_PE)
            || data == setting.byte_of_end
            || internal_rx.get_full_count() >= internal_rx.get_count()
            || (   setting.max_len > 0
                && internal_rx.get_full_count() + 1 >= setting.max_len
                )
            )
        {
#warning лютаякопипаста (см. строку "if (data == setting.byte_of_begin)"), но пока ничего лучше не придумал
            msg_buf.uart.len = internal_rx.get_full_count();
            
            if(msg_buf.uart.len > 0)
            {
                for(uint8_t i = 0; i < msg_buf.uart.len; msg_buf.uart.data[i++] = internal_rx.extract());
                
                
                msg_buf.uart.reason =
                    (flags & USART_FLAG_FE)                                     ? MSG_BRK_UART_FRAME_ERR :
                    (flags & USART_FLAG_PE)                                     ? MSG_BRK_UART_PARITY_ERR :
                    (data == setting.byte_of_end)                               ? MSG_BRK_UART_LAST_ID :
                    (internal_rx.get_full_count() >= internal_rx.get_count())   ? MSG_BRK_UART_OVF :
                                                                                  MSG_BRK_UART_LENGTH;
                if (!bufer->rx.is_full())
                {
                    bufer->rx.add(msg_buf);
                }
                
                internal_rx.clear();
            }
            
        }
    }
}

bsp_serial::bsp_serial(USART_TypeDef *_unit_ptr, fifo_buff * buf, bsp_serial_config_t * _setting, iface_name_t _name):
    bsp_usart(_unit_ptr, IFACE_TYPE_UART, _name),
    bufer(buf),
    internal_tx(),
    internal_rx()
{
    set_setting(_setting);
}

// Переслать данные
bool bsp_serial::send(const uint8_t data)
{
    __disable_irq();
#warning Гарантирует, что мы не затрем данные, но, возможно, что мы пропустим начало передачи.
        bool tx_complete = internal_tx.is_empty();
    __enable_irq();
    
    bool result = !internal_tx.is_full();
    
    if (result)
    {
        internal_tx.add(data);
    }
    
    if (   result == true
        && tx_complete == true
        )
    {
        static uint16_t msg;
        msg = internal_tx.extract();
        send_msg((void *) &msg);
    }

    return result;
}

bsp_serial_config_t * bsp_serial::get_setting(void)
{
    static bsp_serial_config_t _sett;
    
    _sett = setting;
    
    return &_sett;
}

void bsp_serial::set_setting(bsp_serial_config_t * sett)
{
    setting = *sett;
    
    bsp_usart_setting_t tmp_sett = 
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
    
    send_sett((void *)&tmp_sett);
}

uint32_t bsp_serial::round_baud(uint32_t baud)
{
    return bsp_usart::round_baud(baud);
}
