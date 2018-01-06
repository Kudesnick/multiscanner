//------------------------------------------------------------------------------
// Задание настроек консоли
//------------------------------------------------------------------------------

#include <stdint.h>
#include <string.h>

#include "thread_con.h"
#include "thread_serial.h"
#include "parser.h"
#include "parser_set.h"

static bool parser_yn_decode(char ** str, bool *res)
{
    bool result = true;
    
    switch (*str[0])
    {
        case 'y':
        {
            *res = true;
            *str += sizeof(char);
            break;
        }
        case 'n':
        {
            *res = false;
            *str += sizeof(char);
            break;
        }
        default:
            result = false;
            break;
    }
    
    if (result == true
        && *str[0] == ' '
        )
    {
        *str += sizeof(char);
    }
    
    return result;
}

static bool parser_iteration(char ** str, const parse_fsm_steps_t * cmd_list, uint16_t cmd_list_len)
{
    bool result = *str[0] != '\0';
    
    while (*str[0] != '\0'
           && result == true
           )
    {
        int16_t i;
        
        if ((i = parser_find(str, cmd_list, cmd_list_len)) < 0)
        {
            console_send_string(parser_str_err_bad_cmd);
            result = false;
        }
        else if (cmd_list[i].func != NULL)
        {
            result = cmd_list[i].func(str, cmd_list[i].param, cmd_list[i].result);
        }
        else
        {
            console_send_string(parser_str_err_syntax_cmd);
            result = false;
        }
    }
    
    return result;
}

//------------------------------------------------------------------------------
// Функции обработки параметров. Соответствуют сигнатуре
// bool(* const func)(char ** str, const void * const param, void * const result)
//------------------------------------------------------------------------------

template <typename result_t>
static bool parser_set_uint(char ** str, const void * const param, void * const result)
{
    bool success = false;
    
    if (param != NULL)
    {
        *(result_t *)result = *(result_t*)param;
        success = true;
    }
    else
    {
        char * tmp_str = *str;
        result_t res = parser_str_to_uint(&tmp_str);
        
        if (tmp_str != NULL)
        {
            *str = tmp_str;
            *(result_t *)result = res;
            success = true;
        }
    }

    return success;
}

static bool parser_set_bool(char ** str, const void * const param, void * const result)
{
    bool success = false;

    if (param != NULL)
    {
        *(bool *)result = *(bool *)param;
        success = true;
    }
    else
    {
        bool yn;
        success = parser_yn_decode(str, &yn);
        
        if (success)
        {
            *(bool *)result = yn;
        }
    }

    return success;
}

