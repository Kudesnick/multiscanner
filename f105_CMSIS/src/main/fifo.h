//------------------------------------------------------------------------------
// Буфер данных
//------------------------------------------------------------------------------

#ifndef _FIFO_H_
#define _FIFO_H_

#include "stdint.h"

#define FIFO_SIZE_2     2
#define FIFO_SIZE_4     4
#define FIFO_SIZE_8     8
#define FIFO_SIZE_16    16
#define FIFO_SIZE_32    32
#define FIFO_SIZE_64    64
#define FIFO_SIZE_128   128
#define FIFO_SIZE_256   256
#define FIFO_SIZE_512   512
#define FIFO_SIZE_1024  1024
#define FIFO_SIZE_2048  2048
#define FIFO_SIZE_4096  4096
#define FIFO_SIZE_8192  8192
#define FIFO_SIZE_16364 16364

typedef uint16_t fifo_ptr_t;

#ifdef __cplusplus

template <typename data_t, const fifo_ptr_t count> class cpp_fifo
{
	protected:
		data_t fifo[count];
		fifo_ptr_t head, end;
	public:
		cpp_fifo(void);
		fifo_ptr_t get_count(void);
		fifo_ptr_t get_full_count(void);
		bool is_full(void);
		bool is_empty(void);
		void add(data_t data);
		data_t extract(void);
		data_t read_head(void);
		data_t read_end(void);
		void clear(void);
};

#endif /* __cplusplus */

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
