/**
 *  @file    bsp_io.h
 *
 *  @brief   Порт ввода-вывода
 *  @details Модуль для работы с пинами GPIO
 */

#pragma once

#include "stm32f10x_conf.h"

#include "list.h"

/// Макросы-опредленния ножек порта A
#define GPIOA_00 GPIOA,GPIO_Pin_0
#define GPIOA_01 GPIOA,GPIO_Pin_1
#define GPIOA_02 GPIOA,GPIO_Pin_2
#define GPIOA_03 GPIOA,GPIO_Pin_3
#define GPIOA_04 GPIOA,GPIO_Pin_4
#define GPIOA_05 GPIOA,GPIO_Pin_5
#define GPIOA_06 GPIOA,GPIO_Pin_6
#define GPIOA_07 GPIOA,GPIO_Pin_7
#define GPIOA_08 GPIOA,GPIO_Pin_8
#define GPIOA_09 GPIOA,GPIO_Pin_9
#define GPIOA_10 GPIOA,GPIO_Pin_10
#define GPIOA_11 GPIOA,GPIO_Pin_11
#define GPIOA_12 GPIOA,GPIO_Pin_12
#define GPIOA_13 GPIOA,GPIO_Pin_13
#define GPIOA_14 GPIOA,GPIO_Pin_14
#define GPIOA_15 GPIOA,GPIO_Pin_15

/// Макросы-опредленния ножек порта B
#define GPIOB_00 GPIOB,GPIO_Pin_0
#define GPIOB_01 GPIOB,GPIO_Pin_1
#define GPIOB_02 GPIOB,GPIO_Pin_2
#define GPIOB_03 GPIOB,GPIO_Pin_3
#define GPIOB_04 GPIOB,GPIO_Pin_4
#define GPIOB_05 GPIOB,GPIO_Pin_5
#define GPIOB_06 GPIOB,GPIO_Pin_6
#define GPIOB_07 GPIOB,GPIO_Pin_7
#define GPIOB_08 GPIOB,GPIO_Pin_8
#define GPIOB_09 GPIOB,GPIO_Pin_9
#define GPIOB_10 GPIOB,GPIO_Pin_10
#define GPIOB_11 GPIOB,GPIO_Pin_11
#define GPIOB_12 GPIOB,GPIO_Pin_12
#define GPIOB_13 GPIOB,GPIO_Pin_13
#define GPIOB_14 GPIOB,GPIO_Pin_14
#define GPIOB_15 GPIOB,GPIO_Pin_15

/// Макросы-опредленния ножек порта C
#define GPIOC_00 GPIOC,GPIO_Pin_0
#define GPIOC_01 GPIOC,GPIO_Pin_1
#define GPIOC_02 GPIOC,GPIO_Pin_2
#define GPIOC_03 GPIOC,GPIO_Pin_3
#define GPIOC_04 GPIOC,GPIO_Pin_4
#define GPIOC_05 GPIOC,GPIO_Pin_5
#define GPIOC_06 GPIOC,GPIO_Pin_6
#define GPIOC_07 GPIOC,GPIO_Pin_7
#define GPIOC_08 GPIOC,GPIO_Pin_8
#define GPIOC_09 GPIOC,GPIO_Pin_9
#define GPIOC_10 GPIOC,GPIO_Pin_10
#define GPIOC_11 GPIOC,GPIO_Pin_11
#define GPIOC_12 GPIOC,GPIO_Pin_12
#define GPIOC_13 GPIOC,GPIO_Pin_13
#define GPIOC_14 GPIOC,GPIO_Pin_14
#define GPIOC_15 GPIOC,GPIO_Pin_15

/// Макросы-опредленния ножек порта D
#define GPIOD_00 GPIOD,GPIO_Pin_0
#define GPIOD_01 GPIOD,GPIO_Pin_1
#define GPIOD_02 GPIOD,GPIO_Pin_2
#define GPIOD_03 GPIOD,GPIO_Pin_3
#define GPIOD_04 GPIOD,GPIO_Pin_4
#define GPIOD_05 GPIOD,GPIO_Pin_5
#define GPIOD_06 GPIOD,GPIO_Pin_6
#define GPIOD_07 GPIOD,GPIO_Pin_7
#define GPIOD_08 GPIOD,GPIO_Pin_8
#define GPIOD_09 GPIOD,GPIO_Pin_9
#define GPIOD_10 GPIOD,GPIO_Pin_10
#define GPIOD_11 GPIOD,GPIO_Pin_11
#define GPIOD_12 GPIOD,GPIO_Pin_12
#define GPIOD_13 GPIOD,GPIO_Pin_13
#define GPIOD_14 GPIOD,GPIO_Pin_14
#define GPIOD_15 GPIOD,GPIO_Pin_15

