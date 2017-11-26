/**
 *  @file    bsp_tmr.cpp
 *
 *  @brief   класс таймеров
 *  @details работа с системными таймерами
 *
 *  @author  
 *  @date    
 */

#include <stdint.h>
#include <stdbool.h>

#include <misc.h>

#include "bsp_tmr.h"


static void bsp_tmr_routine(TIM_TypeDef * _unit)
{
    for (bsp_tmr * tmr_ptr = (bsp_tmr *)bsp_tmr::get_last_pointer();
         tmr_ptr != NULL;
         tmr_ptr = (bsp_tmr *)bsp_tmr::get_last_pointer()
         )
    {
        if (   tmr_ptr->unit == _unit
            && !tmr_ptr->paused 
            && tmr_ptr->callback != NULL)
        {
            tmr_ptr->callback();
            if (tmr_ptr->counter > 0)
            {
                if (--tmr_ptr->counter == 0)
                {
                    tmr_ptr->paused = true;
                }
            }
        }
    }
}

extern "C" void TIM2_IRQHandler(void)
{
    bsp_tmr_routine(TIM2);
}

extern "C" void TIM3_IRQHandler(void)
{
    bsp_tmr_routine(TIM3);
}

extern "C" void TIM4_IRQHandler(void)
{
    bsp_tmr_routine(TIM4);
}

extern "C" void TIM5_IRQHandler(void)
{
    bsp_tmr_routine(TIM5);
}

extern "C" void TIM6_IRQHandler(void)
{
    bsp_tmr_routine(TIM6);
}

extern "C" void TIM7_IRQHandler(void)
{
    bsp_tmr_routine(TIM7);
}
