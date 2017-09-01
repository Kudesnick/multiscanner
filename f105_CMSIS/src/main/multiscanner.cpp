/********************************************************************
 * cpp_multiscanner класс основного приложения
 ********************************************************************/

#include "multiscanner.h"
#include "thread_con.h"

void cpp_multiscanner::routine(void)
{
    while(1)
    {
        static thread * thread_ptr = NULL;
        thread_ptr = (thread *)thread::get_last_pointer();
        
        while (thread_ptr != NULL)
        {
            thread_ptr->routine();
            thread_ptr = (thread *)thread_ptr->get_prev_pointer();
        }
    }
};
