#include "bsp_usart.h"


bsp_usart::bsp_usart(USART_TypeDef *unit, void (*clbck)(USART_TypeDef *unit)):
    bsp_unit((void *)unit, (void(*)(void))clbck)
{
    switch ((uint32_t)unit)
    {
        case (uint32_t)USART1:
        {
            #if (RTE_USART1 > 0)
                volatile bsp_io pin = bsp_io(RTE_USART1_RX_PORT_DEF, RTE_USART1_RX_BIT_DEF, GPIO_Mode_IPU, true);
                                pin = bsp_io(RTE_USART1_TX_PORT_DEF, RTE_USART1_TX_BIT_DEF, GPIO_Mode_AF_PP);
            #endif
            
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
            NVIC_EnableIRQ(USART1_IRQn);
            
            break;
        }
        case (uint32_t)USART2:
        {
            #if (RTE_USART2 > 0)
                volatile bsp_io pin = bsp_io(RTE_USART2_RX_PORT_DEF, RTE_USART2_RX_BIT_DEF, GPIO_Mode_IPU, true);
                                pin = bsp_io(RTE_USART2_TX_PORT_DEF, RTE_USART2_TX_BIT_DEF, GPIO_Mode_AF_PP);
            #endif
            
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
            NVIC_EnableIRQ(USART2_IRQn);
            
            break;
        }
        case (uint32_t)USART3:
        {
            #if (RTE_USART3 > 0)
                volatile bsp_io pin = bsp_io(RTE_USART3_RX_PORT_DEF, RTE_USART3_RX_BIT_DEF, GPIO_Mode_IPU, true);
                                pin = bsp_io(RTE_USART3_TX_PORT_DEF, RTE_USART3_TX_BIT_DEF, GPIO_Mode_AF_PP);
            #endif
            
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
            NVIC_EnableIRQ(USART3_IRQn);
            
            break;
        }
        case (uint32_t)UART4:
        {
            #if (RTE_UART4 > 0)
                volatile bsp_io pin = bsp_io(RTE_UART4_RX_PORT_DEF, RTE_UART4_RX_BIT_DEF, GPIO_Mode_IPU, true);
                                pin = bsp_io(RTE_UART4_TX_PORT_DEF, RTE_UART4_TX_BIT_DEF, GPIO_Mode_AF_PP);
            #endif
            
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
            NVIC_EnableIRQ(UART4_IRQn);
            
            break;
        }
        case (uint32_t)UART5:
        {
            #if (RTE_UART5 > 0)
                volatile bsp_io pin = bsp_io(RTE_UART5_RX_PORT_DEF, RTE_UART5_RX_BIT_DEF, GPIO_Mode_IPU, true);
                                pin = bsp_io(RTE_UART5_TX_PORT_DEF, RTE_UART5_TX_BIT_DEF, GPIO_Mode_AF_PP);
            #endif
            
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
            NVIC_EnableIRQ(UART5_IRQn);
            
            break;
        }
    }
};
    
void bsp_usart::send_sett(bsp_usart_setting_t *sett)
{
    
};

void *bsp_usart::get_sett(void)
{
    
};

bool bsp_usart::send_msg(uint16_t data)
{
    
};

void bsp_usart::interrupt_handler(void)
{
    
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