static bool parser_set_con(char ** str, const void * const param, void * const result)
{
    static bsp_con_config_t sett;
    static const uint16_t parity_no = USART_Parity_No;
    static const uint16_t parity_even = USART_Parity_Even;
    static const uint16_t parity_odd = USART_Parity_Odd;
    static const uint16_t stop_1 = USART_StopBits_1;
    static const uint16_t stop_2 = USART_StopBits_2;
    static const parse_fsm_steps_t cmd_list[] =
    {
        {          "-b", parser_set_uint<uint32_t>, NULL        , (void *)&sett.baudrate },
        {       "-baud", parser_set_uint<uint32_t>, NULL        , (void *)&sett.baudrate },
        {   "-baudrate", parser_set_uint<uint32_t>, NULL        , (void *)&sett.baudrate },
        {         "-pn", parser_set_uint<uint16_t>, &parity_no  , (void *)&sett.parity   },
        {          "-n", parser_set_uint<uint16_t>, &parity_no  , (void *)&sett.parity   },
        {  "-parity-no", parser_set_uint<uint16_t>, &parity_no  , (void *)&sett.parity   },
        {         "-no", parser_set_uint<uint16_t>, &parity_no  , (void *)&sett.parity   },
        {         "-pe", parser_set_uint<uint16_t>, &parity_even, (void *)&sett.parity   },
        {          "-e", parser_set_uint<uint16_t>, &parity_even, (void *)&sett.parity   },
        {"-parity-even", parser_set_uint<uint16_t>, &parity_even, (void *)&sett.parity   },
        {       "-even", parser_set_uint<uint16_t>, &parity_even, (void *)&sett.parity   },
        {         "-po", parser_set_uint<uint16_t>, &parity_odd , (void *)&sett.parity   },
        {          "-o", parser_set_uint<uint16_t>, &parity_odd , (void *)&sett.parity   },
        { "-parity-odd", parser_set_uint<uint16_t>, &parity_odd , (void *)&sett.parity   },
        {        "-odd", parser_set_uint<uint16_t>, &parity_odd , (void *)&sett.parity   },
        {         "-s1", parser_set_uint<uint16_t>, &stop_1     , (void *)&sett.stop_bits},
        {      "-stop1", parser_set_uint<uint16_t>, &stop_1     , (void *)&sett.stop_bits},
        {         "-s2", parser_set_uint<uint16_t>, &stop_2     , (void *)&sett.stop_bits},
        {      "-stop2", parser_set_uint<uint16_t>, &stop_2     , (void *)&sett.stop_bits},
        {       "-echo", parser_set_bool          , NULL        , (void *)&sett.echo     },
        {      "-color", parser_set_bool          , NULL        , (void *)&sett.color    },
    };

    bsp_con * ptr = (bsp_con *)bsp_con::cpp_list<LIST_TYPE_UNIT>::get_object(IFACE_TYPE_CON, (iface_name_t)(uint32_t)param);
    
    if (ptr != NULL)
    {
        sett = *ptr->get_setting();
        
        if (parser_iteration(str, cmd_list, countof_arr(cmd_list)) == true)
        {
            uint32_t real_baud = ptr->round_baud(sett.baudrate);
            uint32_t delta = (real_baud >= sett.baudrate) ? real_baud - sett.baudrate : sett.baudrate - real_baud;

            if ((uint64_t)delta * 100 / sett.baudrate > 3)
            {
                console_send_string(TAG_RED "Operation aborted!" TAG_DEF " Real baudrate will be " TAG_BLUE);
                    console_send_string(parser_uint_to_str(real_baud));
                    console_send_string(TAG_DEF " baud.\r\n"
                    "the error will be more than 3%\r\n");
            }
            
            else
            {
                ptr->set_setting(&sett);
            }
        }
        else
        {
            console_send_string(parser_str_err_syntax_cmd);
        }
    }
    else
    {
        console_send_string(parser_str_err_iface_fnd);
    }

    return false;
}

