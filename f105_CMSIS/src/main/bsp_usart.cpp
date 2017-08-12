#include "RTE_Device.h"

#include "bsp_usart.h"

bsp_usart::bsp_usart(USART_TypeDef *_unit_ptr, bsp_usart_callback_t *_callback):
    bsp_unit((void *)_unit_ptr, (bsp_unit_callback_t *)_callback)
{
    switch ((uint32_t)unit_ptr)
    {
        case (uint32_t)USART1:
        {
            #if (RTE_USART1 > 0)
                pin_rx = new bsp_io(RTE_USART1_RX_PORT_DEF, GPIO_Pin(RTE_USART1_RX_BIT_DEF), GPIO_Mode_IPU, true);
                pin_tx = new bsp_io(RTE_USART1_TX_PORT_DEF, GPIO_Pin(RTE_USART1_TX_BIT_DEF), GPIO_Mode_AF_PP);
            #endif
            
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
            NVIC_EnableIRQ(USART1_IRQn);
            
            break;
        }
        case (uint32_t)USART2:
        {
            #if (RTE_USART2 > 0)
                pin_rx = new bsp_io(RTE_USART2_RX_PORT_DEF, GPIO_Pin(RTE_USART2_RX_BIT_DEF), GPIO_Mode_IPU, true);
                pin_tx = new bsp_io(RTE_USART2_TX_PORT_DEF, GPIO_Pin(RTE_USART2_TX_BIT_DEF), GPIO_Mode_AF_PP, false);
            #endif
            
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
            NVIC_EnableIRQ(USART2_IRQn);
            
            break;
        }
        case (uint32_t)USART3:
        {
            #if (RTE_USART3 > 0)
                pin_rx = new bsp_io(RTE_USART3_RX_PORT_DEF, GPIO_Pin(RTE_USART3_RX_BIT_DEF), GPIO_Mode_IPU, true);
                pin_tx = new bsp_io(RTE_USART3_TX_PORT_DEF, GPIO_Pin(RTE_USART3_TX_BIT_DEF), GPIO_Mode_AF_PP);
            #endif
            
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
            NVIC_EnableIRQ(USART3_IRQn);
            
            break;
        }
        case (uint32_t)UART4:
        {
            #if (RTE_UART4 > 0)
                pin_rx = new bsp_io(RTE_UART4_RX_PORT_DEF, GPIO_Pin(RTE_UART4_RX_BIT_DEF), GPIO_Mode_IPU, true);
                pin_tx = new bsp_io(RTE_UART4_TX_PORT_DEF, GPIO_Pin(RTE_UART4_TX_BIT_DEF), GPIO_Mode_AF_PP);
            #endif
            
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
            NVIC_EnableIRQ(UART4_IRQn);
            
            break;
        }
        case (uint32_t)UART5:
        {
            #if (RTE_UART5 > 0)
                pin_rx = new bsp_io(RTE_UART5_RX_PORT_DEF, GPIO_Pin(RTE_UART5_RX_BIT_DEF), GPIO_Mode_IPU, true);
                pin_tx = new bsp_io(RTE_UART5_TX_PORT_DEF, GPIO_Pin(RTE_UART5_TX_BIT_DEF), GPIO_Mode_AF_PP);
            #endif
            
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
            NVIC_EnableIRQ(UART5_IRQn);
            
            break;
        }
    }
};
    
void bsp_usart::send_sett(void *sett)
{
    setting = *(bsp_usart_setting_t*)sett;
    
    USART_InitTypeDef usart_init_struct =
    {
        /*.USART_BaudRate            = */setting.USART_BaudRate,
        /*.USART_WordLength          = */setting.USART_WordLength,
        /*.USART_StopBits            = */setting.USART_StopBits,
        /*.USART_Parity              = */setting.USART_Parity,
        /*.USART_Mode                = */setting.USART_Mode,
        /*.USART_HardwareFlowControl = */USART_HardwareFlowControl_None,
    };

    USART_ITConfig((USART_TypeDef *)unit_ptr, USART_IT_TXE, DISABLE);
    USART_ClearITPendingBit((USART_TypeDef *)unit_ptr, USART_IT_TC);
    USART_ITConfig((USART_TypeDef *)unit_ptr, USART_IT_TC, DISABLE);

    USART_ClearITPendingBit((USART_TypeDef *)unit_ptr, USART_IT_RXNE);
    USART_ITConfig((USART_TypeDef *)unit_ptr, USART_IT_RXNE, ENABLE);
    USART_ITConfig((USART_TypeDef *)unit_ptr, USART_IT_IDLE, DISABLE);
    USART_ClearITPendingBit((USART_TypeDef *)unit_ptr, USART_IT_LBD);
    USART_ITConfig((USART_TypeDef *)unit_ptr, USART_IT_LBD, (setting.USART_LIN_Enable) ? ENABLE : DISABLE);
    USART_ITConfig((USART_TypeDef *)unit_ptr, USART_IT_PE, ENABLE);
    USART_ITConfig((USART_TypeDef *)unit_ptr, USART_IT_ERR, ENABLE);

    USART_Init((USART_TypeDef *)unit_ptr, &usart_init_struct);
    USART_LINBreakDetectLengthConfig((USART_TypeDef *)unit_ptr, setting.USART_LIN_Break_Detection_Length);
    USART_LINCmd((USART_TypeDef *)unit_ptr, (setting.USART_LIN_Enable == true) ? ENABLE : DISABLE);
    USART_Cmd((USART_TypeDef *)unit_ptr, (setting.USART_Enable == true) ? ENABLE : DISABLE);
};

