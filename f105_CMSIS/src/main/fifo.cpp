//------------------------------------------------------------------------------
// Буфер данных
//------------------------------------------------------------------------------

#include "fifo.h"

template <typename data_t, const fifo_ptr_t count> cpp_fifo<data_t, count>::cpp_fifo(void):
    head(end)
{
};
	
template <typename data_t, const fifo_ptr_t count> fifo_ptr_t cpp_fifo<data_t, count>::get_count(void)
{
    return count;
};

template <typename data_t, const fifo_ptr_t count> fifo_ptr_t cpp_fifo<data_t, count>::get_full_count(void)
{
    return (end - head);
};

template <typename data_t, const fifo_ptr_t count> bool cpp_fifo<data_t, count>::is_full(void)
{
    return ((fifo_ptr_t)(end - head) >= count);
};

template <typename data_t, const fifo_ptr_t count> bool cpp_fifo<data_t, count>::is_empty(void)
{
    return (end == head);
};

template <typename data_t, const fifo_ptr_t count> void cpp_fifo<data_t, count>::add(data_t data)
{
    fifo[end++ & (count - 1)] = data;
};

template <typename data_t, const fifo_ptr_t count> data_t cpp_fifo<data_t, count>::extract(void)
{
    return fifo[head++ & (count - 1)];
};

template <typename data_t, const fifo_ptr_t count> data_t cpp_fifo<data_t, count>::read_head(void)
{
    return fifo[head & (count - 1)];
};

template <typename data_t, const fifo_ptr_t count> data_t cpp_fifo<data_t, count>::read_end(void)
{
    return fifo[(end - 1) & (count - 1)];
};

template <typename data_t, const fifo_ptr_t count> void cpp_fifo<data_t, count>::clear(void)
{
    head = end;
};
