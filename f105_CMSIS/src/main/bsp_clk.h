/**
 *  @file    bsp_clk.h
 *
 *  @brief   класс отсчета времени
 *  @details используется для отсчета промежутков времени
 *
 *  @author  
 *  @date    
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

#include <misc.h>

#include "list.h"

/// Квантование времени в микросекундах
#define BSP_CLK_TICK 1000

typedef void (bsp_clk_callback_t)(void);

/**
 *  @brief      класс отсчета времени
 *  @details    используется для отсчета промежутков времени, или для вызова функций через определенный интервал
 *              Если интервал равен нулю, то таймер считается выключеным. Следует учесть, что все таймеры и коллбеки
 *              выполняются внутри прерывания системного таймера. По этому злоупотреблять этим классом не стоит. 
 */
class bsp_clk : public cpp_list<LIST_TYPE_SYS_CLK>
{
    private:
    protected:
        uint64_t timestamp;
        uint64_t interval;
        uint64_t prev_timestamp;
        bsp_clk_callback_t * callback;
        friend void bsp_clk_routine(void);  ///< Функция, вызываемая из прерывания системного таймера
    public:
        bsp_clk(uint64_t _interval = 1, bsp_clk_callback_t * _callback = NULL);     ///< Конструктор
        void reinit(uint64_t _interval = 1, bsp_clk_callback_t * _callback = NULL); ///< Изменяем настройки таймера
        uint64_t get_timestamp(void);                                               ///< Возвращаем время таймера
        static uint32_t get_tick(void);                                             ///< Интервал кванта времени в микросекундах
};
