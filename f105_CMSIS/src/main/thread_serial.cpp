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

/// ѕосылаем сообщение непосредственно в периферийный модуль
bool thread_serial::send_msg(msg_t *msg)
{

};

/// ћодуль интерфейса готов дл€ передачи сообщени€
bool thread_serial::send_msg_rdy(void)
{
    return unit.tx_ready();
};

thread_serial::thread_serial(iface_name_t _object_name):
    thread_iface(IFACE_TYPE_UART, _object_name),
#warning изменить механизм, т.к. невозможно выбрать им€ модул€
    unit(LIN1_UNIT, &buf)
{

};

