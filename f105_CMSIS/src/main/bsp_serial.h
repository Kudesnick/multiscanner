
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "fifo_buffers.h"
#include "bsp_usart.h"

#define MSG_BUF_LEN FIFO_SIZE_4

typedef __packed struct
{
    // Хардварные настройки
    uint32_t baudrate;      // Бодрейт
    uint16_t parity;        // Паритет
    uint16_t stop_bits;     // Количество стоповых битов
    // Программные настройки
     int16_t byte_of_begin; // Байт, детектируемый как начало сообщения (если < 0 - игнорируется)
     int16_t byte_of_end;   // Байт, детектируемый как окончание сообщения (если < 0 - игнорируется)
     uint8_t max_len;       // Максимальная длина сообщения (если 0 - ограничивается буфером)
        bool echo;          // Эхо отправки сообщений
        bool enable;        // Вкл./выкл. интерфейс
} bsp_serial_config_t;

class bsp_serial : public bsp_usart
{
    private:
        cpp_fifo<msg_t, FIFO_SIZE_4> internal_tx_buf;
        cpp_fifo<uint8_t, UART_DATA_LEN_MAX> internal_tx;
        cpp_fifo<uint8_t, UART_DATA_LEN_MAX> internal_rx;
        fifo_buff * buffer;
        static bsp_serial_config_t default_sett;
        bsp_serial_config_t setting;
        virtual void callback(void);
    protected:
    public:
        bsp_serial(unit_t *_unit_ptr, fifo_buff * _buffer, iface_name_t _name = IFACE_NAME_DEF, bsp_serial_config_t * _setting = &default_sett);
        bool send(msg_t * msg);
        bsp_serial_config_t *get_setting(void);
        void set_setting(bsp_serial_config_t * sett);
        uint32_t round_baud(uint32_t baud); // Вычисление истинного бодрейта после применения настроек
};
