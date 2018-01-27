/**
 *  @file    parser_msg.cpp
 *
 *  @brief   
 *  @details 
 *
 *  @author  
 *  @date    
 */

#include "parser.h"
#include "parser_msg.h"
#include "thread_con.h"

template <typename result_t>
static bool parser_set_uint(char ** str, const void * const param, void * const result)
{
    bool success = false;
    
    char * tmp_str = *str;
    result_t res = parser_str_to_uint(&tmp_str);
    
    if (tmp_str != NULL)
    {
        *str = tmp_str;
        *(result_t *)result = res;
        success = true;
    }

    return success;
}

static bool hex_str_to_byte_array(char ** str, uint8_t * arr, uint8_t * size)
{
    bool result = true;
    
    if (*str[0] != '\0')
    {
        uint8_t data;
        uint8_t data_len;
        
        for (uint8_t i = 0; strchr(" \0", *str[0]) == NULL; i++)
        {
            data <<= 4;
            if (*str[0] >= '0' && *str[0] <= '9')
            {
                data += *str[0] - '0';
            }
            else if (*str[0] >= 'a' && *str[0] <= 'f')
            {
                data += *str[0] - 'a' + 0x0A;
            }
            else if (*str[0] >= 'A' && *str[0] <= 'F')
            {
                data += *str[0] - 'A' + 0x0A;
            }
            else
            { // недопустимый символ
                result = false;
                break;
            }
            
            *str += sizeof(char);
            
            // Попытка записи очередного байта
            if ((i & 1) == 1)
            {
                if ((i >> 1) < *size)
                {
                    arr[i >> 1] = data;
                    data_len = (i >> 1) + 1;
                }
                else
                { // Превышена максимально допустимая длина сообщения
                    result = false;
                    break;
                }
            }
            else if (strchr(" \0", *str[0]) != NULL)
            { // нечетное количество полубайт в сообщении
                result = false;
                break;
            }
            
        }
        
        if (*str != NULL
            && *str[0] == ' '
            )
        {
            *str += sizeof(char);
        }
        
        if (result)
        {
            *size = data_len;
        }
    }
    else
    {
        result = false;
    }
    
    return result;
}

// Отправить сообщение UART
bool parser_send_uart(char ** str, const void * const param, void * const result)
{
    uint16_t time = 0;
    uint16_t count = 1;
    msg_t msg;

    static const parse_fsm_steps_t cmd_list[] =
    {
        {"-t", parser_set_uint<uint16_t>, NULL, (void *)&time },
        {"-c", parser_set_uint<uint16_t>, NULL, (void *)&count},
    };
    
    msg.uart.len = sizeof(msg.uart.data);
    
    char * tmp_str = *str;
    if (hex_str_to_byte_array(&tmp_str, msg.uart.data, &msg.uart.len) == true)
    {
        *str = tmp_str;
        
        if (   strlen(*str) != 0
            && parser_iteration(str, cmd_list, countof_arr(cmd_list)) != true)
        {
            console_send_string(parser_str_err_syntax_cmd); 
        }
        else if (   count != 1
                 && time == 0)
        {
            console_send_string(TAG_RED "Error!" TAG_DEF " The time must not be zero if the message is to be repeated.\r\n");
        }
        else
        {
            msg.msg_type = IFACE_TYPE_UART;
            msg.route = (iface_name_t)(uint32_t)param;
            msg.direct = MSG_TX;
            msg.counter = 0;
            msg.count = count;
            msg.interval = time;
            
            static fifo_buff * buf_ptr = NULL;
            
            if (buf_ptr == NULL)
            {
                buf_ptr = (fifo_buff *)fifo_buff::cpp_list<LIST_TYPE_BUFFER>::get_object(IFACE_TYPE_CON, IFACE_NAME_CON);
            }
            if (buf_ptr != NULL)
            {
                if (buf_ptr->rx.is_not_full())
                {
                    buf_ptr->rx.add(msg);
                }
                else
                {
                    console_send_string(TAG_RED "Error!" TAG_DEF " Console TX buffer overflow.\r\n");
                }
            }
        }
    }
    else
    {
        console_send_string(parser_str_err_syntax_cmd);
    }
    
    return false;
}
