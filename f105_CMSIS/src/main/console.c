/********************************************************************
 * Console ��������������� ������� �������
 ********************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "misc.h"

#include "fifo.h"
#include "bsp_con.h"

#define PARAM_ERR ((int32_t)-1)

typedef void(*cmd_func_void_t )(char *buf, uint8_t size);
typedef bool(*cmd_func_bool_t )(char *buf, uint8_t size);

typedef int32_t(*cmd_param_int32_t)(char *buf, uint8_t size, uint8_t *param, void * sett);

typedef const struct
{
    const char *name;
    const void *func;
} console_cmd_t;

extern void console_cmd_help(char *buf, uint8_t size);
extern void console_cmd_get(char *buf, uint8_t size);

// ��������� ������ �������
console_cmd_t console_cmd_list[] =
{
    {"help", console_cmd_help}, // �������� ������� � ���������
    { "get", console_cmd_get},  // �������� ��������� ��������� ����������
    { "set", NULL}, // ��������� ���������
    {"can1", NULL}, // ��������� ��������� �� can1
    {"can2", NULL}, // ��������� ��������� �� can2
    {"lin1", NULL}, // ��������� ��������� �� lin1
    {"lin2", NULL}, // ��������� ��������� �� lin2
    {"urt1", NULL}, // ��������� ��������� �� uart1
    {"urt2", NULL}, // ��������� ��������� �� uart2 
};

// ������ ����� ������
const char str_err_bad_cmd[]    = "Error! This command is invalid.\r\n";
const char str_err_null_cmd[]   = "Error! This command is not realised.\r\n";
const char str_err_syntax_cmd[] = "Error! This command syntax is invalid. Print help.\r\n";

// ������� ����� � ������
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

// ���������� � ������� ��������
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

// ���������� ���������� ������
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

// ���������� ��������� �� ������� ��� ������� num
static char *console_get_param(uint8_t num, char *buf, uint8_t size)
{
    uint8_t count = 0; // ������� ����� �������
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

// ������������ ��������� �������
static bool console_param_cmp(uint8_t num, console_cmd_t *cmd, char *buf, uint8_t size)
{
    char *cmd_str = console_get_param(num, buf, size);

    return (strlen(cmd->name) == strlen(cmd_str)
            && memcmp(cmd->name, cmd_str, strlen(cmd_str)) == 0);
};

// ������� ������ �������
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
                ((cmd_func_void_t)console_cmd_list[i].func)(buf, size);
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
// ���������� ������� help
// =========================================================================

const char str_help[]      = "Print help [get|set|can1|can2|lin1|lin2|urt1|urt2] for get help on command.\r\n";
const char str_help_get[]  = "Get interface settings. Syntax:\r\n get [con|can1|can2|lin1|lin2|urt1|urt2]\r\n";
#warning ����������� ������� ��� �������������� ���������� ��������� �����������
const char str_help_set[]  = "Set interface settings. Syntax:\r\n set [con|can1|can2|lin1|lin2|urt1|urt2]\r\n Print help set <iface> for get param list on command.\r\n";
const char str_help_can[]  = "Send CAN msg. Syntax:\r\n can[1|2] 0x<id hex num> <data length> <data hex format without whitespace> -n(nask) -r(retransmit) <time> <count (0 - infinity)>.\r\n";
const char str_help_lin[]  = "Send LIN msg. Syntax:\r\n lin[1|2] 0x<id hex num> <data hex format without whitespace> -s(standard checksum) -r(retransmit) <time (ms)> <count (0 - infinity)>.\r\n";
const char str_help_uart[] = "Send UART msg. Syntax:\r\n urt[1|2] <data hex format without whitespace> -r(retransmit) <time> <count (0 - infinity)>.\r\n";

console_cmd_t console_cmd_help_param_list[] =
{
    { "get", str_help_get},
    { "set", str_help_set},
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
// ���������� ������� get
// =========================================================================

static void console_cmd_get_con(uint8_t *buf, uint8_t size)
{
    bsp_con_send("Interface name: console\r\n");
    
    bsp_con_send("    Baudrate: ");
    bsp_con_send(console_uint_to_str(bsp_con_get_setting()->baudrate));
    
    bsp_con_send("\r\n    Parity: ");
    bsp_con_send((bsp_con_get_setting()->parity == USART_Parity_Even) ? "even" : 
                 (bsp_con_get_setting()->parity == USART_Parity_Odd) ? "odd" : "no");
    
    bsp_con_send("\r\n    Stop bits: ");
    bsp_con_send((bsp_con_get_setting()->stop_bits == USART_StopBits_2) ? "2" :
                 (bsp_con_get_setting()->stop_bits == USART_StopBits_0_5) ? "0.5" :
                 (bsp_con_get_setting()->stop_bits == USART_StopBits_1_5) ? "1.5" : "1");

    bsp_con_send("\r\n");
}

static void console_cmd_get(char *buf, uint8_t size)
{
    console_cmd_t param_list[] =
    {
        { "con", console_cmd_get_con},
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
        for (uint8_t i = 0; i < sizeof_arr(param_list); i++)
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
        bsp_con_send(str_err_syntax_cmd);
    }
}

// =========================================================================
// ���������� ������� set
// =========================================================================

static bool console_cmd_set_con(char *buf, uint8_t size)
{
    console_cmd_t param_list[] =
    {
        {   "-b", NULL},
        {"-baud", NULL},
        {"-even", NULL},
        {  "-ev", NULL},
        { "-odd", NULL},
        {  "-od", NULL},
        {"-none", NULL},
        {  "-no", NULL},
        {"-stop", NULL},
        {   "-s", NULL},
        {"-echo", NULL},
    };

    bool bad_cmd = false;
    
    uint8_t lim = console_get_param_count(buf, size);
    
    bsp_con_config_t con_init_struct_default = *bsp_con_get_setting();
    
    if (lim < 3)
    {
        bad_cmd = true;
    }
    else
    {
        for (uint8_t i = 2; i < lim; i++)
        {
            for (uint8_t j = 0; j < sizeof_arr(param_list); j++)
            {
                if (console_param_cmp(i, &param_list[j], buf, size))
                {
                    if (param_list[i].func != NULL)
                    {
                        bad_cmd = (((cmd_param_int32_t)param_list[i].func)(buf, size, &i, (void *)bsp_con_get_setting()) == PARAM_ERR);
                    }
                    break;
                }
            }
            if (bad_cmd == true)
            {
                break;
            }
        }
    }
    
    return bad_cmd;
}


static void console_cmd_set(char *buf, uint8_t size)
{
    console_cmd_t param_list[] =
    {
        { "con", console_cmd_set_con},
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
        for (uint8_t i = 0; i < sizeof_arr(param_list); i++)
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
        bsp_con_send(str_err_syntax_cmd);
    }
}
