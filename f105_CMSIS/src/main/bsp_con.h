#ifndef _BSP_CON_N_
#define _BSP_CON_N_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "bsp_io.h"
#include "fifo.h"

// Константы длины обязательно должны быть степенью двойки
#define RX_BUFFER_SIZE FIFO_SIZE_256
#define TX_BUFFER_SIZE FIFO_SIZE_256

typedef void(bsp_con_rx_handler_t)(char *buf, const uint8_t size);

typedef struct
{
    uint32_t baudrate;
    uint16_t parity;
    uint16_t stop_bits;
        bool echo;
} bsp_con_config_t;

typedef struct
{
    USART_TypeDef * unit;
    bsp_io         rx_pin;
    bsp_io         tx_pin;
} bsp_con_hw_config_t;

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
        bsp_con_hw_config_t hardware;
        cpp_fifo<char, RX_BUFFER_SIZE> rx_buffer;
        cpp_fifo<char, TX_BUFFER_SIZE> tx_buffer;
    public:
        cpp_con(bsp_con_hw_config_t hw, bsp_con_config_t setting, bsp_con_rx_handler_t *handler);
        void cpp_con_init(bsp_con_config_t *setting);
        bool bsp_con_send(const char *buf);
};

#endif  /* _BSP_CON_N_ */ 
