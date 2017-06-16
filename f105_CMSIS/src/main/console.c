/********************************************************************
 * Console ��������������� ������� �������
 ********************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "bsp_con.h"

typedef const struct
{
    const uint8_t *cmd_name;
    const void(*cmd_func)(uint8_t *buf, uint8_t size);
} console_cmd_t;

// ��������� ������ �������
console_cmd_t console_cmd_list[] =
{
    {(uint8_t *)"help", NULL}, // �������� ������� � ���������
    {(uint8_t *)"get" , NULL}, // �������� ��������� ��������� ����������
    {(uint8_t *)"set" , NULL}, // ��������� ���������
    {(uint8_t *)"can1", NULL}, // ��������� ��������� �� can1
    {(uint8_t *)"can2", NULL}, // ��������� ��������� �� can2
    {(uint8_t *)"lin1", NULL}, // ��������� ��������� �� lin1
    {(uint8_t *)"lin2", NULL}, // ��������� ��������� �� lin2
    {(uint8_t *)"urt1", NULL}, // ��������� ��������� �� uart1
    {(uint8_t *)"urt2", NULL}, // ��������� ��������� �� uart2 
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
        if (buf[i] == ' ' || buf[i] == 0x0A || buf[i] == 0x0D)
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
static uint8_t console_get_param(uint8_t num, uint8_t *buf, uint8_t *param_ptr)
{
    uint8_t count = 0; // ������� ����� �������
    uint8_t size = 0; // ������ �������
    bool start_count = false; // ���� ����, ��� ������ ������� - �� ������
    
    for (uint_fast16_t i = 0; i < size; i++)
    {
        if (buf[i] == ' ' || buf[i] == 0x0A || buf[i] == 0x0D)
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
                    param_ptr = &buf[i];
                }
            }
            if (count == num)
            {
                size++;
            }
        }
    }
    
    if (size == 0)
    {
        param_ptr = NULL;
    }
    
    return size;
}

// ������� ������ �������
static void console_cmd_parser(uint8_t *buf, uint8_t size)
{
    console_lowercase(buf, size);
    if (console_get_param_count(buf, size) > 0)
    {
        uint8_t *cmd_ptr;
        uint8_t cmd_size = console_get_param(0, buf, cmd_ptr);
        for (uint8_t i = 0; i < sizeof(console_cmd_list)/sizeof(console_cmd_list[0]); i++)
        {
            if (sizeof(*console_cmd_list[i].cmd_name) == cmd_size
                && memcmp(buf, console_cmd_list[i].cmd_name, cmd_size) == 0
                && console_cmd_list[i].cmd_func != NULL
                )
            {
                console_cmd_list[i].cmd_func(buf, size);
            }
        }
    }
}

void console_init(void)
{
    bsp_con_init(console_cmd_parser);
}
