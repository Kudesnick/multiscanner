/**
 *  @file    thread.h
 *
 *  @brief   ����������� ����� ������
 *  @details ������ ������������ ����� ���������, ����������� � ������ ������������� ���������������
 *           ��� ����������� ������ ����������� �� ������� � �������� �����
 *
 *  @author  
 *  @date    
 */

#include <typeinfo>

#include "rtt.h"
#include "list.h"
#include "thread.h"

/// ������ ������� ������ ������� � ���������� 1 ��.
bsp_clk thread::timer;

thread::thread(iface_type_t _class_type, iface_name_t _object_name):
    cpp_list(_class_type, _object_name)
{
    RTT_CREATE_LOG;
};