void *bsp_usart::get_sett(void)
{
    static bsp_usart_setting_t sett;
    sett = setting;
    return (void *)&sett;
};

bool bsp_usart::send_msg(void* msg)
{
    bool result = false;
    
    if (USART_GetFlagStatus((USART_TypeDef *)unit_ptr, USART_FLAG_TXE))
    {
        USART_SendData((USART_TypeDef *)unit_ptr, *(bsp_usart_msg_t *)msg);
        USART_ITConfig((USART_TypeDef *)unit_ptr, USART_IT_TXE, ENABLE);
        result = true;
    }
    
    return result;
};

void bsp_usart::interrupt_handler(void)
{
    uint16_t data = 0;
    uint16_t flags = 0;
    
    if (USART_GetITStatus((USART_TypeDef *)unit_ptr, USART_IT_TXE))
    {
        flags |= USART_FLAG_TXE;
        USART_ITConfig((USART_TypeDef *)unit_ptr, USART_IT_TXE, DISABLE);
    };
    if (USART_GetITStatus((USART_TypeDef *)unit_ptr, USART_IT_TC))
    {
        flags |= USART_FLAG_TC;
        USART_ITConfig((USART_TypeDef *)unit_ptr, USART_IT_TC, DISABLE);
        USART_ClearITPendingBit((USART_TypeDef *)unit_ptr, USART_IT_TC);
    };

    if (USART_GetITStatus((USART_TypeDef *)unit_ptr, USART_IT_PE))
    {
        flags |= USART_FLAG_PE;
    }
#warning Тут надо расшифровать флаги ошибок (кошерно)
//    if (USART_GetITStatus((USART_TypeDef *)unit_ptr, USART_IT_ERR))
//    {
//        flags |= USART_FLAG_ERR;
//    }
    if (USART_GetITStatus((USART_TypeDef *)unit_ptr, USART_IT_IDLE))
    {
        flags |= USART_FLAG_IDLE;
        data = USART_IDLE_DATA;
    }
    if (USART_GetITStatus((USART_TypeDef *)unit_ptr, USART_IT_RXNE))
    {
        flags |= USART_FLAG_RXNE;
        data = USART_ReceiveData((USART_TypeDef *)unit_ptr);
        USART_ClearITPendingBit((USART_TypeDef *)unit_ptr, USART_IT_RXNE);
    }
#warning почему-то срабатывает в консоле, хотя LIN выключен и прерывание тоже
//    USART_GetITStatus((USART_TypeDef *)unit_ptr, USART_IT_LBD);
//    {
//        flags |= USART_IT_RXNE;
//        USART_ReceiveData((USART_TypeDef *)unit_ptr);
//        data = USART_LIN_BRK_DATA;
//        USART_ClearITPendingBit((USART_TypeDef *)unit_ptr, USART_IT_LBD);
//    }
    
    if (callback != NULL)
    {
        ((bsp_usart_callback_t *)(callback))(data, flags);
    }
};

// Прерывания от интерфейсов uart
//------------------------------------------------------------------------------
extern "C" void USART1_IRQHandler(void)
{
    bsp_usart *unit = (bsp_usart*)bsp_unit::object_search(USART1);
    if (unit != NULL)
    {
        unit->interrupt_handler();
    }
    else
    {
        NVIC_DisableIRQ(USART1_IRQn);
    }
};

extern "C" void USART2_IRQHandler(void)
{
    bsp_usart *unit = (bsp_usart*)bsp_unit::object_search(USART2);
    if (unit != NULL)
    {
        unit->interrupt_handler();
    }
    else
    {
        NVIC_DisableIRQ(USART2_IRQn);
    }
};

extern "C" void USART3_IRQHandler(void)
{
    bsp_usart *unit = (bsp_usart*)bsp_unit::object_search(USART3);
    if (unit != NULL)
    {
        unit->interrupt_handler();
    }
    else
    {
        NVIC_DisableIRQ(USART3_IRQn);
    }
};

extern "C" void UART4_IRQHandler(void)
{
    bsp_usart *unit = (bsp_usart*)bsp_unit::object_search(UART4);
    if (unit != NULL)
    {
        unit->interrupt_handler();
    }
    else
    {
        NVIC_DisableIRQ(UART4_IRQn);
    }
};

extern "C" void UART5_IRQHandler(void)
{
    bsp_usart *unit = (bsp_usart*)bsp_unit::object_search(UART5);
    if (unit != NULL)
    {
        unit->interrupt_handler();
    }
    else
    {
        NVIC_DisableIRQ(UART5_IRQn);
    }
};
