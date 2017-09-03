/********************************************************************
 * cpp_iface класс интерфейса
 ********************************************************************/

#ifndef _IFACE_
#define _IFACE_

typedef enum
{
    IFACE_TYPE_DEF,
    IFACE_TYPE_CAN,
    IFACE_TYPE_UART,
    IFACE_TYPE_LIN,
    IFACE_TYPE_CON,
} iface_type_t;

typedef enum
{
    IFACE_NAME_DEF,
    IFACE_NAME_CAN1,
    IFACE_NAME_CAN2,
    IFACE_NAME_UART1,
    IFACE_NAME_UART2,
    IFACE_NAME_LIN1,
    IFACE_NAME_LIN2,
    IFACE_NAME_CON,
} iface_name_t;

#endif /* _IFACE_ */
