#ifndef _BSP_CON_N_
#define _BSP_CON_N_


#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#warning костыль! Иначе не видит тип USART_InitTypeDef
#include "stm32f10x_usart.h"

typedef void(bsp_con_rx_handler_t)(char *buf, const uint8_t size);

void bsp_con_init(bsp_con_rx_handler_t * con_rx_handler);
bool bsp_con_send(const char *buf);
USART_InitTypeDef *bsp_con_get_setting(void);

#endif  /* _BSP_CON_N_ */ 
