//------------------------------------------------------------------------------
// Общие типы для всех интерфейсов
//------------------------------------------------------------------------------

#ifndef _IFACES_TYPES_H_
#define _IFACES_TYPES_H_

#include "ifaces_config.h"

#define MSG_LEN_STANDARD 8

typedef enum
{
    #if (CAN1_ENABLED == 1)
        IFACE_CAN1,
    #endif
    #if (CAN2_ENABLED == 1)
        IFACE_CAN2,
    #endif
    #if (LIN1_ENABLED == 1)
        IFACE_UART1,
        IFACE_LIN1,
    #endif
    #if (LIN2_ENABLED == 1)
        IFACE_UART2,
        IFACE_LIN2,
    #endif
    IFACE_CON
} iface_type_t;

typedef enum
{
    IFACE_RX,
    IFACE_TX
} iface_direct_t;

typedef uint32_t specific_setting_t;

typedef struct
{
    bool enabled;
    uint32_t baudrate;
    specific_setting_t sett;
} iface_setting_t;

typedef uint32_t timestamp_t;

typedef uint64_t(*timestamp_callback_t)(void);

typedef struct
{
    timestamp_t timestamp;
    iface_direct_t direct;
    uint32_t id;
    uint8_t len;
    union
    {
        uint8_t bytes[MSG_LEN_STANDARD];
        uint8_t * ptr;
    } data;
    uint16_t err_code;
    specific_setting_t sett;
}msg_t;

typedef void (*iface_reciver_callback_t)(msg_t *);

#endif /* _IFACES_TYPES_H_ */
