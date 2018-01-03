/**
 *  @file    thread_serial.cpp
 *
 *  @brief   ����� ������ ���������� serial
 *  @details 
 *
 *  @author  
 *  @date
 */

#include <stdbool.h>

#include "thread_iface.h"
#include "thread_serial.h"

/// �������� ��������� ��������������� � ������������ ������
bool thread_serial::send_msg(msg_t *msg)
{
    bool result = false;
#warning ����������� �������.
    return result;
};

/// ������ ���������� ����� ��� �������� ���������
bool thread_serial::send_msg_rdy(void)
{
#warning ���� ��������, �� ���� ���������
    return true;
};

thread_serial::thread_serial(USART_TypeDef * _unit, iface_name_t _object_name):
    thread_iface(IFACE_TYPE_UART, _object_name),
    unit(_unit, &buf, _object_name)
{

};