static bool parser_set_urt(char ** str, const void * const param, void * const result)
{
    static bsp_serial_config_t sett;
    static const uint16_t parity_no   = USART_Parity_No;
    static const uint16_t parity_even = USART_Parity_Even;
    static const uint16_t parity_odd  = USART_Parity_Odd;
    static const uint16_t stop_1 = USART_StopBits_1;
    static const uint16_t stop_2 = USART_StopBits_2;
    static const bool on  = false;
    static const bool off = true;
    static const parse_fsm_steps_t cmd_list[] =
    {
        {          "-b", parser_set_uint<uint32_t>,         NULL        , (void *)&sett.baudrate     },
        {       "-baud", parser_set_uint<uint32_t>,         NULL        , (void *)&sett.baudrate     },
        {   "-baudrate", parser_set_uint<uint32_t>,         NULL        , (void *)&sett.baudrate     },
        {         "-pn", parser_set_uint<uint16_t>, (void *)&parity_no  , (void *)&sett.parity       },
        {          "-n", parser_set_uint<uint16_t>, (void *)&parity_no  , (void *)&sett.parity       },
        {  "-parity-no", parser_set_uint<uint16_t>, (void *)&parity_no  , (void *)&sett.parity       },
        {         "-no", parser_set_uint<uint16_t>, (void *)&parity_no  , (void *)&sett.parity       },
        {         "-pe", parser_set_uint<uint16_t>, (void *)&parity_even, (void *)&sett.parity       },
        {          "-e", parser_set_uint<uint16_t>, (void *)&parity_even, (void *)&sett.parity       },
        {"-parity-even", parser_set_uint<uint16_t>, (void *)&parity_even, (void *)&sett.parity       },
        {       "-even", parser_set_uint<uint16_t>, (void *)&parity_even, (void *)&sett.parity       },
        {         "-po", parser_set_uint<uint16_t>, (void *)&parity_odd , (void *)&sett.parity       },
        {          "-o", parser_set_uint<uint16_t>, (void *)&parity_odd , (void *)&sett.parity       },
        { "-parity-odd", parser_set_uint<uint16_t>, (void *)&parity_odd , (void *)&sett.parity       },
        {        "-odd", parser_set_uint<uint16_t>, (void *)&parity_odd , (void *)&sett.parity       },
        {         "-s1", parser_set_uint<uint16_t>, (void *)&stop_1     , (void *)&sett.stop_bits    },
        {      "-stop1", parser_set_uint<uint16_t>, (void *)&stop_1     , (void *)&sett.stop_bits    },
        {         "-s2", parser_set_uint<uint16_t>, (void *)&stop_2     , (void *)&sett.stop_bits    },
        {      "-stop2", parser_set_uint<uint16_t>, (void *)&stop_2     , (void *)&sett.stop_bits    },
        {          "-l", parser_set_uint<uint8_t> ,         NULL        , (void *)&sett.max_len      },
        {        "-len", parser_set_uint<uint8_t> ,         NULL        , (void *)&sett.max_len      },
        {      "-start", parser_set_uint<uint16_t>,         NULL        , (void *)&sett.byte_of_begin},
        {        "-end", parser_set_uint<uint16_t>,         NULL        , (void *)&sett.byte_of_end  },
        {       "-echo", parser_set_bool          ,         NULL        , (void *)&sett.echo         },
        {         "-on", parser_set_bool          , (void *)&on         , (void *)&sett.enable       },
        {     "-enable", parser_set_bool          , (void *)&on         , (void *)&sett.enable       },
        {        "-off", parser_set_bool          , (void *)&off        , (void *)&sett.enable       },
        {    "-disable", parser_set_bool          , (void *)&off        , (void *)&sett.enable       },
        {    "-enabled", parser_set_bool          ,         NULL        , (void *)&sett.enable       },
    };

    bsp_serial * ptr = (bsp_serial *)bsp_serial::cpp_list<LIST_TYPE_UNIT>::get_object(IFACE_TYPE_UART, (iface_name_t)(uint32_t)param);

    if (ptr != NULL)
    {
        sett = *ptr->get_setting();
        
        if (parser_iteration(str, cmd_list, countof_arr(cmd_list)) == true)
        {
            // Проверка бодрейта
            uint32_t real_baud = ptr->round_baud(sett.baudrate);
            uint32_t delta = (real_baud >= sett.baudrate) ? real_baud - sett.baudrate : sett.baudrate - real_baud;

            if ((uint64_t)delta * 100 / sett.baudrate > 3)
            {
                console_send_string(TAG_RED "Operation aborted!" TAG_DEF " Real baudrate will be " TAG_BLUE);
                    console_send_string(parser_uint_to_str(real_baud));
                    console_send_string(TAG_DEF " baud.\r\n"
                    "the error will be more than 3%\r\n");
            }
            
            else
            {
                ptr->set_setting(&sett);
            }
        }
        else
        {
            console_send_string(parser_str_err_syntax_cmd);
        }
    }
    else
    {
        console_send_string(parser_str_err_iface_fnd);
    }

    return false;
}


static bool parser_set_def(char ** str, const void * const param, void * const result)
{
    console_send_string("Test complete\r\n");
    
    return false;
}

bool parser_set(char ** str, const void * const param, void * const result)
{
    static const parse_fsm_steps_t cmd_list[] =
    {
        { "con", parser_set_con, (void *)IFACE_NAME_CON},
        #if (CAN1_ENABLED != 0)
            {"can1", parser_set_def, (void *)IFACE_NAME_CAN1},
        #endif
        #if (CAN2_ENABLED != 0)
            {"can2", parser_set_def, (void *)IFACE_NAME_CAN2},
        #endif
        #if (LIN1_ENABLED != 0)
            {"urt1", parser_set_urt, (void *)IFACE_NAME_UART1},
            {"lin1", parser_set_def, (void *)IFACE_NAME_LIN1},
        #endif
        #if (LIN2_ENABLED != 0)
            {"urt2", parser_set_urt, (void *)IFACE_NAME_UART2},
            {"lin2", parser_set_def, (void *)IFACE_NAME_LIN2},
        #endif
    };

    parser_recursion(str, cmd_list, countof_arr(cmd_list));

    return false;
}
