/**
 *  @file    bsp_io.cpp
 *
 *  @brief   ����� ����� �����-������
 *  @details
 *
 *  @warning ��� ������� ����� ��������� �� ���� � ��� �� ���. � ���� ������ ������������� � ������ ����� �����������
 *           ��������� ����� ��������, �� ����������� ��������� invert.
 *
 */

#include <string.h>
#include <typeinfo>

#include "rtt.h"
#include "bsp_io.h"
#include "misc.h"

/// ����� ��� �������������� ����, ��� ���������� ��� ��� ������������������ �����-���� �������
uint16_t bsp_io::init_pins[];

/// ������� ������� ������������ ����������
bsp_io_callback_t * bsp_io::callback[]; 


/// ������� �����������.
bsp_io::bsp_io(void):
    cpp_list(),
    GPIOx(NULL)
{
    RTT_CREATE_LOG;
};


/**
 *  @brief   ����������� � �������������� ����
 *  @details �������� ������� � ����������� ����, ��������� ������� ������
 *
 *  @param[in] GPIO_unit ������ �� ������ ����� (PORTA, PORTB � �.�.)
 *  @param[in] GPIO_Pin  ����� ����
 *  @param[in] GPIO_Mode ����� ������ ����
 *  @param[in] value     �������� �� ������ ���� �� ���������
 *  @param[in] _invert   ������� �������� �������� ��� ��������� �����-������
 */
bsp_io::bsp_io(GPIO_TypeDef* GPIO_unit, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode, bool value, bool _invert):
    cpp_list()
{
    RTT_CREATE_LOG;
    
    init(GPIO_unit, GPIO_Pin, GPIO_Mode, value, _invert);
};


/**
 *  @brief   ������������� ���� GPIO
 *  @details �������� ������� � ����������� ����, ��������� ������� ������
 *
 *  @param[in] GPIO_unit ������ �� ������ ����� (PORTA, PORTB � �.�.)
 *  @param[in] GPIO_Pin  ����� ����
 *  @param[in] GPIO_Mode ����� ������ ����
 *  @param[in] value     �������� �� ������ ���� �� ���������
 *  @param[in] _invert   ������� �������� �������� ��� ��������� �����-������
 */
void bsp_io::init(GPIO_TypeDef* GPIO_unit, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode, bool value, bool _invert)
{
    if      (GPIO_unit == GPIOA) init_pins[0] |= GPIO_Pin(GPIO_Pin);
    else if (GPIO_unit == GPIOB) init_pins[1] |= GPIO_Pin(GPIO_Pin);
    else if (GPIO_unit == GPIOC) init_pins[2] |= GPIO_Pin(GPIO_Pin);
    else if (GPIO_unit == GPIOD) init_pins[3] |= GPIO_Pin(GPIO_Pin);
    else if (GPIO_unit == GPIOE) init_pins[4] |= GPIO_Pin(GPIO_Pin);
    else if (GPIO_unit == GPIOF) init_pins[5] |= GPIO_Pin(GPIO_Pin);
    else if (GPIO_unit == GPIOG) init_pins[6] |= GPIO_Pin(GPIO_Pin);
    else    return; // �� �������������� ��������� �����.
    
    GPIOx = GPIO_unit;
    invert = _invert;
    
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
}

/**
 *  @brief  �������� �������� ���� � ������ ��������
 *
 *  @return true - ��� ���������� � ���. "1", false - ��� ������� � ���. "0"
 */
bool bsp_io::get_val(void)
{
    return ((GPIOx != NULL) ? (GPIO_ReadOutputDataBit(GPIOx, GPIO_InitStruct.GPIO_Pin) != 0) != invert : false);
};

/**
 *  @brief  ���������� �������� ���� � ������ ��������
 *
 *  @param[in] value true - ���������� ��� � ���. "1", false - ��� ������� � ���. "0"
 */
void bsp_io::set_val(bool value)
{
    if (GPIOx != NULL)
    {
        GPIO_WriteBit(GPIOx, GPIO_InitStruct.GPIO_Pin, (invert != value) ? Bit_SET : Bit_RESET);
    }
};

/// ���������� ���������� �������� ������ �����
void bsp_io::set_sped(GPIOSpeed_TypeDef speed)
{
    if (GPIOx != NULL
        && GPIO_InitStruct.GPIO_Speed != speed
        )
    {
        GPIO_InitStruct.GPIO_Speed = speed;
        GPIO_Init(GPIOx, &GPIO_InitStruct);
    }
};

