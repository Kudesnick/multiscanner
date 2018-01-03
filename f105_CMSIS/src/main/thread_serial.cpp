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

#include "thread_iface.h"
#include "thread_serial.h"

/// Посылаем сообщение непосредственно в периферийный модуль
bool thread_serial::send_msg(msg_t *msg)
{
    bool result = false;
#warning реализовать функцию.
    return result;
};

/// Модуль интерфейса готов для передачи сообщения
bool thread_serial::send_msg_rdy(void)
{
#warning пока заглушка, но надо разрулить
    return true;
};

thread_serial::thread_serial(USART_TypeDef * _unit, iface_name_t _object_name):
    thread_iface(IFACE_TYPE_UART, _object_name),
    unit(_unit, &buf, _object_name)
{

};

