//------------------------------------------------------------------------------
// Задание настроек интерфейсов
//------------------------------------------------------------------------------

#include <stdint.h>
#include <string.h>

#include "thread_con.h"
#include "parser.h"
#include "parser_set.h"
#include "parser_set_con.h"

static bool parser_set_def(char ** str, const void * param)
{
    bsp_con * ptr = (bsp_con *)bsp_con::cpp_list<LIST_TYPE_UNIT>::get_object(IFACE_TYPE_CON, (iface_name_t)(uint32_t)param);
    
    if (ptr != NULL)
    {
        //bsp_con_config_t * sett = ptr->get_setting();
        
        console_send_string("Test complete\r\n");
    }
    
    return false;
}

bool parser_set(char ** str, const void * param)
{
    static const parse_fsm_steps_t cmd_list[] =
    {
        { "con", parser_set_con, (uint32_t *)IFACE_NAME_CON},
        #if (CAN1_ENABLED != 0)
            {"can1", parser_set_def, (uint32_t *)IFACE_NAME_CAN1},
        #endif
        #if (CAN2_ENABLED != 0)
            {"can2", parser_set_def, (uint32_t *)IFACE_NAME_CAN2},
        #endif
        #if (LIN1_ENABLED != 0)
            {"lin1", parser_set_def, (uint32_t *)IFACE_NAME_LIN1},
        #endif
        #if (LIN2_ENABLED != 0)
            {"lin2", parser_set_def, (uint32_t *)IFACE_NAME_LIN2},
        #endif
        #if (UART1_ENABLED != 0)
            {"urt1", parser_set_def, (uint32_t *)IFACE_NAME_UART1},
        #endif
        #if (UART2_ENABLED != 0)
            {"urt2", parser_set_def, (uint32_t *)IFACE_NAME_UART2},
        #endif
    };

    parser_recursion(str, cmd_list, countof_arr(cmd_list));

    return false;
}
