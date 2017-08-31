/********************************************************************
 * cpp_multiscanner класс основного приложения
 ********************************************************************/

#include "multiscanner.h"
#include "console.h"

cpp_multiscanner::cpp_multiscanner(void):
    console()
{
};

void cpp_multiscanner::routine(void)
{
    while(1)
    {
        console.routine();
    }
};
