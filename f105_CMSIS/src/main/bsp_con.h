#ifndef _BSP_CON_N_
#define _BSP_CON_N_


#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef void(bsp_con_rx_handler_t)(char *buf, const uint8_t size);
typedef struct
{
    uint32_t baudrate;
    uint16_t parity;
    uint16_t stop_bits;
} bsp_con_config_t;

void bsp_con_init(bsp_con_rx_handler_t * con_rx_handler);
bool bsp_con_send(const char *buf);
bsp_con_config_t *bsp_con_get_setting(void);

#endif  /* _BSP_CON_N_ */ 