/**
 *  @brief     ��������� ����������
 *  @details   ������������� ���������� ���������� � �����, �� �������� ��� �����������
 *
 *  @param[in] _callback ���������� ����������. ���� �������������� � NULL, �� ��������� ���������� �� ������ ����� EXTI
               (�� �� �� �������!) ������������
 *  @param[in] _trigger ���������� �� ������ ������ (Rising, Falling ��� �����) ����������� ����������
 *
 *  @warning   � ����� � ���, ��� ������������ ���������� �� ������� �������� � �� STM ������ ������������� ����������
               ����������� �� ����� ����. ��������, ������ ������������� ���������� �� ���� � ���������� �������,
               �� �� ������ ������. �������� ����������� ������ EXTI ���������� �� ����� 5-9 � 10-15 �������������
               � ������������ ����� ��������� ��������������. ��� �� �����, �� ������ ����� ���� ����� ����������
               ���� ������� ��������. ��� ����������� ���������� ������� ������.
 */
void bsp_io::set_exti(bsp_io_callback_t * _callback, EXTITrigger_TypeDef _trigger)
{
    if (GPIOx != NULL)
    {
        EXTI_InitTypeDef EXTI_InitStructure;
        
        IRQn_Type irq_n = (GPIO_InitStruct.GPIO_Pin <= 4) ? (IRQn_Type)(EXTI0_IRQn + GPIO_InitStruct.GPIO_Pin) :
                          (GPIO_InitStruct.GPIO_Pin < 10) ? EXTI9_5_IRQn : EXTI15_10_IRQn;

        callback[GPIO_InitStruct.GPIO_Pin] = _callback;
        
        if (_callback == NULL)
        {
            EXTI_InitStructure.EXTI_Line = (1 << GPIO_InitStruct.GPIO_Pin);
            EXTI_InitStructure.EXTI_LineCmd = DISABLE;
            EXTI_Init(&EXTI_InitStructure);
        }
        else
        {
            uint8_t port_src = ((GPIOx == GPIOA) ? 0 :
                                (GPIOx == GPIOB) ? 1 :
                                (GPIOx == GPIOC) ? 2 :
                                (GPIOx == GPIOD) ? 3 :
                                (GPIOx == GPIOE) ? 4 :
                                (GPIOx == GPIOF) ? 5 :
                                                   6);

            GPIO_EXTILineConfig(port_src, GPIO_InitStruct.GPIO_Pin);
            EXTI_InitStructure.EXTI_Line = 1 << GPIO_InitStruct.GPIO_Pin;
            EXTI_InitStructure.EXTI_Trigger = _trigger;
            EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
            EXTI_InitStructure.EXTI_LineCmd = ENABLE;
            EXTI_Init(&EXTI_InitStructure);
            
            NVIC_EnableIRQ(irq_n);
        }
    }
};

/**
 *  @brief   ����� ����� � ���������� EXTI
 *  @details ������ ���������� ����������, ������������� ����� ������� bsp_io
 */
static void EXTI_Handler(void)
{
    for (uint8_t i = 0; i < 16; i++)
    {
        uint32_t line = 1 << i;
        
        if (EXTI_GetITStatus(line) != RESET)
        {
            if (bsp_io::callback[i] != NULL)
            {
                bsp_io::callback[i](line);
            }
            
            EXTI_ClearITPendingBit(line);
        }
    }
}

/// ���������� �� ����� EXTI0 (������)
extern "C" void EXTI0_IRQHandler(void)
{
    EXTI_Handler();
};

/// ���������� �� ����� EXTI1 (������)
extern "C" void EXTI1_IRQHandler(void)
{
    EXTI_Handler();
};

/// ���������� �� ����� EXTI2 (������)
extern "C" void EXTI2_IRQHandler(void)
{
    EXTI_Handler();
};

/// ���������� �� ����� EXTI3 (������)
extern "C" void EXTI3_IRQHandler(void)
{
    EXTI_Handler();
};

/// ���������� �� ����� EXTI4 (������)
extern "C" void EXTI4_IRQHandler(void)
{
    EXTI_Handler();
};

/// ���������� �� ����� EXTI5..EXTI9 (������)
extern "C" void EXTI9_5_IRQHandler(void)
{
    EXTI_Handler();
};

/// ���������� �� ����� EXTI10..EXTI15 (������)
extern "C" void EXTI15_10_IRQHandler(void)
{
    EXTI_Handler();
};
