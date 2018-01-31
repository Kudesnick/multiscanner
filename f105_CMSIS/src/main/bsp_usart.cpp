

#include <typeinfo>

#include "RTE_Device.h"

#include "rtt.h"
#include "bsp_usart.h"

bsp_usart::bsp_usart(unit_t *_unit_ptr, iface_type_t _class_type, iface_name_t _object_name):
    bsp_unit(_unit_ptr, _class_type, _object_name),
    cpp_list<LIST_TYPE_UNIT_USART>(_class_type, _object_name),
    pin_rx(),
    pin_tx()
{
    RTT_CREATE_LOG;
    
    switch ((uint32_t)unit_ptr)
    {
        case (uint32_t)USART1:
        {
            #if (RTE_USART1 > 0)
                pin_rx.init(RTE_USART1_RX_PORT_DEF, GPIO_Pin(RTE_USART1_RX_BIT_DEF), GPIO_Mode_IPU, true);
                pin_rx.init(RTE_USART1_TX_PORT_DEF, GPIO_Pin(RTE_USART1_TX_BIT_DEF), GPIO_Mode_AF_PP);
            #endif
            
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
            NVIC_EnableIRQ(USART1_IRQn);
            
            break;
        }
        case (uint32_t)USART2:
        {
            #if (RTE_USART2 > 0)
                pin_rx.init(RTE_USART2_RX_PORT_DEF, GPIO_Pin(RTE_USART2_RX_BIT_DEF), GPIO_Mode_IPU, true);
                pin_rx.init(RTE_USART2_TX_PORT_DEF, GPIO_Pin(RTE_USART2_TX_BIT_DEF), GPIO_Mode_AF_PP, false);
            #endif
            
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
            NVIC_EnableIRQ(USART2_IRQn);
            
            break;
        }
        case (uint32_t)USART3:
        {
            #if (RTE_USART3 > 0)
                pin_rx.init(RTE_USART3_RX_PORT_DEF, GPIO_Pin(RTE_USART3_RX_BIT_DEF), GPIO_Mode_IPU, true);
                pin_rx.init(RTE_USART3_TX_PORT_DEF, GPIO_Pin(RTE_USART3_TX_BIT_DEF), GPIO_Mode_AF_PP);
            #endif
            
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
            NVIC_EnableIRQ(USART3_IRQn);
            
            break;
        }
        case (uint32_t)UART4:
        {
            #if (RTE_UART4 > 0)
                pin_rx.init(RTE_UART4_RX_PORT_DEF, GPIO_Pin(RTE_UART4_RX_BIT_DEF), GPIO_Mode_IPU, true);
                pin_rx.init(RTE_UART4_TX_PORT_DEF, GPIO_Pin(RTE_UART4_TX_BIT_DEF), GPIO_Mode_AF_PP);
            #endif
            
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
            NVIC_EnableIRQ(UART4_IRQn);
            
            break;
        }
        case (uint32_t)UART5:
        {
            #if (RTE_UART5 > 0)
                pin_rx.init(RTE_UART5_RX_PORT_DEF, GPIO_Pin(RTE_UART5_RX_BIT_DEF), GPIO_Mode_IPU, true);
                pin_rx.init(RTE_UART5_TX_PORT_DEF, GPIO_Pin(RTE_UART5_TX_BIT_DEF), GPIO_Mode_AF_PP);
            #endif
            
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
            NVIC_EnableIRQ(UART5_IRQn);
            
            break;
        }
    }
};

