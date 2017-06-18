/********************************************************************
 * Console Высокоуровневые функции консоли
 ********************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "misc.h"

#include "fifo.h"
#include "bsp_con.h"

#define is_delim(s) ((bool)(s == ' ' || s == 0x0A || s == 0x0D))
#define send_str(s) bsp_con_send(s, strlen((const char *)s))

typedef void(*cmd_func_t)(uint8_t *buf, uint8_t size);

typedef const struct
{
    const uint8_t *name;
    const uint_fast8_t size; // Длину строк приходится проставлять вручную. Пока не придумал, что с этим делать
    const void *func;
} console_cmd_t;

extern void console_cmd_help(uint8_t *buf, uint8_t size);
extern void console_cmd_get(uint8_t *buf, uint8_t size);

// константы команд консоли
console_cmd_t console_cmd_list[] =
{
    {(const uint8_t *)"help", 4, console_cmd_help}, // Получить справку о программе
    {(const uint8_t *)"get" , 3, console_cmd_get},  // Получить параметры настройки интерфейса
    {(const uint8_t *)"set" , 3, NULL}, // Настроить интерфейс
    {(const uint8_t *)"can1", 4, NULL}, // Отправить сообщение по can1
    {(const uint8_t *)"can2", 4, NULL}, // Отправить сообщение по can2
    {(const uint8_t *)"lin1", 4, NULL}, // Отправить сообщение по lin1
    {(const uint8_t *)"lin2", 4, NULL}, // Отправить сообщение по lin2
    {(const uint8_t *)"urt1", 4, NULL}, // Отправить сообщение по uart1
    {(const uint8_t *)"urt2", 4, NULL}, // Отправить сообщение по uart2 
};

// Строки кодов ошибок
const uint8_t str_err_bad_cmd[] = "Error! This command is invalid.\r\n";
const uint8_t str_err_null_cmd[] = "Error! This command is not realised.\r\n";
const uint8_t str_err_syntax_cmd[] = "Error! This command syntax is invalid. Print help.\r\n";

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
static void console_lowercase(uint8_t *buf, uint8_t size)
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
static uint8_t console_get_param_count(uint8_t *buf, uint8_t size)
{
    uint8_t result = 0;
    bool start_count = false;
    
    for (uint_fast16_t i = 0; i < size; i++)
    {
        if (is_delim(buf[i]))
        {
            start_count = false;
        }
        else if (start_count == false)
        {
            result++;
            start_count = true;
        }
    }
    
    return result;
}

// Возвращает размер и указатель на лексему под номером num
static uint8_t console_get_param(uint8_t num, uint8_t *buf, uint8_t size, uint8_t **param_ptr)
{
    uint8_t count = 0; // текущий номер лексемы
    uint8_t param_size = 0; // Размер лексемы
    bool start_count = false; // Флаг того, что символ лексемы - не первый
    
    num++;
    
    for (uint_fast16_t i = 0; i < size; i++)
    {
        if (is_delim(buf[i]))
        {
            start_count = false;
            if (count == num)
            {
                break;
            }
        }
        else
        {
            if (start_count == false)
            {
                count++;
                start_count = true;
                if (count == num)
                {
                    *param_ptr = &buf[i];
                }
            }
            if (count == num)
            {
                param_size++;
            }
        }
    }
    
    if (param_size == 0)
    {
        *param_ptr = NULL;
    }
    
    return param_size;
}

// Соответствие параметра шаблону
static bool console_param_cmp(console_cmd_t *cmd, uint8_t *buf, uint8_t size, uint8_t num)
{
    uint8_t *cmd_ptr;
    uint8_t cmd_size = console_get_param(num, buf, size, &cmd_ptr);

    return (cmd->size == cmd_size
            && memcmp(cmd_ptr, cmd->name, cmd_size) == 0);
};

// Парсинг команд консоли
static void console_cmd_parser(uint8_t *buf, uint8_t size)
{
    bool bad_cmd = true;
    
    console_lowercase(buf, size);
    
    for (uint8_t i = 0; i < sizeof_arr(console_cmd_list); i++)
    {
        if (console_param_cmp(&console_cmd_list[i], buf, size, 0))
        {
            if (console_cmd_list[i].func != NULL)
            {
                ((cmd_func_t)console_cmd_list[i].func)(buf, size);
            }
            else
            {
                send_str(str_err_null_cmd);
            }
            
            bad_cmd = false;
            break;
        }
    }
    
    if (bad_cmd)
    {
        send_str(str_err_bad_cmd);
    }
}

void console_init(void)
{
    bsp_con_init(console_cmd_parser);
}

// =========================================================================
// Реализация команды help
// =========================================================================

const uint8_t str_help[]      = "Print help [get|set|can1|can2|lin1|lin2|urt1|urt2] for get help on command.\r\n";
const uint8_t str_help_get[]  = "Get interface settings. Syntax:\r\n get [con|can1|can2|lin1|lin2|urt1|urt2]\r\n";
#warning реализовать справку для индивидуальных параметров настройки интерфейсов
const uint8_t str_help_set[]  = "Set interface settings. Syntax:\r\n set [con|can1|can2|lin1|lin2|urt1|urt2]\r\n Print help set <iface> for get param list on command.\r\n";
const uint8_t str_help_can[]  = "Send CAN msg. Syntax:\r\n can[1|2] 0x<id hex num> <data length> <data hex format without whitespace> -n(nask) -r(retransmit) <time> <count (0 - infinity)>.\r\n";
const uint8_t str_help_lin[]  = "Send LIN msg. Syntax:\r\n lin[1|2] 0x<id hex num> <data hex format without whitespace> -s(standard checksum) -r(retransmit) <time (ms)> <count (0 - infinity)>.\r\n";
const uint8_t str_help_uart[] = "Send UART msg. Syntax:\r\n urt[1|2] <data hex format without whitespace> -r(retransmit) <time> <count (0 - infinity)>.\r\n";

console_cmd_t console_cmd_help_param_list[] =
{
    {(const uint8_t *)"get" , 3, (void*)str_help_get},
    {(const uint8_t *)"set" , 3, (void*)str_help_set},
    {(const uint8_t *)"can1", 4, (void*)str_help_can},
    {(const uint8_t *)"can2", 4, (void*)str_help_can},
    {(const uint8_t *)"lin1", 4, (void*)str_help_lin},
    {(const uint8_t *)"lin2", 4, (void*)str_help_lin},
    {(const uint8_t *)"urt1", 4, (void*)str_help_uart},
    {(const uint8_t *)"urt2", 4, (void*)str_help_uart},
};

static void console_cmd_help(uint8_t *buf, uint8_t size)
{
    bool bad_cmd = true;
    
    if (console_get_param_count(buf, size) > 0)
    {
        for (uint8_t i = 0; i < sizeof_arr(console_cmd_help_param_list); i++)
        {
            if (console_param_cmp(&console_cmd_help_param_list[i], buf, size, 1))
            {
                bad_cmd = false;

                send_str((uint8_t *)console_cmd_help_param_list[i].func);
            }
        }
    }
    
    if (bad_cmd)
    {
        send_str(str_help);
    }
}

// =========================================================================
// Реализация команды get
// =========================================================================

static void console_cmd_get_con(uint8_t *buf, uint8_t size)
{
    send_str((uint8_t *)"Interface name: console.\r\n");
    
    send_str((uint8_t *)"Baudrate: ");
    send_str((uint8_t *)console_uint_to_str(bsp_con_get_setting()->USART_BaudRate));
    
    send_str((uint8_t *)"\r\nParity: ");
    send_str((uint8_t *) ((bsp_con_get_setting()->USART_Parity == USART_Parity_Even) ? "Even" : 
                          (bsp_con_get_setting()->USART_Parity == USART_Parity_Odd) ? "Odd" : "No"));
    
    send_str((uint8_t *)"\r\nStop bits: ");
    send_str((uint8_t *) ((bsp_con_get_setting()->USART_StopBits == USART_StopBits_2) ? "2" :
                          (bsp_con_get_setting()->USART_StopBits == USART_StopBits_0_5) ? "0.5" :
                          (bsp_con_get_setting()->USART_StopBits == USART_StopBits_1_5) ? "1.5" : "1"));

    send_str((uint8_t *)"\r\n");
}

console_cmd_t console_cmd_get_param_list[] =
{
    {(const uint8_t *)"con",  3, console_cmd_get_con},
    {(const uint8_t *)"can1", 4, NULL},
    {(const uint8_t *)"can2", 4, NULL},
    {(const uint8_t *)"lin1", 4, NULL},
    {(const uint8_t *)"lin2", 4, NULL},
    {(const uint8_t *)"urt1", 4, NULL},
    {(const uint8_t *)"urt2", 4, NULL},
};

static void console_cmd_get(uint8_t *buf, uint8_t size)
{
    bool bad_cmd = true;
    
    if (console_get_param_count(buf, size) > 0)
    {
        for (uint8_t i = 0; i < sizeof_arr(console_cmd_get_param_list); i++)
        {
            if (console_param_cmp(&console_cmd_get_param_list[i], buf, size, 1))
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
        send_str(str_err_syntax_cmd);
    }
}
