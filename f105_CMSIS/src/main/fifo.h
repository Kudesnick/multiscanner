//------------------------------------------------------------------------------
// Буфер данных
//------------------------------------------------------------------------------

#ifndef _FIFO_H_
#define _FIFO_H_

#define sizeof_arr(a) (sizeof(a)/sizeof(a[0]))

#define FIFO_T(type,size) struct{uint16_t begin_idx, end_idx; type data[size];}

#define FIFO_GET_SIZE(queue)        (sizeof_arr(queue.data))
#define FIFO_GET_FULL_COUNT(queue)  (queue.end_idx - queue.begin_idx)
#define FIFO_IS_FULL(queue)         (FIFO_GET_FULL_COUNT(queue) >= FIFO_GET_SIZE(queue))
#define FIFO_IS_EMPTY(queue)        (queue.end_idx == queue.begin_idx)
#define FIFO_ADD(queue,item)        (queue.data[queue.end_idx++ & (FIFO_GET_SIZE(queue) - 1)] = item)
#define FIFO_EXTRACT(queue)         (queue.data[queue.begin_idx++ & (FIFO_GET_SIZE(queue) - 1)])
#define FIFO_READ_HEAD(queue)       (queue.data[queue.begin_idx & (FIFO_GET_SIZE(queue) - 1)])
#define FIFO_READ_END(queue)        (queue.data[(queue.end_idx - 1) & (FIFO_GET_SIZE(queue) - 1)])
#define FIFO_READ_ITEM(queue,item)  (queue.data[((queue.begin_idx) + item) & (FIFO_GET_SIZE(queue) - 1)])
#define FIFO_CLEAR(queue)           (queue.end_idx = queue.begin_idx)

#endif /* _FIFO_H_ */
