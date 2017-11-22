/********************************************************************
 * cpp_multiscanner класс основного приложени€
 ********************************************************************/

#pragma once

#include "thread_con.h"
#include "parser.h"

class cpp_multiscanner
{
    private:
        thread_con console;
    public:
        //  онструктор реализован здесь, чтобы при добавлении тредов не править два файла
        cpp_multiscanner(void):
            console(parser_parse)
        {
        };
        void routine(void);
};
