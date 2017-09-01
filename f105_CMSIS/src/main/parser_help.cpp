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

bool parser_info(char * str, const void * param) // Сводная информация об устройстве (версия, поддерживаемые интерфейсы и пр.)
{
    if (str[0] != '\0')
    {
        console_send_string(parser_str_err_syntax_cmd);
    }
    else
    {
        console_send_string("Multiscanner " TAG_GREEN "v0.0.0a" TAG_DEF "\r\n"
                            "\tbuild date: " TAG_GREEN __DATE__ " [" __TIME__ "]" TAG_DEF "\r\n"
                            "\tAuthor: " TAG_GREEN "Stulov Tikhon" TAG_DEF " (" TAG_BLUE_U "kudesnick@inbox.ru" TAG_DEF ")\r\n"
                            "\t\r\n"
                            "\tConnect to PC interfaces support: " TAG_RED_B "UNDER CONSTRUCTION" TAG_DEF "\r\n"
                            "\tExternal interfaces support: " TAG_RED_B "UNDER CONSTRUCTION" TAG_DEF "\r\n");
    }

    return false;
}

bool parser_help(char * str, const void * param) // Получить справку о программе
{
    const parse_fsm_steps_t cmd_list[] =
    {
        {"info", parser_info, NULL}, // Сводная информация об устройстве (версия, поддерживаемые интерфейсы и пр.)
        { "get",        NULL, NULL}, // Получить параметры настройки интерфейса
        { "set",        NULL, NULL}, // Настроить интерфейс
        {"can1",        NULL, "Send CAN msg. Syntax:\r\n"},  // Отправить сообщение по can1
        {"can2",        NULL, "Send CAN msg. Syntax:\r\n"},  // Отправить сообщение по can2
        {"lin1",        NULL, "Send LIN msg. Syntax:\r\n"},  // Отправить сообщение по lin1
        {"lin2",        NULL, "Send LIN msg. Syntax:\r\n"},  // Отправить сообщение по lin2
        {"urt1",        NULL, "Send UART msg. Syntax:\r\n"}, // Отправить сообщение по uart1
        {"urt2",        NULL, "Send UART msg. Syntax:\r\n"}, // Отправить сообщение по uart2
    };
    
    bool common = true;
    
    int16_t i;
    
    if (str[0] != '\0')
    {
        if ((i = parser_find(&str, cmd_list, countof_arr(cmd_list))) < 0)
        {
            console_send_string(parser_str_err_bad_cmd);
        }
        else if (cmd_list[i].func == NULL)
        {
            console_send_string(parser_str_err_null_cmd);

            common = false;
        }
        else
        {
            cmd_list[i].func(str, cmd_list[i].param);
            
            common = false;
        }
    }
    
    if (common)
    {
        console_send_string("Enter" TAG_LT "help [");

        for (i = 0; i < countof_arr(cmd_list); i++)
        {
            if (i > 0)
            {
                console_send_string("|");
            }
            console_send_string(cmd_list[i].name);
        }

        console_send_string("]" TAG_GT "\r\n"
                            "for get help on command or enter" TAG_LT "info" TAG_GT "to get device information.\r\n");
    }
    
    return false;
}
