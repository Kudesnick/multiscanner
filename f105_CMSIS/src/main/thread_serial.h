/**
 *  @file    thread_serial.h
 *
 *  @brief   
 *  @details 
 *
 *  @author  
 *  @date    
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "thread_iface.h"
#include "msg_types.h"
#include "bsp_serial.h"

/**
 *  @brief   класс потока интерфейса
 *  @details универсальный абстрактный класс потока интерфейса. Предоставляет интерфейс для роутинга сообщений
 */
class thread_serial : public thread_iface
{
    private:
        bsp_serial unit;
    protected:
        virtual bool send_msg(msg_t *msg); ///< Посылаем сообщение непосредственно в периферийный модуль
        virtual bool send_msg_rdy(void); ///< Модуль интерфейса готов для передачи сообщения
    public:
        thread_serial(USART_TypeDef * _unit, iface_name_t _object_name = IFACE_NAME_DEF);
};
