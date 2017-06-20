/********************************************************************
 * Console Высокоуровневые функции консоли
 ********************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "misc.h"

#include "fifo.h"
#include "bsp_con.h"

typedef void(*cmd_func_t)(char *buf, uint8_t size);

typedef const struct
{
    const char *name;
    const void *func;
} console_cmd_t;

extern void console_cmd_help(char *buf, uint8_t size);
extern void console_cmd_get(char *buf, uint8_t size);

// константы команд консоли
console_cmd_t console_cmd_list[] =
{
    {"help", console_cmd_help}, // Получить справку о программе
    {"get" , console_cmd_get},  // Получить параметры настройки интерфейса
    {"set" , NULL}, // Настроить интерфейс
    {"can1", NULL}, // Отправить сообщение по can1
    {"can2", NULL}, // Отправить сообщение по can2
    {"lin1", NULL}, // Отправить сообщение по lin1
    {"lin2", NULL}, // Отправить сообщение по lin2
    {"urt1", NULL}, // Отправить сообщение по uart1
    {"urt2", NULL}, // Отправить сообщение по uart2 
};

// Строки кодов ошибок
const char str_err_bad_cmd[]    = "Error! This command is invalid.\r\n";
const char str_err_null_cmd[]   = "Error! This command is not realised.\r\n";
const char str_err_syntax_cmd[] = "Error! This command syntax is invalid. Print help.\r\n";

// перевод числа в строку
static char * console_uint_to_str(uint32_t num)
{
    static char str[] = "4294967295";// 0xFFFFFFFF
    uint8_t i = strlen(str);
    
    do
    {
        i--;
        str[i] = (num % 10) + '0';
        num /= 10;
    }
    while (num > 0);
    
    return &str[i];
}

// Приведение к нижнему регистру
static void console_lowercase(char *buf, uint8_t size)
{
    for (uint_fast16_t i = 0; i < size; i++)
    {
        if (buf[i] >= 'A' && buf[i] <= 'Z')
        {
            buf[i] += 'a' - 'A';
        }
    }
}

// Возвращает количество лексем
static uint8_t console_get_param_count(char *buf, uint8_t size)
{
    uint8_t result = 0;
    
    for (uint_fast16_t i = 0; i < size; i++)
    {
        if (strlen(&buf[i]) > 0)
        {
            result++;
            i += strlen(&buf[i]);
        }
    }
    
    return result;
}

// Возвращает указатель на лексему под номером num
static char *console_get_param(uint8_t num, char *buf, uint8_t size)
{
    uint8_t count = 0; // текущий номер лексемы
    char *result = NULL;

    for (uint_fast16_t i = 0; i < size && result == NULL; i++)
    {
        if (strlen(&buf[i]) > 0)
        {
            if (count++ == num) result = &buf[i];
            i += strlen(&buf[i]);
        }
    }
    
    return result;
}

// Соответствие параметра шаблону
static bool console_param_cmp(uint8_t num, console_cmd_t *cmd, char *buf, uint8_t size)
{
    char *cmd_str = console_get_param(num, buf, size);

    return (strlen(cmd->name) == strlen(cmd_str)
            && memcmp(cmd->name, cmd_str, strlen(cmd_str)) == 0);
};

// Парсинг команд консоли
static void console_cmd_parser(char *buf, const uint8_t size)
{
    bool bad_cmd = true;
    
    console_lowercase(buf, size);
    
    for (uint8_t i = 0; i < sizeof_arr(console_cmd_list); i++)
    {
        if (console_param_cmp(0, &console_cmd_list[i], buf, size))
        {
            if (console_cmd_list[i].func != NULL)
            {
                ((cmd_func_t)console_cmd_list[i].func)(buf, size);
            }
            else
            {
                bsp_con_send(str_err_null_cmd);
            }
            
            bad_cmd = false;
            break;
        }
    }
    
    if (bad_cmd)
    {
        bsp_con_send(str_err_bad_cmd);
    }
}

void console_init(void)
{
    bsp_con_init(console_cmd_parser);
}

// =========================================================================
// Реализация команды help
// =========================================================================

const char str_help[]      = "Print help [get|set|can1|can2|lin1|lin2|urt1|urt2] for get help on command.\r\n";
const char str_help_get[]  = "Get interface settings. Syntax:\r\n get [con|can1|can2|lin1|lin2|urt1|urt2]\r\n";
#warning реализовать справку для индивидуальных параметров настройки интерфейсов
const char str_help_set[]  = "Set interface settings. Syntax:\r\n set [con|can1|can2|lin1|lin2|urt1|urt2]\r\n Print help set <iface> for get param list on command.\r\n";
const char str_help_can[]  = "Send CAN msg. Syntax:\r\n can[1|2] 0x<id hex num> <data length> <data hex format without whitespace> -n(nask) -r(retransmit) <time> <count (0 - infinity)>.\r\n";
const char str_help_lin[]  = "Send LIN msg. Syntax:\r\n lin[1|2] 0x<id hex num> <data hex format without whitespace> -s(standard checksum) -r(retransmit) <time (ms)> <count (0 - infinity)>.\r\n";
const char str_help_uart[] = "Send UART msg. Syntax:\r\n urt[1|2] <data hex format without whitespace> -r(retransmit) <time> <count (0 - infinity)>.\r\n";

console_cmd_t console_cmd_help_param_list[] =
{
    {"get" , str_help_get},
    {"set" , str_help_set},
    {"can1", str_help_can},
    {"can2", str_help_can},
    {"lin1", str_help_lin},
    {"lin2", str_help_lin},
    {"urt1", str_help_uart},
    {"urt2", str_help_uart},
};

static void console_cmd_help(char *buf, uint8_t size)
{
    bool bad_cmd = true;
    
    if (console_get_param_count(buf, size) > 0)
    {
        for (uint8_t i = 0; i < sizeof_arr(console_cmd_help_param_list); i++)
        {
            if (console_param_cmp(1, &console_cmd_help_param_list[i], buf, size))
            {
                bad_cmd = false;

                bsp_con_send((char *)console_cmd_help_param_list[i].func);
            }
        }
    }
    
    if (bad_cmd)
    {
        bsp_con_send(str_help);
    }
}

// =========================================================================
// Реализация команды get
// =========================================================================

static void console_cmd_get_con(uint8_t *buf, uint8_t size)
{
    bsp_con_send("Interface name: console.\r\n");
    
    bsp_con_send("Baudrate: ");
    bsp_con_send(console_uint_to_str(bsp_con_get_setting()->USART_BaudRate));
    
    bsp_con_send("\r\nParity: ");
    bsp_con_send((bsp_con_get_setting()->USART_Parity == USART_Parity_Even) ? "Even" : 
                 (bsp_con_get_setting()->USART_Parity == USART_Parity_Odd) ? "Odd" : "No");
    
    bsp_con_send("\r\nStop bits: ");
    bsp_con_send((bsp_con_get_setting()->USART_StopBits == USART_StopBits_2) ? "2" :
                 (bsp_con_get_setting()->USART_StopBits == USART_StopBits_0_5) ? "0.5" :
                 (bsp_con_get_setting()->USART_StopBits == USART_StopBits_1_5) ? "1.5" : "1");

    bsp_con_send("\r\n");
}

console_cmd_t console_cmd_get_param_list[] =
{
    {"con",  console_cmd_get_con},
    {"can1", NULL},
    {"can2", NULL},
    {"lin1", NULL},
    {"lin2", NULL},
    {"urt1", NULL},
    {"urt2", NULL},
};

static void console_cmd_get(char *buf, uint8_t size)
{
    bool bad_cmd = true;
    
    if (console_get_param_count(buf, size) > 0)
    {
        for (uint8_t i = 0; i < sizeof_arr(console_cmd_get_param_list); i++)
        {
            if (console_param_cmp(1, &console_cmd_get_param_list[i], buf, size))
            {
                
                if (console_cmd_get_param_list[i].func != NULL)
                {
                    bad_cmd = false;
                    ((cmd_func_t)console_cmd_get_param_list[i].func)(buf, size);
                }

                break;
            }
        }
    }
    
    if (bad_cmd)
    {
        bsp_con_send(str_err_syntax_cmd);
    }
}
