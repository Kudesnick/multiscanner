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
        **(result_t **)result = res;
        success = true;
    }
    else
    {
        *(result_t **)result = (result_t *)param;
    }

    return success;
}

static bool hex_str_to_byte_array(char ** str, uint8_t * arr, uint8_t * size)
{
#warning Реализовать парсинг
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
    
    if (hex_str_to_byte_array(str, msg.uart.data, &msg.uart.len) == true)
    {
        if (   msg.uart.len == 0
            || msg.uart.len > sizeof(msg.uart.data))
        {
            console_send_string(TAG_RED "Error!" TAG_DEF " Data length is invalid.\r\n");
        }
        
        else if (parser_iteration(str, cmd_list, countof_arr(cmd_list)) != true)
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
#warning Реализовать наполнение структуры и отправку в буфер
        }
    }
    else
    {
        console_send_string(parser_str_err_syntax_cmd);
    }
    
    
}
