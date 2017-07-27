//------------------------------------------------------------------------------
// Порт ввода-вывода
//------------------------------------------------------------------------------

#include "bsp_io.h"
#include "misc.h"

cpp_pin::cpp_pin(GPIO_TypeDef* GPIO_unit, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode, bool value, bool invert_mode):
    GPIOx(GPIO_unit), invert(invert_mode)
{
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_default;

    RCC_APB2PeriphClockCmd((GPIOx == GPIOA) ? RCC_APB2Periph_GPIOA :
                           (GPIOx == GPIOB) ? RCC_APB2Periph_GPIOB :
                           (GPIOx == GPIOC) ? RCC_APB2Periph_GPIOC :
                           (GPIOx == GPIOD) ? RCC_APB2Periph_GPIOD :
                           (GPIOx == GPIOE) ? RCC_APB2Periph_GPIOE :
                           (GPIOx == GPIOF) ? RCC_APB2Periph_GPIOF :
                           RCC_APB2Periph_GPIOG, ENABLE);

    if (GPIO_Mode == GPIO_Mode_AF_OD || GPIO_Mode == GPIO_Mode_AF_PP)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    }

    set_val(value);

    GPIO_Init(GPIOx, &GPIO_InitStruct);
};

bool cpp_pin::get_val(void)
{
    return ((GPIO_ReadOutputDataBit(GPIOx, GPIO_InitStruct.GPIO_Pin) != 0) != invert);
};

void cpp_pin::set_val(bool value)
{
    GPIO_WriteBit(GPIOx, GPIO_InitStruct.GPIO_Pin, (invert != value) ? Bit_SET : Bit_RESET);
};

void cpp_pin::set_sped(GPIOSpeed_TypeDef speed)
{
    if (GPIO_InitStruct.GPIO_Speed != speed)
    {
        GPIO_InitStruct.GPIO_Speed = speed;
        GPIO_Init(GPIOx, &GPIO_InitStruct);
    }
};
