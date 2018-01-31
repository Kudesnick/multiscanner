/**
 *  @file    thread_iface.cpp
 *
 *  @brief   класс потока интерфейса
 *  @details универсальный абстрактный класс потока интерфейса. Предоставляет интерфейс для роутинга сообщений
 *
 *  @author  
 *  @date    22.11.2017
 */

#include <stdbool.h>
#include <typeinfo>

#include "rtt.h"
#include "thread_iface.h"

thread_iface::thread_iface(iface_type_t _class_type, iface_name_t _object_name):
    thread(_class_type, _object_name),
    timestamp(0),
    buf(_class_type, _object_name)
{
    RTT_CREATE_LOG;
}

/**
 *  @brief   основной процесс ввода-вывода
 *  @details проверяем буфер на передачу, передаем сообщение, если интерфейс свободен.
 *           если нет, то откладываем передачу до освобождения и передаем управление планировщику.
 *           здесь же разруливаем повторную передачу сообщений по таймауту.
 */
void thread_iface::routine(void)
{
    uint64_t delta_time = timer.get_timestamp() - timestamp;
    timestamp += delta_time;
    
    for(fifo_ptr_t i = buf.tx.get_full_count(); i > 0; i--)
    {
        msg_t msg = buf.tx.extract();
        
        // Если таймер еще не вышел - декрементируем его
        if (msg.counter > delta_time)
        {
            msg.counter -= delta_time;
            buf.tx.add(msg);
        }
        // Передаем сообщение
        else if (send_msg(&msg))
        {
            // Если счетчик количества передач не обнулен - декрементируем его и назад в очередь
            if (msg.count != 1)
            {
                if (msg.interval >= (delta_time - msg.counter))
                {
                    msg.counter = msg.interval - (delta_time - msg.counter);
                }
                else
                {
                    msg.counter = 0;
                }
                
                if (msg.count != 0)
                {
                    msg.count--;
                }
                buf.tx.add(msg);
            }
        }
        // Если не можем передать, то закидываем назад в буфер но с обнуленным таймером
        else
        {
            msg.counter = 0;
            buf.tx.add(msg);
        }
    }
}
