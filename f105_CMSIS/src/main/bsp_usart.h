#ifndef _BSP_USART_H_
#define _BSP_USART_H_

#include <stdint.h>
#include <string.h>

#include "bsp_unit.h"

//------------------------------------------------------------------------------
// Класс порта ввода/вывода UART
//------------------------------------------------------------------------------

#define USART_LIN_BRK_DATA (uint16_t)0x8000;
#define USART_IDLE_DATA    (uint16_t)0xFFFF;

typedef struct
{
    // Хардварные настройки
    uint32_t USART_BaudRate;
    uint16_t USART_WordLength;
    uint16_t USART_StopBits;
    uint16_t USART_Parity;
    uint16_t USART_Mode;
    uint16_t USART_LIN_Break_Detection_Length;
    // Софтварные настройки
    bool USART_LIN_Enable;
    bool USART_Enable;
} bsp_usart_setting_t;

typedef uint16_t bsp_usart_msg_t;

typedef void(bsp_usart_callback_t)(uint16_t data, uint16_t flags);

class bsp_usart: public bsp_unit
{
    private:
        bsp_io pin_rx;
        bsp_io pin_tx;
    protected:
        bsp_usart_setting_t setting;
    public:
        bsp_usart(USART_TypeDef *_unit_ptr, bsp_usart_callback_t *_callback = NULL);
        void send_sett(bsp_usart_setting_t *sett);
        virtual void send_sett(void *sett); // Применение новых настроек модуля
        virtual void *get_sett(void); // Получение настроек модуля
        virtual bool send_msg(void *msg); // Отправка данных
        virtual void interrupt_handler(void); // Обработчик прерывания, основные манипуляции с флагами. Из него вызывается callback
};

#endif /* _BSP_USART_H_ */
