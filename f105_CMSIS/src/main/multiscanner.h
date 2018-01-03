/********************************************************************
 * cpp_multiscanner класс основного приложения
 ********************************************************************/

#pragma once

#include "thread_con.h"
#include "parser.h"

class cpp_multiscanner
{
    private:
        cpp_multiscanner(void);
    public:
        static int routine(void);
};
