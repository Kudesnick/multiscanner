#ifndef _BSP_SERIAL_N_
#define _BSP_SERIAL_N_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "fifo_con.h"
#include "bsp_usart.h"

// причины завершения фрейма
enum
{
    SERIAL_MSG_BRK_TIMEOUT, // Завершение по таймауту
    SERIAL_MSG_BRK_LAST_ID, // Детектирован байт завершения
    SERIAL_MSG_BRK_FIRST_ID, // Детектирован байт начала следующей посылки
    SERIAL_MSG_BRK_LENGTH, // Достигнута фиксированная длина посылки
    SERIAL_MSG_BRK_OVF, // Превышена длина буфера сообщения
    SERIAL_MSG_BRK_FRAME_ERR, // Ошибка фрейма
    SERIAL_MSG_BRK_PARITY_ERR, // Ошибка паритета
} msg_brk_reason_t;

typedef struct
{
            uint64_t timestamp;
             uint8_t data[16];
    msg_brk_reason_t reason;
} msg_serial_t;

typedef struct
{
    uint32_t baudrate;  // Бодрейт
    uint16_t parity;    // Паритет
    uint16_t stop_bits; // Количество стоповых битов
        bool echo;      // Эхо отправки сообщений
        bool color;     // Выделение цветом в консоле
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
