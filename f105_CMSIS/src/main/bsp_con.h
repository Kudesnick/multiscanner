#ifndef _BSP_CON_N_
#define _BSP_CON_N_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "bsp_io.h"

typedef void(bsp_con_rx_handler_t)(char *buf, const uint8_t size);

typedef struct
{
    uint32_t baudrate;
    uint16_t parity;
    uint16_t stop_bits;
        bool echo;
} bsp_con_config_t;

class cpp_con
{
    private:
        static bsp_con_rx_handler_t *uart1_irq_handler;
        static bsp_con_rx_handler_t *uart2_irq_handler;
        static bsp_con_rx_handler_t *uart3_irq_handler;
        static bsp_con_rx_handler_t *uart4_irq_handler;
        static bsp_con_rx_handler_t *uart5_irq_handler;
    protected:
        bsp_con_config_t setting;
        cpp_pin rx_pin;
        cpp_pin tx_pin;
        
    public:
        
};

void bsp_con_init(bsp_con_rx_handler_t * con_rx_handler);
bool bsp_con_send(const char *buf);
bsp_con_config_t *bsp_con_get_setting(void);

#endif  /* _BSP_CON_N_ */ 