void bsp_usart::send_sett(settings_t *sett)
{
    setting = *sett;
    
    // Если модуль включен, то настраиваем его под себя
    if (setting.USART_Enable)
    { 

        USART_InitTypeDef usart_init_struct =
        {
            /*.USART_BaudRate            = */setting.USART_BaudRate,
            /*.USART_WordLength          = */setting.USART_WordLength,
            /*.USART_StopBits            = */setting.USART_StopBits,
            /*.USART_Parity              = */setting.USART_Parity,
            /*.USART_Mode                = */setting.USART_Mode,
            /*.USART_HardwareFlowControl = */USART_HardwareFlowControl_None,
        };
    
        USART_ITConfig(unit_ptr, USART_IT_TXE, DISABLE);
        USART_ClearITPendingBit(unit_ptr, USART_IT_TC);
        USART_ITConfig(unit_ptr, USART_IT_TC, DISABLE);

        USART_ClearITPendingBit((USART_TypeDef *)unit_ptr, USART_IT_RXNE);
        USART_ITConfig(unit_ptr, USART_IT_RXNE, ENABLE);
        USART_ITConfig(unit_ptr, USART_IT_IDLE, ENABLE);
        USART_ClearITPendingBit(unit_ptr, USART_IT_LBD);
        USART_ITConfig(unit_ptr, USART_IT_LBD, (setting.USART_LIN_Enable) ? ENABLE : DISABLE);
        USART_ITConfig(unit_ptr, USART_IT_PE, ENABLE);
        USART_ITConfig(unit_ptr, USART_IT_ERR, ENABLE);
    
        USART_OverSampling8Cmd(unit_ptr, ENABLE);
        USART_Init(unit_ptr, &usart_init_struct);
        USART_LINBreakDetectLengthConfig(unit_ptr, setting.USART_LIN_Break_Detection_Length);
        USART_LINCmd(unit_ptr, (setting.USART_LIN_Enable == true) ? ENABLE : DISABLE);
        
        USART_Cmd(unit_ptr, ENABLE);
        
        switch ((uint32_t)unit_ptr)
        {
            case (uint32_t)USART1: NVIC_EnableIRQ(USART1_IRQn); break;
            case (uint32_t)USART2: NVIC_EnableIRQ(USART2_IRQn); break;
            case (uint32_t)USART3: NVIC_EnableIRQ(USART3_IRQn); break;
            case (uint32_t)UART4:  NVIC_EnableIRQ(UART4_IRQn);  break;
            case (uint32_t)UART5:  NVIC_EnableIRQ(UART5_IRQn);  break;
        }
    }
    // Если модуль не включен, то пытаемся выключить аппаратный модуль
    else
    { 
        bsp_usart* ptr = (bsp_usart *)bsp_usart::cpp_list<LIST_TYPE_UNIT_USART>::get_last_pointer();
        bool this_unit_is_used = false;
        
        while (ptr != NULL && !this_unit_is_used)
        {
            if (   ptr->unit_ptr == unit_ptr
                && ptr->get_sett()->USART_Enable)
            {
                this_unit_is_used = true;
            }
            
            ptr = (bsp_usart *)ptr->cpp_list<LIST_TYPE_UNIT_USART>::get_prev_pointer();
        }

        if (!this_unit_is_used)
        {
            USART_Cmd(unit_ptr, DISABLE);
        }
    }
};

settings_t *bsp_usart::get_sett(void)
{
    static settings_t sett;
    sett = setting;
    return &sett;
};

bool bsp_usart::send_msg(message_t * msg)
{
    bool result = false;
    
    if (USART_GetFlagStatus(unit_ptr, USART_FLAG_TXE))
    {
        USART_SendData(unit_ptr, *msg);
        USART_ITConfig(unit_ptr, USART_IT_TXE, ENABLE);
        result = true;
    }
    
    return result;
};

message_t * bsp_usart::get_msg(void)
{
    return (clbk_data.data == USART_NULL_DATA) ? NULL : &clbk_data.data;
};

