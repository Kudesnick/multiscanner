//------------------------------------------------------------------------------
// Задание настроек консоли
//------------------------------------------------------------------------------

#include <stdint.h>
#include <string.h>

#include "iface.h"
#include "thread_con.h"
#include "parser.h"
#include "parser_set.h"
#include "parser_set_con.h"

static bsp_con * ptr = NULL;
static bsp_con_config_t * sett = NULL;

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
            result = cmd_list[i].func(*str, cmd_list[i].param);
        }
        else
        {
            console_send_string(parser_str_err_syntax_cmd);
            result = false;
        }
    }
    
    return result;
}

static bool parser_set_baud(char * str, const void * param)
{
    console_send_string("Test complete\r\n");

    return true;
}

bool parser_set_con(char * str, const void * param)
{
    static const parse_fsm_steps_t cmd_list[] =
    {
        {          "-b", parser_set_baud, NULL},
        {       "-baud", parser_set_baud, NULL},
        {   "-baudrate", parser_set_baud, NULL},
        {         "-pn",            NULL, NULL},
        {          "-n",            NULL, NULL},
        {  "-parity-no",            NULL, NULL},
        {         "-no",            NULL, NULL},
        {         "-pe",            NULL, NULL},
        {          "-e",            NULL, NULL},
        {"-parity-even",            NULL, NULL},
        {       "-even",            NULL, NULL},
        {         "-po",            NULL, NULL},
        {          "-o",            NULL, NULL},
        { "-parity-odd",            NULL, NULL},
        {        "-odd",            NULL, NULL},
        {         "-s1",            NULL, NULL},
        {      "-stop1",            NULL, NULL},
        {         "-s2",            NULL, NULL},
        {      "-stop2",            NULL, NULL},
        {       "-echo",            NULL, NULL},
        {      "-color",            NULL, NULL},
    };
    
    bsp_con * ptr = (bsp_con *)bsp_unit::get_object(IFACE_TYPE_CON, (iface_name_t)(uint32_t)param);
    
    if (ptr != NULL)
    {
        bsp_con_config_t * sett = ptr->get_setting();
        
        if (parser_iteration(&str, cmd_list, countof_arr(cmd_list)) == true)
        {
            console_send_string("New console setting.\r\n");
        }
    }
    else
    {
        console_send_string(TAG_RED "Error!" TAG_DEF " Invalid name of the interface.\r\n");
    }

    return false;
}
