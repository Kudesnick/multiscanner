/**
 *  @file    thread_serial.cpp
 *
 *  @brief   класс потока интерфейса serial
 *  @details 
 *
 *  @author  
 *  @date
 */

#include <stdbool.h>
#include <typeinfo>

#include "rtt.h"
#include "thread_iface.h"
#include "thread_serial.h"

/// Посылаем сообщение непосредственно в периферийный модуль
bool thread_serial::send_msg(msg_t *msg)
{
    bool result = unit.send(msg);

    return result;
};


thread_serial::thread_serial(USART_TypeDef * _unit, iface_name_t _object_name):
    thread_iface(IFACE_TYPE_UART, _object_name),
    unit(_unit, &buf, _object_name)
{
    RTT_CREATE_LOG;
};

