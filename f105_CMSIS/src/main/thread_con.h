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
        void (* parse)(char * str, bsp_con_config_t * console_sett);
    protected:
    public:
        thread_con(void (* _parse)(char * str, bsp_con_config_t * console_sett) = NULL);
        void set_parser(void (* _parse)(char * str, bsp_con_config_t * console_sett));
        bool send_str(const char * str);
        virtual void routine(void);
};

// Функция - точка входа для парсера.
bool console_send_string(const char * str);

#endif /* _THREAD_CON_H_ */
