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
    /* .byte_of_begin = */ -1,
    /* .byte_of_end   = */ -1,
    /* .max_len       = */ 0,
    /* .echo          = */ false,
    /* .enable        = */ true,
};

// Прерывание по приему/передаче
void bsp_serial::callback(void)
{
    bsp_usart::callback();
    
    // Передача байта закончена
    if (   clbk_data.flags & USART_FLAG_TXE
        && !internal_tx.is_empty()
        )
    {
        message_t msg = internal_tx.extract();
        send_msg(&msg);
    }
    // Передача сообщения закончена
    if (   clbk_data.flags & USART_FLAG_IDLE
        && !internal_tx_buf.is_empty()
        && internal_tx.is_empty()
        )
    {
        msg_t tmp_msg = internal_tx_buf.extract();
        
        for (uint8_t i = 0; i < tmp_msg.uart.len; i++)
        {
            internal_tx.add(tmp_msg.uart.data[i]);
        }
        if (!internal_tx.is_empty())
        {
            message_t msg = internal_tx.extract();
            send_msg(&msg);
        }
    }

    // Принят байт
    if (clbk_data.flags & USART_FLAG_RXNE)
    {
        uint8_t data = clbk_data.data;

        static msg_t msg_buf;
        
        // Байт продетектирован как начало пакета
        if (data == setting.byte_of_begin)
        {
            msg_buf.uart.len = internal_rx.get_full_count();
            
            if(msg_buf.uart.len > 0)
            {
                for(uint8_t i = 0; i < msg_buf.uart.len; msg_buf.uart.data[i++] = internal_rx.extract());
                
                msg_buf.uart.reason = MSG_BRK_UART_FIRST_ID;
            
                if (!buffer->rx.is_full())
                {
                    buffer->rx.add(msg_buf);
                }
                
                internal_rx.clear();
            }
        }
        
        // Принятый байт - первый
        if (internal_rx.is_empty())
        {
            msg_buf.msg_type = IFACE_TYPE_UART;
            msg_buf.route = (iface_name_t)cpp_list<LIST_TYPE_UNIT_USART>::get_object_name();
            msg_buf.direct = MSG_RX;
            msg_buf.rx_timestamp = timer->get_timestamp();
        }
        
        if ((clbk_data.flags & USART_FLAG_IDLE) == 0)
        {
            internal_rx.add(data);
        }
        
        msg_brk_reason_uart_t tmp_reason = 
            (clbk_data.flags & USART_FLAG_FE)                           ? MSG_BRK_UART_FRAME_ERR :
            (clbk_data.flags & USART_FLAG_PE)                           ? MSG_BRK_UART_PARITY_ERR :
            (clbk_data.flags & USART_FLAG_IDLE)                         ? MSG_BRK_UART_TIMEOUT :
            (data == setting.byte_of_end)                               ? MSG_BRK_UART_LAST_ID :
            (data == setting.byte_of_begin)                             ? MSG_BRK_UART_FIRST_ID :
            (internal_rx.get_full_count() >= internal_rx.get_count())   ? MSG_BRK_UART_OVF :
            (   setting.max_len > 0
             && internal_rx.get_full_count() >= setting.max_len)        ? MSG_BRK_UART_LENGTH :
                                                                          MSG_BRK_UART_NA;
        
        // Байт продетектирован как конец пакета или длина пакета превышена
        if (tmp_reason != MSG_BRK_UART_NA)
        {
            // лютая копипаста (см. строку "if (data == setting.byte_of_begin)"), но пока ничего лучше не придумал
            msg_buf.uart.len = internal_rx.get_full_count();
            
            if(msg_buf.uart.len > 0)
            {
                for(uint8_t i = 0; i < msg_buf.uart.len; msg_buf.uart.data[i++] = internal_rx.extract());

                msg_buf.uart.reason = tmp_reason;
                
                if (!buffer->rx.is_full())
                {
                    buffer->rx.add(msg_buf);
                }
                else
                {
#warning реализовать обработку переполнения буфера
                }

                internal_rx.clear();
            }
            
        }
    }
}

bsp_serial::bsp_serial(unit_t *_unit_ptr, fifo_buff * _buffer, iface_name_t _name, bsp_serial_config_t * _setting):
    bsp_usart(_unit_ptr, IFACE_TYPE_UART, _name),
    buffer(_buffer)
{
    set_setting(_setting);
}

// Переслать данные
bool bsp_serial::send(msg_t * msg)
{
    bool result = (!internal_tx_buf.is_full() || internal_tx.is_empty()) && (msg->msg_type == IFACE_TYPE_UART);
    
    if (result)
    {
        __disable_irq();

        if (!internal_tx.is_empty())
        {
            internal_tx_buf.add(*msg);
        }
        else
        {
            msg_t tmp_msg;
            
            if (!internal_tx_buf.is_empty())
            {
                tmp_msg = internal_tx_buf.extract();
                internal_tx_buf.add(*msg);
            }
            else
            {
                tmp_msg = *msg;
            }
            
            for (uint8_t i = 0; i < tmp_msg.uart.len; i++)
            {
                internal_tx.add(tmp_msg.uart.data[i]);
            }
            
            if (tx_empty)
            {
                message_t s_msg = internal_tx.extract();
                send_msg(&s_msg);
            }
        }
        
        __enable_irq();
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
    
    if (setting.max_len > UART_DATA_LEN_MAX) setting.max_len = UART_DATA_LEN_MAX;
    if ((setting.byte_of_begin & 0xFF) != setting.byte_of_begin) setting.byte_of_begin = -1;
    if ((setting.byte_of_end & 0xFF) != setting.byte_of_end) setting.byte_of_end = -1;
    
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

uint32_t bsp_serial::round_baud(uint32_t baud)
{
    settings_t tmp_sett = * get_sett();
    tmp_sett.USART_BaudRate = baud;
    
    return bsp_usart::round_baud(&tmp_sett);
}
