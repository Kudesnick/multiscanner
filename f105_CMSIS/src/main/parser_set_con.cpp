//------------------------------------------------------------------------------
// «адание настроек консоли
//------------------------------------------------------------------------------

#include <stdint.h>
#include <string.h>

#include "thread_con.h"
#include "thread_serial.h"
#include "parser.h"
#include "parser_set.h"
#include "parser_set_con.h"

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
// ‘ункции обработки параметров. —оответствуют сигнатуре
// bool(* const func)(char ** str, const void * const param, void * const result)
//------------------------------------------------------------------------------

static bool parser_set_baud(char ** str, const void * const param, void * const result)
{
    bool success = false;
    
    char * tmp_str = *str;
    uint32_t baud = parser_str_to_uint(&tmp_str);
    
    if (tmp_str != NULL)
    {
        *str = tmp_str;
        *(uint32_t *)result = baud;
        success = true;
    }

    return success;
}

static bool parser_set_parity(char ** str, const void * const param, void * const result)
{
    bool success = false;
    
    uint16_t parity = (uint32_t)param;
    
    if (parity == USART_Parity_No   ||
        parity == USART_Parity_Even ||
        parity == USART_Parity_Odd
        )
    {
        *(uint16_t *)result = parity;
        success = true;
    }
    
    return success;
}

static bool parser_set_stop_bits(char ** str, const void * const param, void * const result)
{
    bool success = false;
    
    uint16_t stop_bits = (uint32_t)param;
    
    if (stop_bits == USART_StopBits_1   ||
        stop_bits == USART_StopBits_0_5 ||
        stop_bits == USART_StopBits_2   ||
        stop_bits == USART_StopBits_1_5
        )
    {
        *(uint16_t *)result = stop_bits;
        success = true;
    }
    
    return success;
}

static bool parser_set_echo(char ** str, const void * const param, void * const result)
{
    bool yn;
    bool success = parser_yn_decode(str, &yn);
    
    if (success)
    {
        *(bool *)result = yn;
    }
    
    return success;
}

static bool parser_set_color(char ** str, const void * const param, void * const result)
{
    bool yn;
    bool success = parser_yn_decode(str, &yn);
    
    if (success)
    {
        *(bool *)result = yn;
    }
    
    return success;
}

