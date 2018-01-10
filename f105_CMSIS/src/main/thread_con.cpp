//------------------------------------------------------------------------------
// Класс консоли
//------------------------------------------------------------------------------

#include <stdint.h>
#include <string.h>

#include "thread.h"
#include "bsp_con.h"
#include "fifo_con.h"
#include "thread_con.h"
#include "units_config.h"

#warning вот эти функции - в отдельный модуль >>>

#include "fifo.h"
#include "msg_types.h"
#include "parser.h"

class msg_buf : public cpp_fifo<char, FIFO_SIZE_256>
{
    private:
    protected:
    public:
        msg_buf(void): cpp_fifo(){};
        void add_str(const char * str)
        {
            if (str != NULL)
            {
                for(; *str != '\0'; str += sizeof(char))
                {
                    add(*str);
                }
            }
        };
};

void msg_uart_to_str(msg_t * msg, msg_buf * buf)
{
    // парсинг длины сообщения
    buf->add_str(parser_uint_to_str(msg->uart.len));
    buf->add_str(" ");
    // данные
    for (uint8_t i = 0; i < msg->uart.len; i++)
    {
        buf->add_str(parser_uint_to_hex(msg->uart.data[i], 2));
    }
    buf->add_str(" ");
    // причина завершения
    buf->add_str((msg->direct == MSG_TX) ? TAG_GREEN : TAG_BLUE);
    switch(msg->uart.reason)
    {
        case MSG_BRK_UART_TIMEOUT   : buf->add_str("TIM"); break; // Завершение по таймауту
        case MSG_BRK_UART_LAST_ID   : buf->add_str("LID"); break; // Детектирован байт завершения
        case MSG_BRK_UART_FIRST_ID  : buf->add_str("FID"); break; // Детектирован байт начала следующей посылки
        case MSG_BRK_UART_LENGTH    : buf->add_str("LEN"); break; // Достигнута фиксированная длина посылки SERIAL_MSG_LEN_MAX
        case MSG_BRK_UART_OVF       : buf->add_str(TAG_RED "OVF"); break; // Превышена длина буфера сообщения
        case MSG_BRK_UART_FRAME_ERR : buf->add_str(TAG_RED "FER"); break; // Ошибка фрейма
        case MSG_BRK_UART_PARITY_ERR: buf->add_str(TAG_RED "PER"); break; // Ошибка паритета
        default                     : buf->add_str(TAG_RED "NA "); break;
    }
    buf->add_str(" ");
};

static char * msg_to_str(msg_t * msg)
{
    char * result = NULL; 
    
    if (msg != NULL)
    {
#warning разрулить эту костыльную константу
        static msg_buf buf;
        
        buf.clear();
        
        // Парсинг направления
        switch (msg->direct)
        {
            case MSG_TX: buf.add_str(TAG_GREEN "TX")      ; break;
            case MSG_RX: buf.add_str(TAG_BLUE  "RX")      ; break; 
            default    : buf.add_str(TAG_RED "NA" TAG_DEF); break; 
        }
        buf.add_str(" ");
        // Парсинг интерфейса
        switch (msg->route)
        {
            case IFACE_NAME_CON: buf.add_str(" con"); break;
            #if CAN1_ENABLED != 0
                case IFACE_NAME_CAN1: buf.add_str("CAN1"); break;
            #endif
            #if CAN2_ENABLED != 0
                case IFACE_NAME_CAN2: buf.add_str("CAN2"); break;
            #endif
            #if (LIN1_ENABLED != 0)
                case IFACE_NAME_UART1: buf.add_str("URT1"); break;
                case IFACE_NAME_LIN1 : buf.add_str("LIN1"); break;
            #endif
            #if (LIN2_ENABLED != 0)
                case IFACE_NAME_UART2: buf.add_str("URT2"); break;
                case IFACE_NAME_LIN2 : buf.add_str("LIN2"); break;
            #endif
            default:  buf.add_str(TAG_RED " NA " TAG_DEF); break;
        }
        buf.add_str(" "  TAG_DEF);
        // Парсинг тела собщения
        switch (msg->msg_type)
        {
#warning реализовать парсинг тела сообщения для всех сообщений
            #if (CAN1_ENABLED != 0) || (CAN2_ENABLED != 0)
                case IFACE_TYPE_CAN : break;
            #endif
            #if (LIN1_ENABLED != 0) || (LIN2_ENABLED != 0) 
                case IFACE_TYPE_UART: msg_uart_to_str(msg, &buf); break;   
                case IFACE_TYPE_LIN : break;
            #endif
            default: buf.add_str(TAG_RED "ERROR MESSAGE TYPE "); break;
        }
        // Парсинг метки времени
        buf.add_str(TAG_DEF);
        buf.add_str(parser_uint_to_str(msg->rx_timestamp));
        // Отправка сообщения в консоль
        buf.add_str("\r\n");

#warning че-то дофига буферов.
        static char out_buf[FIFO_SIZE_256 + 1];
        memset(out_buf, '\0', sizeof(out_buf));
        
        for (uint16_t i = 0; !buf.is_empty(); i++)
        {
            out_buf[i] = buf.extract();
        }
        result = out_buf;
    }
    
    return result;
}

#warning вот эти функции - в отдельный модуль <<<

bool thread_con::send_msg(msg_t *msg)
{
    bool result = false;
#warning реализовать функцию.
    return result;
}

bool thread_con::send_msg_rdy(void)
{
#warning пока заглушка, но надо разрулить
    return true;
}
    
thread_con::thread_con(void (* _parse)(char * str)):
    thread_iface(IFACE_TYPE_CON, IFACE_NAME_CON),
    con_buf(),
    unit(CON_UNIT, &con_buf),
    parse(_parse)
{
};

void thread_con::set_parser(void (* _parse)(char * str))
{
    parse = _parse;
};

bool thread_con::send_str(const char * str)
{
    return unit.send(str);
};

void thread_con::routine(void)
{
    // thread_iface::routine();

    while (!buf.tx.is_empty())
    {
        msg_t msg = buf.tx.read_head();
        
        if (console_send_string(msg_to_str(&msg)))
        {
            buf.tx.cut_head();
        }
        else
        {
            break;
        }
    }

    if (con_buf.rx.get_str_count() > 0)
    { // В буфере полноценная команда

        static char str_buf[CON_RX_BUFFER_SIZE];

        for(uint16_t i = 0; i < sizeof(str_buf)/sizeof(str_buf[0]); i++)
        {
            str_buf[i] = con_buf.rx.extract();

            if (str_buf[i] == '\0')
            {
                if (parse != NULL)
                {
                    parse(str_buf);
                }
                break;
            }
        }
    }
};

bool console_send_string(const char * str)
{
    bool result = false;

    thread_con * ptr = (thread_con *)thread::get_last_pointer();

    while (ptr != NULL)
    {
        if (ptr->get_class_type() == IFACE_TYPE_CON)
        {
            result = ptr->send_str(str);
        }
        ptr = (thread_con *)ptr->get_prev_pointer();
    }

    return result;
}
