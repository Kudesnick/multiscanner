
#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "bsp_io.h"
#include "bsp_unit.h"

//------------------------------------------------------------------------------
// Класс порта ввода/вывода UART
//------------------------------------------------------------------------------

#define USART_LIN_BRK_DATA ((uint16_t)0x8000)
#define USART_IDLE_DATA    ((uint16_t)0xFFFF)
#define USART_NULL_DATA    ((uint16_t)0xF000)

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

typedef USART_TypeDef       unit_t;
typedef uint16_t            message_t;
typedef bsp_usart_setting_t settings_t;

typedef struct
{
    message_t data;
    uint16_t flags;
} bsp_usart_callback_data_t;


class bsp_usart: public bsp_unit<unit_t, message_t, settings_t>, public cpp_list<LIST_TYPE_UNIT_USART>
{
    private:
        bsp_io pin_rx;
        bsp_io pin_tx;
    protected:
        bsp_usart_setting_t setting;
        virtual void callback(void);
        bsp_usart_callback_data_t clbk_data;
        friend bool irq_handler(unit_t *unit);
        virtual void send_sett(settings_t *sett); ///< Применение новых настроек модуля
        virtual settings_t *get_sett(void);       ///< Получение настроек модуля
        virtual bool send_msg(message_t *msg);    ///< Отправка данных
        virtual message_t *get_msg(void);         ///< Получение данных
    public:
        bsp_usart(unit_t *_unit_ptr, iface_type_t _class_type = IFACE_TYPE_DEF, iface_name_t _object_name = IFACE_NAME_DEF);
        uint32_t round_baud(settings_t *sett);    ///< Вычисление истинного бодрейта после применения настроек
};
