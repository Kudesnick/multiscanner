#ifndef _BSP_USART_H_
#define _BSP_USART_H_

#include "bsp_unit.h"

//------------------------------------------------------------------------------
// Класс порта ввода/вывода UART
//------------------------------------------------------------------------------

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
    bool USART_Half_Duplex;
    bool USART_Enable;
} bsp_usart_setting_t;

class bsp_usart : public bsp_unit
{
    private:
    protected:
        bsp_usart_setting_t setting;
    public:
        bsp_usart(USART_TypeDef *unit, void (*clbck)(USART_TypeDef *unit));
        virtual void send_sett(bsp_usart_setting_t *sett); // Применение новых настроек модуля
        virtual void *get_sett(void); // Получение настроек модуля
        virtual bool send_msg(uint16_t data); // Отправка данных
        virtual void interrupt_handler(void); // Обработчик прерывания, основные манипуляции с флагами. Из него вызывается callback
};

#endif /* _BSP_USART_H_ */
