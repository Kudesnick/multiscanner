/********************************************************************
 * cpp_multiscanner класс основного приложения
 ********************************************************************/

#include "multiscanner.h"
#include "thread_con.h"
#include "thread_serial.h"
#include "units_config.h"

cpp_multiscanner::cpp_multiscanner(void)
{
    // просто перечисляем здесь все создаваемые потоки c модификатором static
    static thread_con console(parser_parse);
    #if (LIN1_ENABLED == 1)
        static thread_serial serial_1(LIN1_UNIT, IFACE_NAME_UART1);
    #endif
    #if (LIN2_ENABLED == 1)
        static thread_serial serial_2(LIN2_UNIT, IFACE_NAME_UART2);
    #endif
};

int cpp_multiscanner::routine(void)
{
    static cpp_multiscanner multiscanner_single;
    
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
