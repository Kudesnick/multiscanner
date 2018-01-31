/**
 *  @file    thread_iface.cpp
 *
 *  @brief   ����� ������ ����������
 *  @details ������������� ����������� ����� ������ ����������. ������������� ��������� ��� �������� ���������
 *
 *  @author  
 *  @date    22.11.2017
 */

#include <stdbool.h>
#include <typeinfo>

#include "rtt.h"
#include "thread_iface.h"

thread_iface::thread_iface(iface_type_t _class_type, iface_name_t _object_name):
    thread(_class_type, _object_name),
    timestamp(0),
    buf(_class_type, _object_name)
{
    RTT_CREATE_LOG;
}

/**
 *  @brief   �������� ������� �����-������
 *  @details ��������� ����� �� ��������, �������� ���������, ���� ��������� ��������.
 *           ���� ���, �� ����������� �������� �� ������������ � �������� ���������� ������������.
 *           ����� �� ����������� ��������� �������� ��������� �� ��������.
 */
void thread_iface::routine(void)
{
    uint64_t delta_time = timer.get_timestamp() - timestamp;
    timestamp += delta_time;
    
    for(fifo_ptr_t i = buf.tx.get_full_count(); i > 0; i--)
    {
        msg_t msg = buf.tx.extract();
        
        // ���� ������ ��� �� ����� - �������������� ���
        if (msg.counter > delta_time)
        {
            msg.counter -= delta_time;
            buf.tx.add(msg);
        }
        // �������� ���������
        else if (send_msg(&msg))
        {
            // ���� ������� ���������� ������� �� ������� - �������������� ��� � ����� � �������
            if (msg.count != 1)
            {
                if (msg.interval >= (delta_time - msg.counter))
                {
                    msg.counter = msg.interval - (delta_time - msg.counter);
                }
                else
                {
                    msg.counter = 0;
                }
                
                if (msg.count != 0)
                {
                    msg.count--;
                }
                buf.tx.add(msg);
            }
        }
        // ���� �� ����� ��������, �� ���������� ����� � ����� �� � ���������� ��������
        else
        {
            msg.counter = 0;
            buf.tx.add(msg);
        }
    }
}
