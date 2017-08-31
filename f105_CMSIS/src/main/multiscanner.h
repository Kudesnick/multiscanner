/********************************************************************
 * cpp_multiscanner класс основного приложения
 ********************************************************************/

#ifndef _MULTISCANNER_
#define _MULTISCANNER_

#include "thread_con.h"

class cpp_multiscanner
{
    private:
        thread_con console;
    public:
        cpp_multiscanner(void);
        void routine(void);
};

#endif /* _MULTISCANNER_ */
