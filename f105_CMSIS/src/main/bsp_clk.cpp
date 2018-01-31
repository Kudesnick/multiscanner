/**
 *  @file    bsp_clk.cpp
 *
 *  @brief   класс отсчета времени
 *  @details используется для отсчета промежутков времени
 *
 *  @author  
 *  @date    
 */

#include <stdint.h>
#include <stdbool.h>
#include <typeinfo>

#include <misc.h>

#include "rtt.h"
#include "bsp_clk.h"

bsp_clk::bsp_clk(uint64_t _interval, bsp_clk_callback_t * _callback):
    cpp_list(),
    timestamp(0),
    interval(_interval),
    prev_timestamp(0),
    callback(_callback)
{
    RTT_CREATE_LOG;
    
    if (get_prev_pointer() == NULL)
    {
        RCC_ClocksTypeDef RCC_Clocks;

        RCC_GetClocksFreq(&RCC_Clocks);
        SysTick_Config(RCC_Clocks.HCLK_Frequency / BSP_CLK_TICK);
    }
};

void bsp_clk::reinit(uint64_t _interval, bsp_clk_callback_t * _callback) 
{
    timestamp = 0;
    interval = _interval;
    prev_timestamp = 0;
    callback = _callback; 
};

uint64_t bsp_clk::get_timestamp(void)
{
    return timestamp;
}

uint32_t bsp_clk::get_tick(void)
{
    return BSP_CLK_TICK;
};


static void bsp_clk_routine(void)
{
    for(bsp_clk * clk_ptr = (bsp_clk *)bsp_clk::get_last_pointer();
        clk_ptr != NULL;
        clk_ptr = (bsp_clk *)clk_ptr->get_prev_pointer()
        )
    {
        if (clk_ptr->interval > 0)
        {
            clk_ptr->timestamp++;
            if (clk_ptr->callback != NULL)
            {
                if (   clk_ptr->interval == 1
                    || clk_ptr->prev_timestamp + clk_ptr->interval <= clk_ptr->timestamp)
                {
                    // Реализованный механизм подразумевает накопление ошибки времени. Это сделано умышленно, чтобы 
                    // Тормозные задачи не могли повесить систему совсем
                    clk_ptr->prev_timestamp = clk_ptr->timestamp;
                    clk_ptr->callback();
                }
            }
        }
    }
}

extern "C" void SysTick_Handler(void)
{
    bsp_clk_routine();
}
