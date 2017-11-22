//------------------------------------------------------------------------------
// Класс консоли
//------------------------------------------------------------------------------

#pragma once

#include <stdint.h>
#include <string.h>

#include "thread.h"
#include "bsp_con.h"
#include "fifo_con.h"

class thread_con : public thread
{
    private:
        fifo_con buf;
        bsp_con unit;
        void (* parse)(char * str);
    protected:
    public:
        thread_con(void (* _parse)(char * str) = NULL);
        void set_parser(void (* _parse)(char * str));
        bool send_str(const char * str);
        virtual void routine(void);
};

// Функция - точка входа для парсера.
bool console_send_string(const char * str);
