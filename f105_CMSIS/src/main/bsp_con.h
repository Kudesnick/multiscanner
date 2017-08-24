#ifndef _BSP_CON_N_
#define _BSP_CON_N_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "fifo_con.h"
#include "bsp_usart.h"

typedef struct
{
    uint32_t baudrate;
    uint16_t parity;
    uint16_t stop_bits;
        bool echo;
} bsp_con_config_t;

class bsp_con : private bsp_usart
{
    private:
        fifo_con * bufer;
        static bsp_con_config_t default_sett;
        void bsp_usart_callback(uint16_t data, uint16_t flags);
        bsp_con_config_t setting;
    protected:
    public:
        bsp_con(USART_TypeDef *_unit_ptr, fifo_con * buf, bsp_con_config_t * _setting = &default_sett);
        bool send(const char *buf);
        bsp_con_config_t *get_setting(void);
        void set_setting(bsp_con_config_t * sett);
};

#endif  /* _BSP_CON_N_ */ 
