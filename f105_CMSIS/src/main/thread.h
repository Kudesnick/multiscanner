/**
 *  @file    thread.h
 *
 *  @brief   Абстрактный класс потока
 *  @details Потоки представляют собой алгоритмы, выполняемые в режиме кооперативной многозадачности
 *           все запускаемые потоки выполняются по очереди в основном цикле
 *
 *  @author  
 *  @date    
 */
#pragma once

#include "list.h"
#include "bsp_clk.h"

/// Абстрактный класс потока
class thread : public cpp_list<LIST_TYPE_THREAD>
{
    private:
    protected:
        static bsp_clk timer; ///< Системный таймер. Общий для всех потоков
    public:
        /// Конструктор потока
        thread(iface_type_t _class_type = IFACE_TYPE_DEF, iface_name_t _object_name = IFACE_NAME_DEF);
        virtual void routine(void) = NULL; ///< Собственно, тело потока
};
