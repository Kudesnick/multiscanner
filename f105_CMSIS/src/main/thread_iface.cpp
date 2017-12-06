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

#include "thread_iface.h"

thread_iface::thread_iface(iface_type_t _class_type, iface_name_t _object_name):
    thread(_class_type, _object_name),
    buf()
{

}

msg_t *thread_iface::msg_get(void)
{
    msg_t * result = NULL;
    
    if (!buf.rx.is_empty())
    {
        static msg_t msg;
        msg = buf.rx.extract();
        result = &msg;
    }
    
    return result;
}

bool thread_iface::msg_send(msg_t *msg)
{
    bool result = msg->msg_type == get_class_type() && !buf.tx.is_full();
    
    if (result)
    {
        buf.tx.add(*msg);
    }
    
    return result;
}

/**
 *  @brief   основной процесс ввода-вывода
 *  @details проверяем буфер на передачу, передаем сообщение, если интерфейс свободен.
 *           если нет, то откладываем передачу до освобождения и передаем управление планировщику
 */
void thread_iface::routine(void)
{
    static uint64_t timestamp = 0;

    if (send_msg_rdy())
    {
        uint64_t delta_time = timer.get_timestamp() - timestamp;
        timestamp += delta_time;
        
        for(fifo_ptr_t i = buf.tx.get_full_count(); i > 0; i--)
        {
            msg_t msg = buf.tx.extract();
            
            if (msg.counter > delta_time)
            {
                msg.counter -= delta_time;
                buf.tx.add(msg);
            }
            else
            {
                send_msg(&msg);

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
                
                if (!send_msg_rdy())
                {
                    break;
                }
            }
            
        }
    }
}
