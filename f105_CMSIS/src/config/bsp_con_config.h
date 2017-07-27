//------------------------------------------------------------------------------
// Настройки USART консоли
//------------------------------------------------------------------------------

#ifndef _BSP_CON_CONFIG_H_
#define _BSP_CON_CONFIG_H_

#warning В последствии заменить значения на данные из RTE_Device.h

#define BSP_CON_UNIT            USART2

#define BSP_CON_BAUDRATE        9600
#define BSP_CON_PARITY          USART_Parity_No

#define BSP_CON_TX_PIN          GPIOA_02                
#define BSP_CON_RX_PIN          GPIOA_03

#endif /* _BSP_CON_CONFIG_H_ */
