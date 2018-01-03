/**
 *  @file    thread.h
 *
 *  @brief   јбстрактный класс потока
 *  @details ѕотоки представл€ют собой алгоритмы, выполн€емые в режиме кооперативной многозадачности
 *           все запускаемые потоки выполн€ютс€ по очереди в основном цикле
 *
 *  @author  
 *  @date    
 */

#include "list.h"
#include "thread.h"

/// “аймер потоков должен считать с интервалом 1 мс.
bsp_clk thread::timer = bsp_clk();

thread::thread(iface_type_t _class_type, iface_name_t _object_name):
    cpp_list(_class_type, _object_name)
{
};
