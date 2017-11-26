/**
 *  @file    bsp_clk.h
 *
 *  @brief   ����� ������� �������
 *  @details ������������ ��� ������� ����������� �������
 *
 *  @author  
 *  @date    
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

#include <misc.h>

#include "list.h"

/// ����������� ������� � �������������
#define BSP_CLK_TICK 1000

typedef void (bsp_clk_callback_t)(void);

/**
 *  @brief      ����� ������� �������
 *  @details    ������������ ��� ������� ����������� �������, ��� ��� ������ ������� ����� ������������ ��������
 *              ���� �������� ����� ����, �� ������ ��������� ����������. ������� ������, ��� ��� ������� � ��������
 *              ����������� ������ ���������� ���������� �������. �� ����� �������������� ���� ������� �� �����. 
 */
class bsp_clk : public cpp_list<LIST_TYPE_SYS_CLK>
{
    private:
    protected:
        uint64_t timestamp;
        uint64_t interval;
        uint64_t prev_timestamp;
        bsp_clk_callback_t * callback;
        friend void bsp_clk_routine(void);  ///< �������, ���������� �� ���������� ���������� �������
    public:
        bsp_clk(uint64_t _interval = 1, bsp_clk_callback_t * _callback = NULL);     ///< �����������
        void reinit(uint64_t _interval = 1, bsp_clk_callback_t * _callback = NULL); ///< �������� ��������� �������
        uint64_t get_timestamp(void);                                               ///< ���������� ����� �������
        static uint32_t get_tick(void);                                             ///< �������� ������ ������� � �������������
};
