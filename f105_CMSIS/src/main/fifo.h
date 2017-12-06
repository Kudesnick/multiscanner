/**
 *  @file    fifo.h
 *
 *  @brief   ����� ������
 *  @details ��������� ����� ��������������� ������ ���� FIFO. ����������� ����������� ������ ��� ����������� �������
 *
 *  @author  
 *  @date    06.12.2017
 */

#pragma once

#include <stdint.h>

#include "misc.h"

/// ��� ���������� - ��������� �� ����� ������� ������ (������������ ������������ ������ �������)
typedef uint16_t fifo_ptr_t;

/**
 *  @brief   ��������� ����� ������
 *  @details ����� ������ ����� �����, ������� ������� ������. ��� ���������� ��� ����������� �������
 *           ���������� ������� � ��������� ������
 */
typedef enum : fifo_ptr_t
{
    FIFO_SIZE_2     = 2,
    FIFO_SIZE_4     = 4,
    FIFO_SIZE_8     = 8,
    FIFO_SIZE_16    = 16,
    FIFO_SIZE_32    = 32,
    FIFO_SIZE_64    = 64,
    FIFO_SIZE_128   = 128,
    FIFO_SIZE_256   = 256,
    FIFO_SIZE_512   = 512,
    FIFO_SIZE_1024  = 1024,
    FIFO_SIZE_2048  = 2048,
    FIFO_SIZE_4096  = 4096,
    FIFO_SIZE_8192  = 8192,
    FIFO_SIZE_16364 = 16364,
    FIFO_SIZE_32768 = 32768,
} fifo_size_t;

/**
 *  @brief ��������� ����� ������ FIFO
 *  @details ����� ����� ����� ���. ������ ��������� ��������� ���
 *
 *  @param[in] data_t  ��� ���������, ���������� � ������
 *  @param[in] count   ���������� ��������� ������� - ��������� ������
 */
template <typename data_t, const fifo_size_t count> class cpp_fifo
{
	private:
		data_t fifo[count]; ///< ������ ��� �������� ���������
		fifo_ptr_t head, end; ///< ��������� �� ������ � ����� �������
	public:
		cpp_fifo(void);                     ///< �����������
		fifo_size_t get_count(void);        ///< �������� ������������ ����� �������
		fifo_ptr_t get_full_count(void);    ///< �������� ���-�� ��������� � �������
		bool is_full(void);                 ///< ������� ���������
		bool is_empty(void);                ///< ������� �����
		virtual void add(data_t data);      ///< �������� ������� � �������
		virtual data_t extract(void);       ///< ������� ������� �� �������
		data_t read_head(void);             ///< ��������� ������
		data_t read_end(void);              ///< ��������� �����
		void clear(void);                   ///< �������� �������
};

/**
 *  @brief �����������
 */
template <typename data_t, const fifo_size_t count> cpp_fifo<data_t, count>::cpp_fifo(void):
    head(0),
    end(0)
{
}

/**
 *  @brief �������� ����� ������� �������
 *  @details ���������� ������������ ���������� ��������� �������
 *
 *  @return ����� �������
 */
template <typename data_t, const fifo_size_t count> fifo_size_t cpp_fifo<data_t, count>::get_count(void)
{
    return count;
}

/**
 *  @brief �������� ����������� ���������� ��������� � �������
 *
 *  @return ���������� ��������� � �������
 */
template <typename data_t, const fifo_size_t count> fifo_ptr_t cpp_fifo<data_t, count>::get_full_count(void)
{
    return (end - head);
}

/**
 *  @brief �������� ���������� �������
 *
 *  @return true - ������� ���������, false - � ������� ���� ��������� �����
 */
template <typename data_t, const fifo_size_t count> bool cpp_fifo<data_t, count>::is_full(void)
{
    return ((end - head) >= count);
}

/**
 *  @brief �������� ����������� �������
 *
 *  @return true - ������� �����, false - ������� �������� ������������� ��������
 */
template <typename data_t, const fifo_size_t count> bool cpp_fifo<data_t, count>::is_empty(void)
{
    return (end == head);
}

/**
 *  @brief ���������� �������� � �������
 *
 *  @param[in] data ������ ��� ���������� � ����� �������
 *
 *  @warning ����� �� ���������� �������� ������� �� ������������. ��� ������������ ������� ����������.
 */
template <typename data_t, const fifo_size_t count> void cpp_fifo<data_t, count>::add(data_t data)
{
    register fifo_ptr_t tmp;
    
    do
    {
        tmp = __LDREXW(&end);
        tmp++;
    }
    while(__STREXW(tmp, &end));
    
    fifo[--tmp & (count - 1)] = data;
}

/**
 *  @brief ���������� �������� �� �������
 *
 *  @return ������, ����������� �� ������ �������
 *
 *  @warning ����� �� ���������� �������� ������� �� ������������.
 *           � ������ ������� ���������� �������� �� ������ �������, ��� ����������� �������
 */
template <typename data_t, const fifo_size_t count> data_t cpp_fifo<data_t, count>::extract(void)
{
    return fifo[head++ & (count - 1)];
}

/**
 *  @brief ������ �������� �� ������ �������
 *  @details ������ ������ �� �� ������� �� �� �������
 *
 *  @return ������, ���������� � ������ �������
 *
 *  @warning ����� �� ���������� �������� ������� �� ������� ������. ���� ������� ����� - ���������� �����.
 */
template <typename data_t, const fifo_size_t count> data_t cpp_fifo<data_t, count>::read_head(void)
{
    return fifo[head & (count - 1)];
}

/**
 *  @brief ������ �������� �� ������ �������
 *  @details ������ ������ �� �� ������� �� �� �������
 *
 *  @return ������, ���������� � ������ �������
 *
 *  @warning ����� �� ���������� �������� ������� �� ������� ������. ���� ������� ����� - ���������� �����.
 */
template <typename data_t, const fifo_size_t count> data_t cpp_fifo<data_t, count>::read_end(void)
{
    return fifo[(end - 1) & (count - 1)];
}

/**
 *  @brief ������� �������
 *  @details ������� ��� ������ �� �������. ���������� - ������������ ��������� ������ � ������.
 */
template <typename data_t, const fifo_size_t count> void cpp_fifo<data_t, count>::clear(void)
{
    head = end;
}
