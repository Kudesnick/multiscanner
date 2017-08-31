/********************************************************************
 * cpp_iface класс интерфейса
 ********************************************************************/

#ifndef _IFACE_
#define _IFACE_

#include "stdint.h"
#include "fifo.h"

#define MSG_DATA_LEN_MAX 9
#define RX_BUFFER_SIZE 8 // Константа длины обязательно должна быть степенью двойки
#define TX_BUFFER_SIZE 8 // Константа длины обязательно должна быть степенью двойки

typedef enum
{
    IFACE_TYPE_CAN,
    IFACE_TYPE_UART,
    IFACE_TYPE_LIN,
    IFACE_TYPE_CON,
} iface_type_t;

typedef enum
{
    IFACE_NAME_CAN1,
    IFACE_NAME_CAN2,
    IFACE_NAME_UART1,
    IFACE_NAME_UART2,
    IFACE_NAME_LIN1,
    IFACE_NAME_LIN2,
    IFACE_NAME_CON,
} iface_name_t;

#define iface_name_to_type(t) (\
    (t == IFACE_NAME_CAN1 ) ? IFACE_TYPE_CAN :\
    (t == IFACE_NAME_CAN2 ) ? IFACE_TYPE_CAN :\
    (t == IFACE_NAME_UART1) ? IFACE_TYPE_UART :\
    (t == IFACE_NAME_UART2) ? IFACE_TYPE_UART :\
    (t == IFACE_NAME_LIN1 ) ? IFACE_TYPE_LIN :\
    (t == IFACE_NAME_LIN2 ) ? IFACE_TYPE_LIN :\
    (t == IFACE_NAME_CON  ) ? IFACE_TYPE_CON :\
    -1\
)

typedef enum
{
    MSG_RX,
    MSG_TX,
} msg_direct_t;

typedef union
{
    struct
    {
        uint32_t baudrate;
        uint8_t  phase_seg1;
        uint8_t  phase_seg2;
        uint8_t  sjw_range;
        bool     silent_mode;
        bool     echo;
    } can;
    struct
    {
        uint32_t baudrate;
        uint16_t word_length;
        uint16_t stop_bits;
        uint16_t parity;
        uint32_t timeout;
        bool     echo;
    } uart;
    struct
    {
        uint32_t baudrate;
        uint16_t break_len;
        bool     autobaud;
        uint32_t timeout;
        bool     echo;
        
    } lin;
    struct
    {
        uint32_t baudrate;
        uint16_t stop_bits;
        uint16_t parity;
        bool     echo;
    } con;
} iface_sett_t;

typedef struct
{
    iface_name_t iface,
    msg_direct_t direct,
    uint32_t     timestamp,
    uint32_t     id,
    uint8_t      len,
    uint8_t      data[MSG_DATA_LEN_MAX],
    union
    {
        struct
        {
            uint32_t ext_id:1;
            uint32_t ask:1;
            uint32_t fault:1;
        } can;
        struct
        {
            uint32_t parity_err:MSG_DATA_LEN_MAX+1;
            uint32_t tout_err:1;
        } uart;
        struct
        {
            uint32_t ext_id:1;
            uint32_t id_err:1;
            uint32_t chksm_err:1;
            uint32_t tout_err:1;
        } lin;
    } flags;
} msg_t;

#ifdef __cplusplus

class cpp_iface
{
    protected:
        iface_name_t name;
        iface_sett_t sett;
        iface_type_t type;
        FIFO_T(msg_t, RX_BUFFER_SIZE) rx_buffer;
        FIFO_T(msg_t, TX_BUFFER_SIZE) tx_buffer;
    public:
        cpp_iface(iface_name_t i_name);
        void init(iface_sett_t *i_sett);
        int32_t send_msg(msg_t *i_msg);
        msg_t *get_msg(void);
        void clear_rx_buf(void);
        void clear_tx_buf(void);
        iface_name_t get_name(void);
        iface_type_t get_type(void);
};

class cpp_iface_can : public cpp_iface
{
    public:
        cpp_iface_can(iface_name_t i_name);
        void init(iface_sett_t *i_sett);
        int32_t send_msg(msg_t *i_msg);
        msg_t *get_msg(void);
};

class cpp_iface_uart : public cpp_iface
{
    public:
        cpp_iface_uart(iface_name_t i_name);
        void init(iface_sett_t *i_sett);
        int32_t send_msg(msg_t *i_msg);
        msg_t *get_msg(void);
};

class cpp_iface_lin : public cpp_iface
{
    public:
        cpp_iface_lin(iface_name_t i_name);
        void init(iface_sett_t *i_sett);
        int32_t send_msg(msg_t *i_msg);
        msg_t *get_msg(void);
};

#endif /* __cplusplus */

#endif /* _IFACE_ */
