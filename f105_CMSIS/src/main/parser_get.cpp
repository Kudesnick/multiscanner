//------------------------------------------------------------------------------
// Получение настроек интерфейсов
//------------------------------------------------------------------------------

#include <stdint.h>
#include <string.h>

#include "thread_con.h"
#include "parser.h"
#include "parser_get.h"
#include "bsp_con.h"

bool parser_get(char * str, const void * param)
{
    static const parse_fsm_steps_t cmd_list[] =
    {
        { "con", NULL, NULL},
        {"can1", NULL, NULL},
        {"can2", NULL, NULL},
        {"lin1", NULL, NULL},
        {"lin2", NULL, NULL},
        {"urt1", NULL, NULL},
        {"urt2", NULL, NULL},
    };

    parser_recursion(&str, cmd_list, countof_arr(cmd_list));

    return false;
}
