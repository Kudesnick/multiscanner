//------------------------------------------------------------------------------
// Порт ввода-вывода
//------------------------------------------------------------------------------

#pragma once

#include "stm32f10x_conf.h"

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

#define GPIO_Pin(n) (1<<n)

class bsp_io
{
    private:
        static const GPIOSpeed_TypeDef GPIO_Speed_default = GPIO_Speed_50MHz;
    protected:
        GPIO_TypeDef* GPIOx;
        GPIO_InitTypeDef GPIO_InitStruct;
        bool invert;
    public:
        bsp_io(void);
        void init(GPIO_TypeDef* GPIO_unit, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode, bool value = true, bool invert_mode = false);
        bsp_io(GPIO_TypeDef* GPIO_unit, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode, bool value = true, bool invert_mode = false);
        bool get_val(void);
        void set_val(bool value);
        void set_sped(GPIOSpeed_TypeDef speed = GPIO_Speed_default);
};
