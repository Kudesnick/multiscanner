#include "thread_route.h"
#include "fifo_buffers.h"
#include "list.h"
#include "msg_types.h"

thread_route::thread_route():
    thread()
{
}

/// Перекидываем сообщения из интерфейса в интерфейс согласно названию целевого интерфейса
void thread_route::routine(void)
{
    static thread_route thread_route_single;
    
    for (fifo_buff * rx = (fifo_buff *)fifo_buff::get_last_pointer(); rx != NULL; rx = (fifo_buff *)rx->get_prev_pointer())
    {
        if (!rx->rx.is_empty())
        {
            msg_t msg = rx->rx.extract();
            
            fifo_buff * tx = (fifo_buff *)fifo_buff::get_object(msg.msg_type, msg.route);
            
            if (tx != NULL)
            {
                if (tx->tx.is_full())
                {
                    rx->rx.add(msg);
                }
                else
                {
                    msg.route = rx->get_object_name();
                    tx->tx.add(msg);
                }
            }
        }
    }
}
