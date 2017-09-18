#ifndef _BSP_SERIAL_N_
#define _BSP_SERIAL_N_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "fifo_serial.h"
#include "bsp_usart.h"

typedef struct
{
    // Хардварные настройки
    uint32_t baudrate;      // Бодрейт
    uint16_t word_length;   // Длина сообщения в битах
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
        fifo_serial * bufer;
        static bsp_serial_config_t default_sett;
        bsp_serial_config_t setting;
        virtual void callback(void * msg, uint32_t flags); // В теле метода преобразовать (void *)->(msg_serial_t *)
    protected:
    public:
        bsp_serial(USART_TypeDef *_unit_ptr, fifo_serial * buf, bsp_serial_config_t * _setting = &default_sett);
        bool send(msg_serial_t *msg);
        bsp_serial_config_t *get_setting(void);
        void set_setting(bsp_serial_config_t * sett);
        uint32_t round_baud(uint32_t baud); // Вычисление истинного бодрейта после применения настроек
};

#endif  /* _BSP_SERIAL_N_ */ 
