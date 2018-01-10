//------------------------------------------------------------------------------
// Получение настроек интерфейсов
//------------------------------------------------------------------------------

#include <stdint.h>
#include <string.h>

#include "thread_con.h"
#include "parser.h"
#include "parser_get.h"
#include "bsp_con.h"
#include "bsp_serial.h"

#define TAG_PARAM TAG_GREEN

static void parser_get_baudrate_decode(uint32_t baud)
{
    console_send_string("\tbaudrate: " TAG_PARAM);
    console_send_string(parser_uint_to_str(baud));
    console_send_string(TAG_DEF "\r\n");
}

static void parser_get_parity_decode(uint16_t parity)
{
    console_send_string("\tparity: " TAG_PARAM);
    
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
    console_send_string(" parity" TAG_DEF "\r\n");
};

static void parser_get_stop_bits_decode(uint16_t stop_bits)
{
    console_send_string("\tstop bits: " TAG_PARAM);

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

    console_send_string(TAG_DEF "\r\n");
};

static void parser_get_echo_decode(bool echo)
{
    console_send_string("\techo of transmit: " TAG_PARAM);
    console_send_string(parser_bool_to_enable_disable(echo));
    console_send_string(TAG_DEF "\r\n");
};

static void parser_get_enable_decode(bool enable)
{
    console_send_string("\tenabled interface: " TAG_PARAM);
    console_send_string(parser_bool_to_yes_no(enable));
    console_send_string(TAG_DEF "\r\n");
};

static bool parser_get_con(char ** str, const void * const param, void * const result)
{
    bsp_con * ptr = (bsp_con *)bsp_con::cpp_list<LIST_TYPE_UNIT>::get_object(IFACE_TYPE_CON, (iface_name_t)(uint32_t)param);
    
    if (ptr != NULL)
    {
        bsp_con_config_t * sett = ptr->get_setting();
        
        console_send_string("Current the console settings:\r\n");
            parser_get_baudrate_decode(sett->baudrate);
            parser_get_parity_decode(sett->parity);
            parser_get_stop_bits_decode(sett->stop_bits);
            parser_get_echo_decode(sett->echo);
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
    else
    {
        console_send_string(parser_str_err_iface_fnd);
    }
    
    return false;
};

static bool parser_get_urt(char ** str, const void * const param, void * const result)
{
    bsp_serial * ptr = (bsp_serial *)bsp_serial::cpp_list<LIST_TYPE_UNIT>::get_object(IFACE_TYPE_UART, (iface_name_t)(uint32_t)param);
    
    if (ptr != NULL)
    {
        bsp_serial_config_t * sett = ptr->get_setting();
        
        console_send_string("Current the USART interface settings:\r\n");
            parser_get_baudrate_decode(sett->baudrate);
            parser_get_parity_decode(sett->parity);
            parser_get_stop_bits_decode(sett->stop_bits);
            console_send_string("\tbyte of begin: " TAG_PARAM);
                if ((sett->byte_of_begin & 0xFF) != sett->byte_of_begin)
                {
                    console_send_string("not set");
                }
                else
                {
                    console_send_string("0x");
                    console_send_string(parser_uint_to_hex(sett->byte_of_begin, sizeof(uint8_t)*2));
                }
                console_send_string(TAG_DEF "\r\n");
            console_send_string("\tbyte of end: " TAG_PARAM);
                if ((sett->byte_of_end & 0xFF) != sett->byte_of_end)
                {
                    console_send_string("not set");
                }
                else
                {
                    console_send_string("0x");
                    console_send_string(parser_uint_to_hex(sett->byte_of_end, sizeof(uint8_t)*2));
                }
                console_send_string(TAG_DEF "\r\n");
            console_send_string("\tmax message length: " TAG_PARAM);
                if (sett->max_len > 0)
                    console_send_string(parser_uint_to_str(sett->max_len));
                else
                    console_send_string(parser_uint_to_str(UART_DATA_LEN_MAX));
                console_send_string(TAG_DEF "\r\n");
            parser_get_echo_decode(sett->echo);
            parser_get_enable_decode(sett->enable);
    }
    else
    {
        console_send_string(parser_str_err_iface_fnd);
    }
    
    return false;
};

bool parser_get(char ** str, const void * const param, void * const result)
{
    static const parse_fsm_steps_t cmd_list[] =
    {
        { "con", parser_get_con, (void *)IFACE_NAME_CON},
        {"can1", NULL, NULL},
        {"can2", NULL, NULL},
        {"lin1", NULL, NULL},
        {"lin2", NULL, NULL},
        {"urt1", parser_get_urt, (void *)IFACE_NAME_UART1},
        {"urt2", parser_get_urt, (void *)IFACE_NAME_UART2},
    };

    parser_recursion(str, cmd_list, countof_arr(cmd_list));

    return false;
}
