#ifndef _BSP_CON_N_
#define _BSP_CON_N_


#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "fifo_con.h"

typedef void(bsp_con_rx_handler_t)(char *buf, const uint8_t size);

typedef struct
{
    uint32_t baudrate;
    uint16_t parity;
    uint16_t stop_bits;
        bool echo;
} bsp_con_config_t;

void bsp_con_init(fifo_con * buf);
bool bsp_con_send(const char *buf);
bsp_con_config_t *bsp_con_get_setting(void);

#endif  /* _BSP_CON_N_ */ 