void bsp_usart::callback(void)
{
    clbk_data.data = USART_NULL_DATA;
    clbk_data.flags = 0;

    if (USART_GetITStatus(unit_ptr, USART_IT_TXE))
    {
        clbk_data.flags |= USART_FLAG_TXE;
        USART_ITConfig(unit_ptr, USART_IT_TXE, DISABLE);
    }
    if (USART_GetITStatus(unit_ptr, USART_IT_TC))
    {
        clbk_data.flags |= USART_FLAG_TC;
        USART_ITConfig(unit_ptr, USART_IT_TC, DISABLE);
        USART_ClearITPendingBit(unit_ptr, USART_IT_TC);
    }
    if (USART_GetITStatus(unit_ptr, USART_IT_PE))
    {
        clbk_data.flags |= USART_FLAG_PE;
    }
    if (USART_GetITStatus(unit_ptr, USART_IT_ORE))
    {
        clbk_data.flags |= USART_FLAG_ORE;
    }
    if (USART_GetITStatus(unit_ptr, USART_IT_NE))
    {
        clbk_data.flags |= USART_FLAG_NE;
    }
    if (USART_GetITStatus(unit_ptr, USART_IT_FE))
    {
        clbk_data.flags |= USART_FLAG_FE;
    }
    if (USART_GetITStatus(unit_ptr, USART_IT_IDLE))
    {
        clbk_data.flags |= USART_FLAG_IDLE;
        USART_ReceiveData(unit_ptr);
        clbk_data.data = USART_IDLE_DATA;
    }
    if (USART_GetITStatus(unit_ptr, USART_IT_RXNE))
    {
        clbk_data.flags |= USART_FLAG_RXNE;
        clbk_data.data = USART_ReceiveData(unit_ptr);
        USART_ClearITPendingBit(unit_ptr, USART_IT_RXNE);
    }
    if (USART_GetITStatus(unit_ptr, USART_IT_LBD))
    {
        clbk_data.flags |= USART_IT_LBD;
        USART_ReceiveData(unit_ptr);
        clbk_data.data = USART_LIN_BRK_DATA;
        USART_ClearITPendingBit(unit_ptr, USART_IT_LBD);
    }
};

uint32_t bsp_usart::round_baud(settings_t *sett)
{
        // Расчет истинного бодрейта
    RCC_ClocksTypeDef RCC_ClocksStatus;
    RCC_GetClocksFreq(&RCC_ClocksStatus);
    uint32_t freq = (unit_ptr == USART1) ? RCC_ClocksStatus.PCLK2_Frequency : RCC_ClocksStatus.PCLK1_Frequency;
    freq <<= 1; // Умножаем на два, потому что включаем USART_OverSampling8Cmd((USART_TypeDef *)unit_ptr, ENABLE);
    uint16_t brr = freq / sett->USART_BaudRate;
    return freq / brr;
};

static bool irq_handler(unit_t *unit)
{
    bsp_usart* ptr = (bsp_usart *)bsp_usart::cpp_list<LIST_TYPE_UNIT_USART>::get_last_pointer();
    bool this_unit_is_used = false;
    
    while (ptr != NULL && !this_unit_is_used)
    {
        if (   ptr->unit_ptr == unit
            && ptr->get_sett()->USART_Enable)
        {
            this_unit_is_used = true;
            
            ptr->callback();
        }
        
        ptr = (bsp_usart *)ptr->cpp_list<LIST_TYPE_UNIT_USART>::get_prev_pointer();
    }

    return this_unit_is_used;
}

// Прерывания от интерфейсов uart
//------------------------------------------------------------------------------
extern "C" void USART1_IRQHandler(void)
{
    if (!irq_handler(USART1))
    {
        NVIC_DisableIRQ(USART1_IRQn);
    }
};

extern "C" void USART2_IRQHandler(void)
{
    if (!irq_handler(USART2))
    {
        NVIC_DisableIRQ(USART2_IRQn);
    }
};

extern "C" void USART3_IRQHandler(void)
{
    if (!irq_handler(USART3))
    {
        NVIC_DisableIRQ(USART3_IRQn);
    }
};

extern "C" void UART4_IRQHandler(void)
{
    if (!irq_handler(UART4))
    {
        NVIC_DisableIRQ(UART4_IRQn);
    }
};

extern "C" void UART5_IRQHandler(void)
{
    if (!irq_handler(UART5))
    {
        NVIC_DisableIRQ(UART5_IRQn);
    }
};
