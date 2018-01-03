/**
 *  @file    bsp_tmr.cpp
 *
 *  @brief   ����� ��������
 *  @details ������ � ���������� ���������
 *
 *  @author  
 *  @date    
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

#include <misc.h>

#include "list.h"

typedef void (bsp_tmr_callback_t)(void);

class bsp_tmr : public cpp_list<LIST_TYPE_TMR>
{
    private:
    protected:
        TIM_TypeDef * unit; ///< ���������� ������ �������
        bsp_tmr_callback_t * callback; ///< �������, ����������� �� ������������ �������
        uint32_t interval; ///< �������� ��������� �������
        uint32_t counter; ///< ���������� ���������� ������ ����� ���������� �������
        bool paused; ///< ������ ����� �� �����
        void stop_timers_detect(void); /// ��������� ������� �� ������� ����������� ����������
        friend void bsp_tmr_routine(TIM_TypeDef * _unit); ///< �������, ���������� �� ���������� ����������� �������
    public:
        bsp_tmr(TIM_TypeDef * _unit, uint32_t _interval, bsp_tmr_callback_t * _callback, uint32_t _counter = 0); ///< �����������
        void reinit(uint32_t _interval, bsp_tmr_callback_t * _callback, uint32_t _counter = 0); ///< �������� ��������� �������
        void pause(void); ///< ������ ������ �� �����
        void start(void); ///< ��������� ������ � ���������� �����������
};
