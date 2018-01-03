//------------------------------------------------------------------------------
// Класс консоли
//------------------------------------------------------------------------------

#pragma once

#include <stdint.h>
#include <string.h>

#include "thread_iface.h"
#include "bsp_con.h"
#include "fifo_con.h"

class thread_con : public thread_iface
{
    private:
        fifo_con con_buf;
        bsp_con unit;
        void (* parse)(char * str);
    protected:
        virtual bool send_msg(msg_t *msg); ///< Посылаем сообщение непосредственно в периферийный модуль
        virtual bool send_msg_rdy(void);   ///< Модуль интерфейса готов для передачи сообщения
    public:
        thread_con(void (* _parse)(char * str) = NULL);
        void set_parser(void (* _parse)(char * str));
        bool send_str(const char * str);
        virtual void routine(void);
};

// Функция - точка входа для парсера.
bool console_send_string(const char * str);
