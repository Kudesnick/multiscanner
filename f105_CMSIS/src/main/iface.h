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

struct msg_t
{
    static const iface_type_t msg_type = IFACE_TYPE_DEF; // Тип сообщения
    iface_name_t route; // В какой интерфейс передаем
     uint8_t len;       // Истинная длина сообщения
    uint64_t time;      // метка времени
    uint64_t interval;  // Интервал для повторений
    uint16_t count;     // Счетчик повторений
    uint16_t reason;    // Причина окончания сообщения (или ошибка отправки)
};

// причины завершения фрейма
typedef enum
{
    SERIAL_MSG_BRK_TIMEOUT,     // Завершение по таймауту
    SERIAL_MSG_BRK_LAST_ID,     // Детектирован байт завершения
    SERIAL_MSG_BRK_FIRST_ID,    // Детектирован байт начала следующей посылки
    SERIAL_MSG_BRK_LENGTH,      // Достигнута фиксированная длина посылки SERIAL_MSG_LEN_MAX
    SERIAL_MSG_BRK_OVF,         // Превышена длина буфера сообщения
    SERIAL_MSG_BRK_FRAME_ERR,   // Ошибка фрейма
    SERIAL_MSG_BRK_PARITY_ERR,  // Ошибка паритета
} msg_serial_brk_reason_t;

struct msg_serial_t : msg_t
{
    static const iface_type_t msg_type = IFACE_TYPE_UART; // Тип сообщения
    uint8_t data[16]; // Данные
};

#endif /* _IFACE_ */