/// Макросы-опредленния ножек порта E
#define GPIOE_00 GPIOE,GPIO_Pin_0
#define GPIOE_01 GPIOE,GPIO_Pin_1
#define GPIOE_02 GPIOE,GPIO_Pin_2
#define GPIOE_03 GPIOE,GPIO_Pin_3
#define GPIOE_04 GPIOE,GPIO_Pin_4
#define GPIOE_05 GPIOE,GPIO_Pin_5
#define GPIOE_06 GPIOE,GPIO_Pin_6
#define GPIOE_07 GPIOE,GPIO_Pin_7
#define GPIOE_08 GPIOE,GPIO_Pin_8
#define GPIOE_09 GPIOE,GPIO_Pin_9
#define GPIOE_10 GPIOE,GPIO_Pin_10
#define GPIOE_11 GPIOE,GPIO_Pin_11
#define GPIOE_12 GPIOE,GPIO_Pin_12
#define GPIOE_13 GPIOE,GPIO_Pin_13
#define GPIOE_14 GPIOE,GPIO_Pin_14
#define GPIOE_15 GPIOE,GPIO_Pin_15

/// Макросы-опредленния ножек порта F
#define GPIOF_00 GPIOF,GPIO_Pin_0
#define GPIOF_01 GPIOF,GPIO_Pin_1
#define GPIOF_02 GPIOF,GPIO_Pin_2
#define GPIOF_03 GPIOF,GPIO_Pin_3
#define GPIOF_04 GPIOF,GPIO_Pin_4
#define GPIOF_05 GPIOF,GPIO_Pin_5
#define GPIOF_06 GPIOF,GPIO_Pin_6
#define GPIOF_07 GPIOF,GPIO_Pin_7
#define GPIOF_08 GPIOF,GPIO_Pin_8
#define GPIOF_09 GPIOF,GPIO_Pin_9
#define GPIOF_10 GPIOF,GPIO_Pin_10
#define GPIOF_11 GPIOF,GPIO_Pin_11
#define GPIOF_12 GPIOF,GPIO_Pin_12
#define GPIOF_13 GPIOF,GPIO_Pin_13
#define GPIOF_14 GPIOF,GPIO_Pin_14
#define GPIOF_15 GPIOF,GPIO_Pin_15

/// Макросы-опредленния ножек порта G
#define GPIOG_00 GPIOG,GPIO_Pin_0
#define GPIOG_01 GPIOG,GPIO_Pin_1
#define GPIOG_02 GPIOG,GPIO_Pin_2
#define GPIOG_03 GPIOG,GPIO_Pin_3
#define GPIOG_04 GPIOG,GPIO_Pin_4
#define GPIOG_05 GPIOG,GPIO_Pin_5
#define GPIOG_06 GPIOG,GPIO_Pin_6
#define GPIOG_07 GPIOG,GPIO_Pin_7
#define GPIOG_08 GPIOG,GPIO_Pin_8
#define GPIOG_09 GPIOG,GPIO_Pin_9
#define GPIOG_10 GPIOG,GPIO_Pin_10
#define GPIOG_11 GPIOG,GPIO_Pin_11
#define GPIOG_12 GPIOG,GPIO_Pin_12
#define GPIOG_13 GPIOG,GPIO_Pin_13
#define GPIOG_14 GPIOG,GPIO_Pin_14
#define GPIOG_15 GPIOG,GPIO_Pin_15

#define NUM_OF_GPIO_UNITS 7 ///< Количество модулей GPIO (A..G)

/// Макрос битовой маски пина
#define GPIO_Pin(n) (1<<n)

/// Коллбек по прерыванию на линии
typedef void(bsp_io_callback_t)(uint32_t line);

/**
 *  @brief   Класс порта ввода-вывода
 *  @details Класс инициализирует пин таким образом, что обращение к нему происходит как к булевой переменной,
 *           значение которой инвертируется при необходимости
 *
 *  @warning Два объекта могут указывать на один и тот же пин. В этом случае инициализация и запись пинов перекрывает
 *           настройки таких объектов, за исключением параметра invert.
 *
 */
class bsp_io: public cpp_list<LIST_TYPE_IO>
{
    private:
        static uint16_t init_pins[NUM_OF_GPIO_UNITS]; ///< Флаги для детектирования того, что конкретный пин уже проинициализирован каким-либо модулем
        static bsp_io_callback_t * callback[16]; ///< Функции внешних обработчиков прерываний
        static const GPIOSpeed_TypeDef GPIO_Speed_default = GPIO_Speed_50MHz; ///< Скорость работы порта по умолчанию
        friend void EXTI_Handler(void); ///< Вызывается из прерывания модулей EXTI
    protected:
        GPIO_TypeDef* GPIOx; ///< Указатель на порт ввода-вывода
        GPIO_InitTypeDef GPIO_InitStruct; ///< Номер пина и его аппаратные настройки
        bool invert; ///< признак того, что значение пина будет инвертировано в операциях ввода-вывода
    public:
        bsp_io(void); ///< простой конструктор.
        /// Конструктор с инициализацией
        bsp_io(GPIO_TypeDef* GPIO_unit, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode, bool value = true, bool _invert = false);
        /// Инициализация пина
        void init(GPIO_TypeDef* GPIO_unit, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode, bool value = true, bool _invert = false);
        bool get_val(void); ///< Получить значение (с учетом инверсии)
        void set_val(bool value); ///< Записать значение (с учетом инверсии)
        void set_sped(GPIOSpeed_TypeDef speed = GPIO_Speed_default); ///< Установить предельную скорость работы порта
        /// Настройка прерывания
        void set_exti(bsp_io_callback_t * _callback, EXTITrigger_TypeDef _trigger = EXTI_Trigger_Rising_Falling);
};
