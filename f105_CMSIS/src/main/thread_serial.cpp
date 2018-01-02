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

};

/// ������ ���������� ����� ��� �������� ���������
bool thread_serial::send_msg_rdy(void)
{
    return unit.tx_ready();
};

thread_serial::thread_serial(iface_name_t _object_name):
    thread_iface(IFACE_TYPE_UART, _object_name),
#warning �������� ��������, �.�. ���������� ������� ��� ������
    unit(LIN1_UNIT, &buf)
{

};