bool parser_set_con(char ** str, const void * const param, void * const result)
{
    static bsp_con_config_t sett;
    static const parse_fsm_steps_t cmd_list[] =
    {
        {          "-b", parser_set_baud     ,             NULL             , (void *)&sett.baudrate  },
        {       "-baud", parser_set_baud     ,             NULL             , (void *)&sett.baudrate  },
        {   "-baudrate", parser_set_baud     ,             NULL             , (void *)&sett.baudrate  },
        {         "-pn", parser_set_parity   , (uint32_t *)USART_Parity_No  , (void *)&sett.parity    },
        {          "-n", parser_set_parity   , (uint32_t *)USART_Parity_No  , (void *)&sett.parity    },
        {  "-parity-no", parser_set_parity   , (uint32_t *)USART_Parity_No  , (void *)&sett.parity    },
        {         "-no", parser_set_parity   , (uint32_t *)USART_Parity_No  , (void *)&sett.parity    },
        {         "-pe", parser_set_parity   , (uint32_t *)USART_Parity_Even, (void *)&sett.parity    },
        {          "-e", parser_set_parity   , (uint32_t *)USART_Parity_Even, (void *)&sett.parity    },
        {"-parity-even", parser_set_parity   , (uint32_t *)USART_Parity_Even, (void *)&sett.parity    },
        {       "-even", parser_set_parity   , (uint32_t *)USART_Parity_Even, (void *)&sett.parity    },
        {         "-po", parser_set_parity   , (uint32_t *)USART_Parity_Odd , (void *)&sett.parity    },
        {          "-o", parser_set_parity   , (uint32_t *)USART_Parity_Odd , (void *)&sett.parity    },
        { "-parity-odd", parser_set_parity   , (uint32_t *)USART_Parity_Odd , (void *)&sett.parity    },
        {        "-odd", parser_set_parity   , (uint32_t *)USART_Parity_Odd , (void *)&sett.parity    },
        {         "-s1", parser_set_stop_bits, (uint32_t *)USART_StopBits_1 , (void *)&sett.stop_bits },
        {      "-stop1", parser_set_stop_bits, (uint32_t *)USART_StopBits_1 , (void *)&sett.stop_bits },
        {         "-s2", parser_set_stop_bits, (uint32_t *)USART_StopBits_2 , (void *)&sett.stop_bits },
        {      "-stop2", parser_set_stop_bits, (uint32_t *)USART_StopBits_2 , (void *)&sett.stop_bits },
        {       "-echo", parser_set_echo     ,             NULL             , (void *)&sett.echo      },
        {      "-color", parser_set_color    ,             NULL             , (void *)&sett.color     },
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

bool parser_set_urt(char ** str, const void * const param, void * const result)
{
    static bsp_serial_config_t sett;
    static const parse_fsm_steps_t cmd_list[] =
    {
        {          "-b", parser_set_baud      ,             NULL             , (void *)&sett.baudrate     },
        {       "-baud", parser_set_baud      ,             NULL             , (void *)&sett.baudrate     },
        {   "-baudrate", parser_set_baud      ,             NULL             , (void *)&sett.baudrate     },
        {         "-pn", parser_set_parity    , (uint32_t *)USART_Parity_No  , (void *)&sett.parity       },
        {          "-n", parser_set_parity    , (uint32_t *)USART_Parity_No  , (void *)&sett.parity       },
        {  "-parity-no", parser_set_parity    , (uint32_t *)USART_Parity_No  , (void *)&sett.parity       },
        {         "-no", parser_set_parity    , (uint32_t *)USART_Parity_No  , (void *)&sett.parity       },
        {         "-pe", parser_set_parity    , (uint32_t *)USART_Parity_Even, (void *)&sett.parity       },
        {          "-e", parser_set_parity    , (uint32_t *)USART_Parity_Even, (void *)&sett.parity       },
        {"-parity-even", parser_set_parity    , (uint32_t *)USART_Parity_Even, (void *)&sett.parity       },
        {       "-even", parser_set_parity    , (uint32_t *)USART_Parity_Even, (void *)&sett.parity       },
        {         "-po", parser_set_parity    , (uint32_t *)USART_Parity_Odd , (void *)&sett.parity       },
        {          "-o", parser_set_parity    , (uint32_t *)USART_Parity_Odd , (void *)&sett.parity       },
        { "-parity-odd", parser_set_parity    , (uint32_t *)USART_Parity_Odd , (void *)&sett.parity       },
        {        "-odd", parser_set_parity    , (uint32_t *)USART_Parity_Odd , (void *)&sett.parity       },
        {         "-s1", parser_set_stop_bits , (uint32_t *)USART_StopBits_1 , (void *)&sett.stop_bits    },
        {      "-stop1", parser_set_stop_bits , (uint32_t *)USART_StopBits_1 , (void *)&sett.stop_bits    },
        {         "-s2", parser_set_stop_bits , (uint32_t *)USART_StopBits_2 , (void *)&sett.stop_bits    },
        {      "-stop2", parser_set_stop_bits , (uint32_t *)USART_StopBits_2 , (void *)&sett.stop_bits    },
        {          "-l", parser_set_msg_len   ,             NULL             , (void *)&sett.max_len      },
        {        "-len", parser_set_msg_len   ,             NULL             , (void *)&sett.max_len      },
        {      "-start", parser_set_start_byte,             NULL             , (void *)&sett.byte_of_begin},
        {        "-end", parser_set_end_byte  ,             NULL             , (void *)&sett.byte_of_end  },
        {       "-echo", parser_set_echo      ,             NULL             , (void *)&sett.echo         },
        {         "-on", parser_set_enabled   , (uint32_t *)'y'              , (void *)&sett.enable       },
        {     "-enable", parser_set_enabled   , (uint32_t *)'y'              , (void *)&sett.enable       },
        {        "-off", parser_set_enabled   , (uint32_t *)'n'              , (void *)&sett.enable       },
        {    "-disable", parser_set_enabled   , (uint32_t *)'n'              , (void *)&sett.enable       },
        {    "-enabled", parser_set_enabled   ,             NULL             , (void *)&sett.enable       },
    };

    bsp_serial * ptr = (bsp_serial *)bsp_serial::cpp_list<LIST_TYPE_UNIT>::get_object(IFACE_TYPE_UART, (iface_name_t)(uint32_t)param);

    if (ptr != NULL)
    {
        sett = *ptr->get_setting();
        
        if (parser_iteration(str, cmd_list, countof_arr(cmd_list)) == true)
        {
            // ѕроверка бодрейта
#warning сдесь же проверить вс€кие паритеты и прочие числовые значени€ на соответствие диапазону
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
