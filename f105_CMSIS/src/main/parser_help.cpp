//------------------------------------------------------------------------------
// Общие команды и типы для парсера строк
//------------------------------------------------------------------------------

#include <stdint.h>
#include <string.h>

#include "thread_con.h"
#include "parser.h"
#include "parser_help.h"

static bool parser_help_get(char * str, const void * param)
{
    const parse_fsm_steps_t cmd_list[] =
    {
        { "con", NULL, NULL},
        {"can1", NULL, NULL},
        {"can2", NULL, NULL},
        {"lin1", NULL, NULL},
        {"lin2", NULL, NULL},
        {"urt1", NULL, NULL},
        {"urt2", NULL, NULL},
    };
    
    int16_t i;
    
    if (str[0] == 0)
    {
        console_send_string("Get interface setting. Enter" TAG_LT "help get [");
        
        for (i = 0; i < countof_arr(cmd_list); i++)
        {
            if (i > 0)
            {
                console_send_string("|");
            }
            console_send_string(cmd_list[i].name);
        }

        console_send_string("]" TAG_GT " to learn more details.\r\n");
    }

#warning under construction
    if ((i = parser_find(&str, cmd_list, countof_arr(cmd_list))) < 0)
        {
            console_send_string(parser_str_err_bad_cmd);
        }
        else if (cmd_list[i].func == NULL)
        {
            console_send_string(parser_str_err_null_cmd);
        }
        else
        {
            cmd_list[i].func(str, cmd_list[i].param);
        }

    return false;
    
};
    
static bool parser_help_set(char * str, const void * param)
{
    const parse_fsm_steps_t cmd_list[] =
    {
        { "con", NULL, NULL}, // Сводная информация об устройстве (версия, поддерживаемые интерфейсы и пр.)
        {"can1", NULL, NULL}, // Отправить сообщение по can1
        {"can2", NULL, NULL}, // Отправить сообщение по can2
        {"lin1", NULL, NULL}, // Отправить сообщение по lin1
        {"lin2", NULL, NULL}, // Отправить сообщение по lin2
        {"urt1", NULL, NULL}, // Отправить сообщение по uart1
        {"urt2", NULL, NULL}, // Отправить сообщение по uart2
    };
};

bool parser_help(char * str, const void * param) // Получить справку о программе
{
#warning синтаксис неправильный. Просто скопипастил по пьяни.
    const char str_can[] = "Send CAN message. Syntax:\r\n"
                           TAG_LT "[can1|can2] 0x<id hex num> <data length> <data hex format without whitespace> -n(nask) -r(retransmit) <time> <count (0 - infinity)>" TAG_GT "\r\n";
    const char str_lin[] = "Send LIN message. Syntax:\r\n"
                           TAG_LT "[lin1|lin2] 0x<id hex num> <data hex format without whitespace> -s(standard checksum) -r(retransmit) <time (ms)> <count (0 - infinity)>" TAG_GT "\r\n";
    const char str_uart[] = "Send UART message. Syntax:\r\n"
                            TAG_LT "[urt1|urt2] <data hex format without whitespace> -r(retransmit) <time> <count (0 - infinity)>" TAG_GT "\r\n";

    const parse_fsm_steps_t cmd_list[] =
    {
        { "get", NULL, NULL}, // Получить параметры настройки интерфейса
        { "set", NULL, NULL}, // Настроить интерфейс
        {"can1", NULL, str_can},  // Отправить сообщение по can1
        {"can2", NULL, str_can},  // Отправить сообщение по can2
        {"lin1", NULL, str_lin},  // Отправить сообщение по lin1
        {"lin2", NULL, str_lin},  // Отправить сообщение по lin2
        {"urt1", NULL, str_uart}, // Отправить сообщение по uart1
        {"urt2", NULL, str_uart}, // Отправить сообщение по uart2
    };

    if (str[0] != '\0')
    {
        parser_recursion(&str, cmd_list, countof_arr(cmd_list));
    }
    else
    {
        console_send_string("Enter" TAG_LT "[help|?] [");
        parser_all_param_to_str(cmd_list, countof_arr(cmd_list));
        console_send_string("]" TAG_GT "\r\n"
                            "for get help on command or enter" TAG_LT "[info|about]" TAG_GT "to get device information.\r\n");
    }
    
    return false;
}
