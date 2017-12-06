/**
 *  @file    fifo.h
 *
 *  @brief   Буфер данных
 *  @details Шаблонный класс закольцованного буфера типа FIFO. Гарантирует целостность данных при асинхронном доступе
 *
 *  @author  
 *  @date    06.12.2017
 */

#pragma once

#include <stdint.h>

#include "misc.h"

/// Тип переменной - указателя на длину массива буфера (ограничивает максимальный размер очереди)
typedef uint16_t fifo_ptr_t;

/**
 *  @brief   Возможная длина буфера
 *  @details Буфер должен иметь длину, кратную степени двойки. Это необходимо для организации быстрых
 *           алгоритмов доступа к элементам буфера
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
 *  @brief Шаблонный класс буфера FIFO
 *  @details Может иметь любой тип. Размер ограничен размерами ОЗУ
 *
 *  @param[in] data_t  Тип элементов, хранящихся в буфере
 *  @param[in] count   Количество элементов массива - хранилища буфера
 */
template <typename data_t, const fifo_size_t count> class cpp_fifo
{
	private:
		data_t fifo[count]; ///< Массив для хранения элементов
		fifo_ptr_t head, end; ///< Указатели на голову и хвост очереди
	public:
		cpp_fifo(void);                     ///< Конструктор
		fifo_size_t get_count(void);        ///< Получить максимальную длину очереди
		fifo_ptr_t get_full_count(void);    ///< Получить кол-во элементов в очереди
		bool is_full(void);                 ///< Очередь заполнена
		bool is_empty(void);                ///< Очередь пуста
		virtual void add(data_t data);      ///< Добавить элемент в очередь
		virtual data_t extract(void);       ///< Извлечь элемент из очереди
		data_t read_head(void);             ///< Прочитать голову
		data_t read_end(void);              ///< Прочитать хвост
		void clear(void);                   ///< Очистить очередь
};

/**
 *  @brief Конструктор
 */
template <typename data_t, const fifo_size_t count> cpp_fifo<data_t, count>::cpp_fifo(void):
    head(0),
    end(0)
{
}

/**
 *  @brief Получить длину массива очереди
 *  @details Возвращает максимальное количество элементов очереди
 *
 *  @return Длина очереди
 */
template <typename data_t, const fifo_size_t count> fifo_size_t cpp_fifo<data_t, count>::get_count(void)
{
    return count;
}

/**
 *  @brief Получить фактическое количество элементов в очереди
 *
 *  @return Количество элементов в очереди
 */
template <typename data_t, const fifo_size_t count> fifo_ptr_t cpp_fifo<data_t, count>::get_full_count(void)
{
    return (end - head);
}

/**
 *  @brief Проверка заполнения очереди
 *
 *  @return true - очередь заполнена, false - в очереди есть свободное место
 */
template <typename data_t, const fifo_size_t count> bool cpp_fifo<data_t, count>::is_full(void)
{
    return ((end - head) >= count);
}

/**
 *  @brief Проверка опустошения очереди
 *
 *  @return true - очередь пуста, false - очередь содержит непрочитанные элементы
 */
template <typename data_t, const fifo_size_t count> bool cpp_fifo<data_t, count>::is_empty(void)
{
    return (end == head);
}

/**
 *  @brief Добавление элемента в очередь
 *
 *  @param[in] data Данные для добавления в хвост очереди
 *
 *  @warning здесь не происходит проверки очереди на переполнение. При переполнении очередь обнуляется.
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
 *  @brief Извлечение элемента из очереди
 *
 *  @return Данные, извлеченные из головы очереди
 *
 *  @warning Здесь не происходит проверки очереди на переполнение.
 *           В случае попытки извлечения элемента из пустой очереди, она заполняется мусором
 */
template <typename data_t, const fifo_size_t count> data_t cpp_fifo<data_t, count>::extract(void)
{
    return fifo[head++ & (count - 1)];
}

/**
 *  @brief Чтение элемента из головы очереди
 *  @details Читает данные но не удаляет их из очереди
 *
 *  @return Данные, хранящиеся в голове очереди
 *
 *  @warning Здесь не происходит проверки очереди на наличие данных. Если очередь пуста - возвращает мусор.
 */
template <typename data_t, const fifo_size_t count> data_t cpp_fifo<data_t, count>::read_head(void)
{
    return fifo[head & (count - 1)];
}

/**
 *  @brief Чтение элемента из хвоста очереди
 *  @details Читает данные но не удаляет их из очереди
 *
 *  @return Данные, хранящиеся в хвосте очереди
 *
 *  @warning Здесь не происходит проверки очереди на наличие данных. Если очередь пуста - возвращает мусор.
 */
template <typename data_t, const fifo_size_t count> data_t cpp_fifo<data_t, count>::read_end(void)
{
    return fifo[(end - 1) & (count - 1)];
}

/**
 *  @brief Очищает очередь
 *  @details Удаляет все данные из очереди. Фактически - приравнивает указатели хвоста и головы.
 */
template <typename data_t, const fifo_size_t count> void cpp_fifo<data_t, count>::clear(void)
{
    head = end;
}
