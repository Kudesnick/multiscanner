//------------------------------------------------------------------------------
// Общие команды и типы для парсера строк
//------------------------------------------------------------------------------

#include <stdint.h>
#include <string.h>

#include "thread_con.h"
#include "parser.h"
#include "parser_help.h"
   
static bool parser_help_set(char * str, const void * param)
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

    if (str[0] != '\0')
    {
        parser_recursion(&str, cmd_list, countof_arr(cmd_list));
    }
    else
    {
        console_send_string("Setup the interface.\r\n"
                            "\tEnter" TAG_LT "help get <");
                                parser_all_param_to_str(cmd_list, countof_arr(cmd_list));
                                console_send_string(">" TAG_GT "\r\n"
                            "\tto get help on setup the interface.\r\n");
    }

    return false;
};

bool parser_help(char * str, const void * param) // Получить справку о программе
{
    const char str_get[] = "Get the interface settings.\r\n"
                            "\tEnter" TAG_LT "get <con|can1|can2|lin1|lin2|uart1|uart2>" TAG_GT "\r\n"
                            "\tto get the interface settings.\r\n";
    
    const char str_can[] = "Send CAN message. Syntax:\r\n"
                                "\t" TAG_BLUE  "<can1|can2> " TAG_GREEN "<id>" TAG_BLUE " <-s|-e> " TAG_GREEN "<len> <data>" TAG_BLUE " [-n|-a] " TAG_GREEN "[-t time] [-c count]" TAG_DEF "\r\n\r\n"
                                "\t" TAG_BLUE  "can1, can2"  TAG_DEF " - physical interface name\r\n"
                                "\t" TAG_GREEN "id"          TAG_DEF " - hexadecimal identifier of message\r\n"
                                "\t" TAG_BLUE  "-s (-std)"   TAG_DEF " - standard id (11 bytes)\r\n"
                                "\t" TAG_BLUE  "-e (-ext)"   TAG_DEF " - extended id (29 bytes)\r\n"
                                "\t" TAG_GREEN "len"         TAG_DEF " - length of message (should be from 1 to 8)\r\n"
                                "\t" TAG_GREEN "data"        TAG_DEF " - data bytes\r\n"
                                "\t" TAG_BLUE "-n (-nask)"   TAG_DEF " - do not wait for an acknowledge\r\n"
                                "\t" TAG_BLUE  "-a (-ask)"   TAG_DEF " - transmit acknowledge\r\n"
                                "\t" TAG_GREEN "-t (-time)"  TAG_DEF " - message repeat interval, ms\r\n"
                                "\t" TAG_GREEN "-c (-count)" TAG_DEF " - message repeat counter\r\n";

    const char str_lin[] = "Send LIN message. Syntax:\r\n"
                                "\t" TAG_BLUE  "<lin1|lin2> " TAG_GREEN "<id> [-s len] <data> [checksum]" TAG_BLUE " [-v] " TAG_GREEN "[-t time] [-c count]" TAG_DEF "\r\n\r\n"
                                "\t" TAG_BLUE  "lin1, lin2"  TAG_DEF " - physical interface name\r\n"
                                "\t" TAG_GREEN "id"          TAG_DEF " - hexadecimal identifier of message\r\n"
                                "\t" TAG_GREEN "-s (-std)"   TAG_DEF " - standard message format (lin v1.1) and length of message\r\n"
                                    "\t\t(should be 2, 4 or 8)\r\n"
                                "\t" TAG_GREEN "data"        TAG_DEF " - data bytes\r\n"
                                "\t" TAG_GREEN "checksum"    TAG_DEF " - check sum (can be automatically calculate)\r\n"
                                "\t" TAG_BLUE  "-v (-valid)" TAG_DEF " - autocalculation of check sum and parity bits of id\r\n"
                                "\t" TAG_GREEN "-t (-time)"  TAG_DEF " - message repeat interval, ms\r\n"
                                "\t" TAG_GREEN "-c (-count)" TAG_DEF " - message repeat counter\r\n";

    const char str_uart[] = "Send UART message. Syntax:\r\n"
                                "\t" TAG_BLUE  "<urt1|urt2> " TAG_GREEN "<data> [-t time] [-c count]" TAG_DEF "\r\n\r\n"
                                "\t" TAG_BLUE  "urt1, urt2"  TAG_DEF " - physical interface name\r\n"
                                "\t" TAG_GREEN "data"        TAG_DEF " - data bytes\r\n"
                                "\t" TAG_GREEN "-t (-time)"  TAG_DEF " - message repeat interval, ms\r\n"
                                "\t" TAG_GREEN "-c (-count)" TAG_DEF " - message repeat counter\r\n";

    const parse_fsm_steps_t cmd_list[] =
    {
        { "get",            NULL, str_get},     // Получить параметры настройки интерфейса
        { "set", parser_help_set, NULL},     // Настроить интерфейс
        {"can1",            NULL, str_can},  // Отправить сообщение по can1
        {"can2",            NULL, str_can},  // Отправить сообщение по can2
        {"lin1",            NULL, str_lin},  // Отправить сообщение по lin1
        {"lin2",            NULL, str_lin},  // Отправить сообщение по lin2
        {"urt1",            NULL, str_uart}, // Отправить сообщение по uart1
        {"urt2",            NULL, str_uart}, // Отправить сообщение по uart2
    };

    if (str[0] != '\0')
    {
        parser_recursion(&str, cmd_list, countof_arr(cmd_list));
    }
    else
    {
        console_send_string("Enter" TAG_LT "<help|?> <");
                            parser_all_param_to_str(cmd_list, countof_arr(cmd_list));
                            console_send_string(">" TAG_GT "\r\n"
                            "for get help of command or enter" TAG_LT "<info|about>" TAG_GT "to get device information.\r\n");
    }
    
    return false;
}
