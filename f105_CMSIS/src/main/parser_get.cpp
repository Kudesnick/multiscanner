//------------------------------------------------------------------------------
// Получение настроек интерфейсов
//------------------------------------------------------------------------------

#include <stdint.h>
#include <string.h>

#include "iface.h"
#include "thread_con.h"
#include "parser.h"
#include "parser_get.h"
#include "bsp_con.h"

static void parser_get_parity_decode(uint16_t parity)
{
    console_send_string("\tparity: ");
    
    switch (parity)
    {
        case USART_Parity_No:
            console_send_string("no");
            break;
        case USART_Parity_Even:
            console_send_string("even");
            break;
        case USART_Parity_Odd:
            console_send_string("odd");
            break;
    }
    console_send_string(" parity\r\n");
};

static void parser_get_stop_bits_decode(uint16_t stop_bits)
{
    console_send_string("\tstop bits: ");

    switch (stop_bits)
    {
        case USART_StopBits_1:
            console_send_string("1 bit");
            break; 
        case USART_StopBits_0_5:
            console_send_string("0.5 bits");
            break;
        case USART_StopBits_2:
            console_send_string("2 bits");
            break; 
        case USART_StopBits_1_5:
            console_send_string("1.5 bits");
            break; 
    }

    console_send_string("\r\n");
};

static void parser_get_enable_decode(bool enable)
{
    if (enable)
    {
        console_send_string("enabled\r\n");
    }
    else
    {
        console_send_string("disabled\r\n");
    }
};

static void parser_get_yes_decode(bool enable)
{
    if (enable)
    {
        console_send_string("yes\r\n");
    }
    else
    {
        console_send_string("no\r\n");
    }
};

static bool parser_get_con(char ** str, const void * param)
{
    bsp_con * ptr = (bsp_con *)bsp_unit::get_object(IFACE_TYPE_CON, (iface_name_t)(uint32_t)param);
    
    if (ptr != NULL)
    {
        bsp_con_config_t * sett = ptr->get_setting();
        
        console_send_string("Current the console settings:\r\n");
            console_send_string("\tbaudrate: ");
                console_send_string(parser_uint_to_str(sett->baudrate));
                console_send_string("\r\n");
            parser_get_parity_decode(sett->parity);
            parser_get_stop_bits_decode(sett->stop_bits);
            console_send_string("\techo of transmit: ");
                parser_get_enable_decode(sett->echo);
            console_send_string("\tcolorised console: ");
                if (sett->color)
                {
                    console_send_string("\x1b[31me\x1b[32mn\x1b[33ma\x1b[34mb\x1b[35ml\x1b[36me\x1b[37md\x1b[0m\r\n");
                }
                else
                {
                    console_send_string("disabled\r\n");
                }
    }
    
    return false;
}

bool parser_get(char ** str, const void * param)
{
    static const parse_fsm_steps_t cmd_list[] =
    {
        { "con", parser_get_con, (uint32_t *)IFACE_NAME_CON},
        {"can1", NULL, NULL},
        {"can2", NULL, NULL},
        {"lin1", NULL, NULL},
        {"lin2", NULL, NULL},
        {"urt1", NULL, NULL},
        {"urt2", NULL, NULL},
    };

    parser_recursion(str, cmd_list, countof_arr(cmd_list));

    return false;
}
