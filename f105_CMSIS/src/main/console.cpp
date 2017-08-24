/********************************************************************
 * Console ¬ысокоуровневые функции консоли
 ********************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "misc.h"

#include "fifo.h"
#include "fifo_con.h"
#include "bsp_con.h"
#include "units_config.h"

#define PARAM_ERR ((int32_t)-1)

#define countof_arr(a) (sizeof(a)/sizeof(a[0]))

typedef void(*cmd_func_void_t )(char *buf, uint8_t size);
typedef bool(*cmd_func_bool_t )(char *buf, uint8_t size);

typedef int32_t(*cmd_param_int32_t)(char *buf, uint8_t size, uint8_t *param, void * sett);

typedef const struct
{
    const char *name;
    const void *func;
} console_cmd_t;

fifo_con * console_buf;
bsp_con * console_unit;

// —троки кодов ошибок
static const char str_err_bad_cmd[]    = "\x1b[31mError! This command is invalid.\x1b[0m\r\n";
static const char str_err_null_cmd[]   = "\x1b[31mError! This command is not realised.\x1b[0m\r\n";
static const char str_err_syntax_cmd[] = "\x1b[31mError! This command syntax is invalid. Print help.\x1b[0m\r\n";

// перевод числа в строку
static char * console_uint_to_str(uint32_t num)
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

// перевод строки в число
static uint32_t console_str_to_uint(char * str)
{
    uint32_t result = 0;
    
    for (uint8_t i = 0; str[i] != '\0'; i++)
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

// ѕриведение к нижнему регистру
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

// ¬озвращает количество лексем
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

// ¬озвращает указатель на лексему под номером num
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

// —оответствие параметра шаблону
static bool console_param_cmp(uint8_t num, console_cmd_t *cmd, char *buf, uint8_t size)
{
    char *cmd_str = console_get_param(num, buf, size);

    return (strlen(cmd->name) == strlen(cmd_str)
            && memcmp(cmd->name, cmd_str, strlen(cmd_str)) == 0);
}


// ѕарсинг команд консоли
static void console_cmd_help(char *buf, uint8_t size);
static void console_cmd_get(char *buf, uint8_t size);
static void console_cmd_set(char *buf, uint8_t size);

static void console_cmd_parser(char *buf, const uint8_t size)
{
    console_cmd_t cmd_list[] =
    {
        {   "?", (const void *)console_cmd_help},
        {"help", (const void *)console_cmd_help}, // ѕолучить справку о программе
        { "get", (const void *)console_cmd_get},  // ѕолучить параметры настройки интерфейса
        { "set", (const void *)console_cmd_set}, // Ќастроить интерфейс
        {"can1", NULL}, // ќтправить сообщение по can1
        {"can2", NULL}, // ќтправить сообщение по can2
        {"lin1", NULL}, // ќтправить сообщение по lin1
        {"lin2", NULL}, // ќтправить сообщение по lin2
        {"urt1", NULL}, // ќтправить сообщение по uart1
        {"urt2", NULL}, // ќтправить сообщение по uart2 
    };

    bool bad_cmd = true;
    
    console_lowercase(buf, size);
    
    for (uint8_t i = 0; i < countof_arr(cmd_list); i++)
    {
        if (console_param_cmp(0, &cmd_list[i], buf, size))
        {
            if (cmd_list[i].func != NULL)
            {
                ((cmd_func_void_t)cmd_list[i].func)(buf, size);
            }
            else
            {
#warning Ѕл€€€!!!
//                bsp_con_send(str_err_null_cmd);
            }
            
            bad_cmd = false;
            break;
        }
    }
    
    if (bad_cmd)
    {
#warning Ѕл€€€!!!
//        bsp_con_send(str_err_bad_cmd);
    }
}

void console_init(void)
{
    console_buf = new fifo_con();
    console_unit = new bsp_con(CON_UNIT, console_buf);
}

// =========================================================================
// –еализаци€ команды help
// =========================================================================

const char str_help[]      = "Print help [get|set|can1|can2|lin1|lin2|urt1|urt2] for get help on command.\r\n";
const char str_help_get[]  = "Get interface settings. Syntax:\r\n get [con|can1|can2|lin1|lin2|urt1|urt2]\r\n";
#warning реализовать справку дл€ индивидуальных параметров настройки интерфейсов
const char str_help_set[]  = "Set interface settings. Syntax:\r\n set [con|can1|can2|lin1|lin2|urt1|urt2]\r\n Print help set <iface> for get param list on command.\r\n";
const char str_help_can[]  = "Send CAN msg. Syntax:\r\n can[1|2] 0x<id hex num> <data length> <data hex format without whitespace> -n(nask) -r(retransmit) <time> <count (0 - infinity)>.\r\n";
const char str_help_lin[]  = "Send LIN msg. Syntax:\r\n lin[1|2] 0x<id hex num> <data hex format without whitespace> -s(standard checksum) -r(retransmit) <time (ms)> <count (0 - infinity)>.\r\n";
const char str_help_uart[] = "Send UART msg. Syntax:\r\n urt[1|2] <data hex format without whitespace> -r(retransmit) <time> <count (0 - infinity)>.\r\n";

console_cmd_t console_cmd_help_param_list[] =
{
    { "get", (const void *)str_help_get},
    { "set", (const void *)str_help_set},
    {"can1", (const void *)str_help_can},
    {"can2", (const void *)str_help_can},
    {"lin1", (const void *)str_help_lin},
    {"lin2", (const void *)str_help_lin},
    {"urt1", (const void *)str_help_uart},
    {"urt2", (const void *)str_help_uart},
};

static void console_cmd_help(char *buf, uint8_t size)
{
    bool bad_cmd = true;
    
    if (console_get_param_count(buf, size) > 0)
    {
        for (uint8_t i = 0; i < countof_arr(console_cmd_help_param_list); i++)
        {
            if (console_param_cmp(1, &console_cmd_help_param_list[i], buf, size))
            {
                bad_cmd = false;

#warning Ѕл€€€!!!
//                bsp_con_send((char *)console_cmd_help_param_list[i].func);
            }
        }
    }
    
    if (bad_cmd)
    {
#warning Ѕл€€€!!!
//        bsp_con_send(str_help);
    }
}

// =========================================================================
// –еализаци€ команды get
// =========================================================================

static void console_cmd_get_con(uint8_t *buf, uint8_t size)
{
#warning Ѕл€€€!!!
//    bsp_con_send("Interface name: console\r\n");
//    
//    bsp_con_send("    Baudrate: ");
//    bsp_con_send(console_uint_to_str(bsp_con_get_setting()->baudrate));
//    
//    bsp_con_send("\r\n    Parity: ");
//    bsp_con_send((bsp_con_get_setting()->parity == USART_Parity_Even) ? "even" : 
//                 (bsp_con_get_setting()->parity == USART_Parity_Odd) ? "odd" : "no");
//    
//    bsp_con_send("\r\n    Stop bits: ");
//    bsp_con_send((bsp_con_get_setting()->stop_bits == USART_StopBits_2) ? "2" :
//                 (bsp_con_get_setting()->stop_bits == USART_StopBits_0_5) ? "0.5" :
//                 (bsp_con_get_setting()->stop_bits == USART_StopBits_1_5) ? "1.5" : "1");
//
//    bsp_con_send("\r\n");
}

static void console_cmd_get(char *buf, uint8_t size)
{
    console_cmd_t param_list[] =
    {
        { "con", (const void *)console_cmd_get_con},
        {"can1", NULL},
        {"can2", NULL},
        {"lin1", NULL},
        {"lin2", NULL},
        {"urt1", NULL},
        {"urt2", NULL},
    };
    
    bool bad_cmd = true;
    
    if (console_get_param_count(buf, size) > 0)
    {
        for (uint8_t i = 0; i < countof_arr(param_list); i++)
        {
            if (console_param_cmp(1, &param_list[i], buf, size))
            {
                
                if (param_list[i].func != NULL)
                {
                    bad_cmd = false;
                    ((cmd_func_void_t)param_list[i].func)(buf, size);
                }

                break;
            }
        }
    }
    
    if (bad_cmd)
    {
#warning Ѕл€€€!!!
//        bsp_con_send(str_err_syntax_cmd);
    }
}

// =========================================================================
// –еализаци€ команды set
// =========================================================================

int32_t console_cmd_set_con_baud(char *buf, uint8_t size, uint8_t *param, void * sett)
{
    if (console_get_param_count(buf, size) < *param + 2)
    {
        return PARAM_ERR;
    }
    
    (*param)++;
    uint32_t baud = console_str_to_uint(console_get_param(*param, buf, size));
    if (baud > 0)
    {
        ((bsp_con_config_t*)sett)->baudrate = baud;
    }
    else
    {
        return PARAM_ERR;
    }

    return baud;
}

int32_t console_cmd_set_con_even(char *buf, uint8_t size, uint8_t *param, void * sett)
{
    ((bsp_con_config_t*)sett)->parity = USART_Parity_Even;
    
    return USART_Parity_Even;
}

int32_t console_cmd_set_con_odd(char *buf, uint8_t size, uint8_t *param, void * sett)
{
    ((bsp_con_config_t*)sett)->parity = USART_Parity_Odd;
    
    return USART_Parity_Odd;
}

int32_t console_cmd_set_con_none(char *buf, uint8_t size, uint8_t *param, void * sett)
{
    ((bsp_con_config_t*)sett)->parity = USART_Parity_No;
    
    return USART_Parity_No;
}

static bool console_cmd_set_con(char *buf, uint8_t size)
{
    console_cmd_t param_list[] =
    {
        {   "-b", (const void *)console_cmd_set_con_baud},
        {"-baud", (const void *)console_cmd_set_con_baud},
        {"-even", (const void *)console_cmd_set_con_even},
        {  "-ev", (const void *)console_cmd_set_con_even},
        { "-odd", (const void *)console_cmd_set_con_odd},
        {  "-od", (const void *)console_cmd_set_con_odd},
        {"-none", (const void *)console_cmd_set_con_none},
        {  "-no", (const void *)console_cmd_set_con_none},
        {"-stop", NULL},
        {   "-s", NULL},
        {"-echo", NULL},
    };

    uint8_t lim = console_get_param_count(buf, size);
    
    if (lim < 3) return false;

    for (uint8_t i = 2; i < lim; i++)
    {
        for (uint8_t j = 0; j < countof_arr(param_list); j++)
        {
            if (console_param_cmp(i, &param_list[j], buf, size))
            {
                if (param_list[i].func != NULL)
                {
#warning Ѕл€€€!!!
//                    if (((cmd_param_int32_t)param_list[j].func)(buf, size, &i, (void *)bsp_con_get_setting()) == PARAM_ERR)
                    {
                        return false;
                    }
                }
                break;
            }
        }
    }
    
    console_init();
    
    return true;
}


static void console_cmd_set(char *buf, uint8_t size)
{
    console_cmd_t param_list[] =
    {
        { "con", (const void *)console_cmd_set_con},
        {"can1", NULL},
        {"can2", NULL},
        {"lin1", NULL},
        {"lin2", NULL},
        {"urt1", NULL},
        {"urt2", NULL},
    };

    bool bad_cmd = true;
    
    if (console_get_param_count(buf, size) > 0)
    {
        for (uint8_t i = 0; i < countof_arr(param_list); i++)
        {
            if (console_param_cmp(1, &param_list[i], buf, size))
            {
                if (param_list[i].func != NULL)
                {
                    bad_cmd = !((cmd_func_bool_t)param_list[i].func)(buf, size);
                }
                break;
            }
        }
    }
    
    if (bad_cmd)
    {
#warning Ѕл€€€!!!
//        bsp_con_send(str_err_syntax_cmd);
    }
}
