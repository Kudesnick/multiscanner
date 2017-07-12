//------------------------------------------------------------------------------
// Порт ввода-вывода
//------------------------------------------------------------------------------

#ifndef _BSP_IO_
#define _BSP_IO_

#include "stm32f10x_conf.h"

class cpp_port
{
    private:
        static const GPIOSpeed_TypeDef GPIO_Speed_default = GPIO_Speed_2MHz;
        static const bool invert_default = false;
    protected:
        GPIO_TypeDef* GPIOx;
        GPIO_InitTypeDef* GPIO_InitStruct;
        bool invert;
    public:
        cpp_port(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode);
        cpp_port(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode, bool value);
        cpp_port(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode, bool value, bool invert_mode);
        bool get_val(void);
        void set_val(bool value);
};

#endif /* _BSP_IO_ */
