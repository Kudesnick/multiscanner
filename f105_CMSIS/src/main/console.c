/********************************************************************
 * Console ��������������� ������� �������
 ********************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "fifo.h"
#include "bsp_con.h"

#define is_delim(s) ((bool)(s == ' ' || s == 0x0A || s == 0x0D))

typedef void(cmd_func_t)(uint8_t *buf, uint8_t size);

typedef const struct
{
    const uint8_t *name;
    const uint_fast8_t size; // ����� ����� ���������� ����������� �������. ���� �� ��������, ��� � ���� ������
    cmd_func_t *const func;
} console_cmd_t;

// ��������� ������ �������
console_cmd_t console_cmd_list[] =
{
    {(const uint8_t *)"help", 4, NULL}, // �������� ������� � ���������
    {(const uint8_t *)"get" , 3, NULL}, // �������� ��������� ��������� ����������
    {(const uint8_t *)"set" , 3, NULL}, // ��������� ���������
    {(const uint8_t *)"can1", 4, NULL}, // ��������� ��������� �� can1
    {(const uint8_t *)"can2", 4, NULL}, // ��������� ��������� �� can2
    {(const uint8_t *)"lin1", 4, NULL}, // ��������� ��������� �� lin1
    {(const uint8_t *)"lin2", 4, NULL}, // ��������� ��������� �� lin2
    {(const uint8_t *)"urt1", 4, NULL}, // ��������� ��������� �� uart1
    {(const uint8_t *)"urt2", 4, NULL}, // ��������� ��������� �� uart2 
};

// ���������� � ������� ��������
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

// ���������� ���������� ������
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

// ���������� ������ � ��������� �� ������� ��� ������� num
static uint8_t console_get_param(uint8_t num, uint8_t *buf, uint8_t size, uint8_t **param_ptr)
{
    uint8_t count = 0; // ������� ����� �������
    uint8_t param_size = 0; // ������ �������
    bool start_count = false; // ���� ����, ��� ������ ������� - �� ������
    
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

// ������� ������ �������
static void console_cmd_parser(uint8_t *buf, uint8_t size)
{
    console_lowercase(buf, size);
    if (console_get_param_count(buf, size) > 0)
    {
        uint8_t *cmd_ptr;
        uint8_t cmd_size = console_get_param(0, buf, size, &cmd_ptr);
        for (uint8_t i = 0; i < sizeof_arr(console_cmd_list); i++)
        {
            if (console_cmd_list[i].size == cmd_size
                && memcmp(buf, console_cmd_list[i].name, cmd_size) == 0
                && console_cmd_list[i].func != NULL
                )
            {
                console_cmd_list[i].func(buf, size);
            }
        }
    }
}

void console_init(void)
{
    bsp_con_init(console_cmd_parser);
}
