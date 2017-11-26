
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "fifo_con.h"
#include "bsp_usart.h"

typedef struct
{
    uint32_t baudrate;  // Бодрейт
    uint16_t parity;    // Паритет
    uint16_t stop_bits; // Количество стоповых битов
        bool echo;      // Эхо отправки сообщений
        bool color;     // Выделение цветом в консоле
} bsp_con_config_t;

class bsp_con : public bsp_usart
{
    private:
        fifo_con * bufer;
        static bsp_con_config_t default_sett;
        bsp_con_config_t setting;
        virtual void callback(void * msg, uint32_t flags); // В теле метода преобразовать (void *)->(uint16_t)
    protected:
    public:
        bsp_con(USART_TypeDef *_unit_ptr, fifo_con * buf, bsp_con_config_t * _setting = &default_sett);
        bool send(const char *buf);
        bsp_con_config_t *get_setting(void);
        void set_setting(bsp_con_config_t * sett);
        uint32_t round_baud(uint32_t baud); // Вычисление истинного бодрейта после применения настроек
};
