//------------------------------------------------------------------------------
// Настройки USART консоли
//------------------------------------------------------------------------------

#ifndef _BSP_CON_CONFIG_H_
#define _BSP_CON_CONFIG_H_

#warning В последствии заменить значения на данные из RTE_Device.h

#define BSP_CON_UNIT            USART2
#define BSP_CON_UNIT_CLK        RCC_APB1Periph_USART2
#define BSP_CON_IRQn            USART2_IRQn
#define BSP_CON_IRQ_PRIORITY    0xF
#define bsp_con_handler         USART2_IRQHandler

#define BSP_CON_BAUDRATE        9600
#define BSP_CON_PARITY          USART_Parity_No

#define BSP_CON_TX_PORT         GPIOA                 
#define BSP_CON_TX_PIN          GPIO_Pin_2            
#define BSP_CON_TX_PORT_RCC     RCC_APB2Periph_GPIOA  

#define BSP_CON_RX_PORT         GPIOA                 
#define BSP_CON_RX_PIN          GPIO_Pin_3           
#define BSP_CON_RX_PORT_RCC     RCC_APB2Periph_GPIOA  

#endif /* _BSP_CON_CONFIG_H_ */
