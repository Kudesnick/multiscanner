/********************************************************************
 * cpp_multiscanner ����� ��������� ����������
 ********************************************************************/

#include "multiscanner.h"
#include "thread_con.h"
#include "units_config.h"

cpp_multiscanner::cpp_multiscanner(void)
{
    // ������ ����������� ����� ��� ����������� ������ c ������������� static
    static thread_con console(parser_parse);
    #if (LIN1_ENABLED == 1)

    #endif
};

int cpp_multiscanner::routine(void)
{
    static cpp_multiscanner multiscanner_singletone;
    
    int result = 0;
    
    while(result == 0)
    {
        static thread * thread_ptr = NULL;
        thread_ptr = (thread *)thread::get_last_pointer();
        
        if (thread_ptr == NULL)
        {
            result = -1;
        }
        
        while (thread_ptr != NULL)
        {
            thread_ptr->routine();
            thread_ptr = (thread *)thread_ptr->get_prev_pointer();
        }
    }
    
    return result;
};
