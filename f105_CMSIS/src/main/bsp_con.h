#ifndef _BSP_CON_N_
#define _BSP_CON_N_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef void(bsp_con_rx_handler_t)(uint8_t *buf, uint8_t size);

void bsp_con_init(bsp_con_rx_handler_t * con_rx_handler);
bool bsp_con_send(uint8_t *buf, uint8_t size);

#endif  /* _BSP_CON_N_ */ 
