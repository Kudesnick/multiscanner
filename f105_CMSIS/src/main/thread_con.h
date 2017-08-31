#ifndef _THREAD_CON_H_
#define _THREAD_CON_H_

//------------------------------------------------------------------------------
// Класс консоли
//------------------------------------------------------------------------------

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
        bool (* parse)(char * str);
    protected:
    public:
        thread_con(bool (* _parse)(char * str) = NULL);
        void set_parser(bool (* _parse)(char * str));
        void send_str(const char * str);
        virtual void routine(void);
};

// Функция - точка входа для парсера.
bool console_send_string(const char * str);

#endif /* _THREAD_CON_H_ */
