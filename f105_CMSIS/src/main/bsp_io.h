//------------------------------------------------------------------------------
// Порт ввода-вывода
//------------------------------------------------------------------------------

#ifndef _BSP_IO_
#define _BSP_IO_

#include "stm32f10x_conf.h"

class cpp_port
{
    private:
        static const GPIOSpeed_TypeDef GPIO_Speed_default = GPIO_Speed_50MHz;
    protected:
        GPIO_TypeDef* GPIOx;
        GPIO_InitTypeDef GPIO_InitStruct;
        bool invert;
    public:
        cpp_port(GPIO_TypeDef* GPIO_unit, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode, bool value = true, bool invert_mode = false);
        bool get_val(void);
        void set_val(bool value);
        void set_sped(GPIOSpeed_TypeDef speed = GPIO_Speed_default);
};

#endif /* _BSP_IO_ */
