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
    protected:
    public:
        thread_con(void);
        virtual void routine(void);
};

void console_init(void);

#endif /* _THREAD_CON_H_ */
