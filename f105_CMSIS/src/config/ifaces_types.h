//------------------------------------------------------------------------------
// Общие типы для всех интерфейсов
//------------------------------------------------------------------------------

#ifndef _IFACES_TYPES_H_
#define _IFACES_TYPES_H_

#include "ifaces_config.h"

#define MSG_LEN_MAX 9

typedef enum
{
    IFACE_TYPE_CAN,
    IFACE_TYPE_UART,
    IFACE_TYPE_LIN,
    IFACE_TYPE_CON
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
