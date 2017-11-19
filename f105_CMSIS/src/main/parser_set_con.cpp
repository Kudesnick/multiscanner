//------------------------------------------------------------------------------
// Задание настроек консоли
//------------------------------------------------------------------------------

#include <stdint.h>
#include <string.h>

#include "thread_con.h"
#include "parser.h"
#include "parser_set.h"
#include "parser_set_con.h"

static bsp_con_config_t sett;

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
    bool result = true;
    
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
            result = cmd_list[i].func(str, cmd_list[i].param);
        }
        else
        {
            console_send_string(parser_str_err_syntax_cmd);
            result = false;
        }
    }
    
    return result;
}

static bool parser_set_baud(char ** str, const void * param)
{
    bool result = false;
    
    char * tmp_str = *str;
    uint32_t baud = parser_str_to_uint(&tmp_str);
    
    if (tmp_str != NULL)
    {
        *str = tmp_str;
        sett.baudrate = baud;
        result = true;
    }

    return result;
}

static bool parser_set_parity(char ** str, const void * param)
{
    bool result = false;
    
    uint16_t parity = (uint32_t)param;
    
    if (parity == USART_Parity_No   ||
        parity == USART_Parity_Even ||
        parity == USART_Parity_Odd
        )
    {
        sett.parity = parity;
        result = true;
    }
    
    return result;
}

static bool parser_set_stop_bits(char ** str, const void * param)
{
    bool result = false;
    
    uint16_t stop_bits = (uint32_t)param;
    
    if (stop_bits == USART_StopBits_1   ||
        stop_bits == USART_StopBits_0_5 ||
        stop_bits == USART_StopBits_2   ||
        stop_bits == USART_StopBits_1_5
        )
    {
        sett.stop_bits = stop_bits;
        result = true;
    }
    
    return result;
}

static bool parser_set_echo(char ** str, const void * param)
{
    bool yn;
    bool result = parser_yn_decode(str, &yn);
    
    if (result)
    {
        sett.echo = yn;
    }
    
    return result;
}

static bool parser_set_color(char ** str, const void * param)
{
    bool yn;
    bool result = parser_yn_decode(str, &yn);
    
    if (result)
    {
        sett.color = yn;
    }
    
    return result;
}

bool parser_set_con(char ** str, const void * param)
{
    static const parse_fsm_steps_t cmd_list[] =
    {
        {          "-b", parser_set_baud     , NULL},
        {       "-baud", parser_set_baud     , NULL},
        {   "-baudrate", parser_set_baud     , NULL},
        {         "-pn", parser_set_parity   , (uint32_t *)USART_Parity_No},
        {          "-n", parser_set_parity   , (uint32_t *)USART_Parity_No},
        {  "-parity-no", parser_set_parity   , (uint32_t *)USART_Parity_No},
        {         "-no", parser_set_parity   , (uint32_t *)USART_Parity_No},
        {         "-pe", parser_set_parity   , (uint32_t *)USART_Parity_Even},
        {          "-e", parser_set_parity   , (uint32_t *)USART_Parity_Even},
        {"-parity-even", parser_set_parity   , (uint32_t *)USART_Parity_Even},
        {       "-even", parser_set_parity   , (uint32_t *)USART_Parity_Even},
        {         "-po", parser_set_parity   , (uint32_t *)USART_Parity_Odd},
        {          "-o", parser_set_parity   , (uint32_t *)USART_Parity_Odd},
        { "-parity-odd", parser_set_parity   , (uint32_t *)USART_Parity_Odd},
        {        "-odd", parser_set_parity   , (uint32_t *)USART_Parity_Odd},
        {         "-s1", parser_set_stop_bits, (uint32_t *)USART_StopBits_1},
        {      "-stop1", parser_set_stop_bits, (uint32_t *)USART_StopBits_1},
        {         "-s2", parser_set_stop_bits, (uint32_t *)USART_StopBits_2},
        {      "-stop2", parser_set_stop_bits, (uint32_t *)USART_StopBits_2},
        {       "-echo", parser_set_echo     , NULL},
        {      "-color", parser_set_color    , NULL},
    };
    
    bsp_con * ptr = (bsp_con *)bsp_unit::get_object(IFACE_TYPE_CON, (iface_name_t)(uint32_t)param);
    
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
    }
    else
    {
        console_send_string(TAG_RED "Error!" TAG_DEF " Invalid name of the interface.\r\n");
    }

    return false;
}
