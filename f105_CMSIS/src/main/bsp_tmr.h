/**
 *  @file    bsp_tmr.cpp
 *
 *  @brief   класс таймеров
 *  @details работа с системными таймерами
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
        TIM_TypeDef * unit; ///< Аппаратный модуль таймера
        bsp_tmr_callback_t * callback; ///< Функция, выполняемая по переполнении таймера
        uint32_t interval; ///< Интервал отработки таймера
        uint32_t counter; ///< Количество выполненых циклов перед остановкой таймера
        bool paused; ///< Таймер стоит на паузе
        void stop_timers_detect(void); /// Сканируем таймеры на предмет возможности выключения
        friend void bsp_tmr_routine(TIM_TypeDef * _unit); ///< Функция, вызываемая из прерывания аппаратного таймера
    public:
        bsp_tmr(TIM_TypeDef * _unit, uint32_t _interval, bsp_tmr_callback_t * _callback, uint32_t _counter = 0); ///< Конструктор
        void reinit(uint32_t _interval, bsp_tmr_callback_t * _callback, uint32_t _counter = 0); ///< Изменяем настройки таймера
        void pause(void); ///< Ставим таймер на паузу
        void start(void); ///< Запускаем таймер с последними настройками
};
