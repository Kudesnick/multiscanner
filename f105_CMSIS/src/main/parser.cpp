//------------------------------------------------------------------------------
// Общие команды и типы для парсера строк
//------------------------------------------------------------------------------

#include <stdint.h>
#include <string.h>

#include "parser.h"

char * parser_uint_to_str(uint32_t num)
{
    static char str[] = "4294967295";// 0xFFFFFFFF
    uint8_t i = (uint8_t)strlen(str);
    
    do
    {
        i--;
        str[i] = (num % 10) + '0';
        num /= 10;
    }
    while (num > 0);
    
    return &str[i];
}

uint32_t parser_str_to_uint(char * str)
{
    uint32_t result = 0;
    
    for (uint8_t i = 0; strchr(" \0", str[i]) == NULL; i++)
    {
        result *= 10;
        if (str[i] < '0' || str[i] > '9')
        {
            str = NULL;
            result = NULL;
            break;
        }
        result += str[i] - '0';
    }
    
    return result;
}

void parser_lowercase(char *buf)
{
    for (uint16_t i = 0; buf[i] != '\0'; i++)
    {
        if (buf[i] >= 'A' && buf[i] <= 'Z')
        {
            buf[i] += 'a' - 'A';
        }
    }
}

int16_t parser_find(char * str, const fsm_steps_t * cmd_list, uint16_t cmd_list_len)
{
    int16_t result = -1;
    
    if (str[0] != '\0')
    {
        char * next_str;
        if ((next_str = strchr(str, ' ')) == NULL) next_str = strchr(str, '\0');
        uint8_t len = (next_str - str) / sizeof(char);
        
        for (uint8_t i = 0; i < cmd_list_len)
        {
            if (strlen(cmd_list[i].name) == len
                && strncmp(str, strlen(cmd_list[i].name, len) == 0)
            {
                result = i;
                str = next_str[0];
                if (str[0] == ' ') str += sizeof(char);
                break;
            }
        }
    }
    
    return result;
}

void parser_parse(char * str, void * param)
{
    const fsm_steps_t cmd_list[] =
    {
        {   "?", console_cmd_help, NULL},
        {"help", console_cmd_help, NULL}, // Получить справку о программе
        { "get",  console_cmd_get, NULL}, // Получить параметры настройки интерфейса
        { "set",  console_cmd_set, NULL}, // Настроить интерфейс
        {"can1",             NULL, NULL}, // Отправить сообщение по can1
        {"can2",             NULL, NULL}, // Отправить сообщение по can2
        {"lin1",             NULL, NULL}, // Отправить сообщение по lin1
        {"lin2",             NULL, NULL}, // Отправить сообщение по lin2
        {"urt1",             NULL, NULL}, // Отправить сообщение по uart1
        {"urt2",             NULL, NULL}, // Отправить сообщение по uart2 
    };
    
    if ((int16_t i = parser_find(str, cmd_list, countof_arr(cmd_list))) < 0)
    {
        send_str(parser_str_err_bad_cmd);
    }
    else if (cmd_list[i].func == NULL)
    {
        send_str(parser_str_err_null_cmd);
    }
    else
    {
        cmd_list[i].func(str, cmd_list[i].param);
    }
}
