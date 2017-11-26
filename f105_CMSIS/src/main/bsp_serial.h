
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "fifo_buffers.h"
#include "bsp_usart.h"

typedef struct
{
    // Хардварные настройки
    uint32_t baudrate;      // Бодрейт
    uint16_t parity;        // Паритет
    uint16_t stop_bits;     // Количество стоповых битов
    // Программные настройки
    uint16_t timeout;       // Таймаут окончания сообщения, мс.
     int16_t byte_of_begin; // Байт, детектируемый как начало сообщения (если < 0 - игнорируется)
     int16_t byte_of_end;   // Байт, детектируемый как окончание сообщения (если < 0 - игнорируется)
     uint8_t max_len;       // Максимальная длина сообщения (если 0 - ограничивается буфером)
        bool echo;          // Эхо отправки сообщений
        bool enable;        // Вкл./выкл. интерфейс
} bsp_serial_config_t;

class bsp_serial : private bsp_usart
{
    private:
        fifo_buff * bufer;
        cpp_fifo<uint8_t, UART_DATA_LEN_MAX> internal_tx;
        cpp_fifo<uint8_t, UART_DATA_LEN_MAX> internal_rx;
        static bsp_serial_config_t default_sett;
        bsp_serial_config_t setting;
        virtual void callback(void * msg, uint32_t flags); // В теле метода преобразовать (void *)->(uint8_t)
    protected:
    public:
        bsp_serial(USART_TypeDef *_unit_ptr, fifo_buff * buf, bsp_serial_config_t * _setting = &default_sett, iface_name_t _name = IFACE_NAME_DEF);
        bool send(const uint8_t data);
        bsp_serial_config_t *get_setting(void);
        void set_setting(bsp_serial_config_t * sett);
        uint32_t round_baud(uint32_t baud); // Вычисление истинного бодрейта после применения настроек
};
