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
        data_t *get_head(void);
};

#endif /* _FIFO_H_ */
