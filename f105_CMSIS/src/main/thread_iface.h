/**
 *  @file    thread_iface.h
 *
 *  @brief   класс потока интерфейса
 *  @details универсальный абстрактный класс потока интерфейса. Предоставляет интерфейс для роутинга сообщений
 *
 *  @author  
 *  @date    22.11.2017
 */

#pragma once

#include <stdint.h>
#include <string.h>

#include "thread.h"
#include "fifo_buffers.h"
#include "msg_types.h"

/**
 *  @brief   класс потока интерфейса
 *  @details универсальный абстрактный класс потока интерфейса. Предоставляет интерфейс для роутинга сообщений
 */
class thread_iface : public thread
{
    protected:
        fifo_buff buf;
        virtual bool send_msg(msg_t *msg) = NULL; ///< Посылаем сообщение непосредственно в периферийный модуль
        virtual bool send_msg_rdy(void)   = NULL; ///< Модуль интерфейса готов для передачи сообщения
    public:
        thread_iface(iface_type_t _class_type = IFACE_TYPE_DEF, iface_name_t _object_name = IFACE_NAME_DEF);
        msg_t *msg_get(void);
        virtual bool msg_send(msg_t *msg);
        virtual void routine(void);
};
