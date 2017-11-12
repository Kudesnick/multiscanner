/**
 *  @file iface.h
 *
 *  @brief типы для работы с сообщениями
 *  @details перечислены типы и имена интерфейсов а также все типы сообщений
 *           возвращаемые коды ошибок
 */
 
#ifndef _IFACE_
#define _IFACE_

/// Тип интерфейса.
typedef enum : uint8_t
{
    IFACE_TYPE_DEF,     ///< Умолчательный тип. На практике не применяется. Его использование говорит об ошибке в программе
    IFACE_TYPE_CAN,     ///< CAN интерфейс. Среда передачи определяется внешним драйвером.
    IFACE_TYPE_UART,    ///< UART. Канальный уровень без надстроек.
    IFACE_TYPE_LIN,     ///< LIN. Использует физический модуль USART.
    IFACE_TYPE_CON,     ///< Интерфейс консоли. Прикладной уровень.
} iface_type_t;

/// Имена конкретных интерфейсов, доступных в системе
typedef enum : uint8_t
{
    IFACE_NAME_DEF,    ///< Имя по умолчанию. Его использование говорит об ошибке в программе
    IFACE_NAME_CAN1,
    IFACE_NAME_CAN2,
    IFACE_NAME_UART1,
    IFACE_NAME_UART2,
    IFACE_NAME_LIN1,
    IFACE_NAME_LIN2,
    IFACE_NAME_CON,
} iface_name_t;

/// Направление передачи сообщения
typedef enum : uint8_t
{
    MSG_TX,
    MSG_RX
} msg_direction_t;

/// Заголовок сообщения, универсальный для всех интерфейсов
struct msg_header_t
{
    iface_type_t msg_type;  ///< Тип сообщения
    iface_name_t route;     ///< В какой интерфейс передаем
    msg_direction_t direct; ///< Направление передачи
     uint8_t len;           ///< Истинная длина сообщения
    uint64_t time;          ///< метка времени
    uint64_t interval;      ///< Интервал для повторений
    uint16_t count;         ///< Счетчик повторений
};

// Структуры тела сообщения CAN
//-----------------------------

/// Причины завершения фрейма CAN-сообщения
typedef enum : uint8_t
{
    MSG_BRK_CAN_COMPLETE,       ///< Сообщение принято без ошибок
    MSG_BRK_CAN_TIMEOUT,        ///< Передача сообщения прервана по таймауту
#warning добавить все возможные типы ошибок для интерфейса CAN
} msg_brk_reason_can_t;

typedef enum : uint8_t
{
    CAN_TYPE_STD,
    CAN_TYPE_EXT,
} can_msg_type_t;

typedef uint32_t can_id_t;

#define CAN_DATA_LEN_MAX 8

typedef struct : msg_header_t
{
    can_id_t id;
    can_msg_type_t type;
    uint8_t data[CAN_DATA_LEN_MAX];
    msg_brk_reason_can_t reason; ///< Причина окончания сообщения (или ошибка отправки)
} msg_can_t;

// Структуры тела сообщения LIN
//-----------------------------

/// Причины завершения фрейма LIN-сообщения
typedef enum : uint8_t
{
    MSG_BRK_LIN_COMPLETE,       ///< Сообщение принято без ошибок
    MSG_BRK_LIN_TIMEOUT,        ///< Передача сообщения прервана по таймауту
#warning добавить все возможные типы ошибок для интерфейса LIN
} msg_brk_reason_lin_t;

typedef enum : uint8_t
{
    LIN_TYPE_V1,
    LIN_TYPE_V2,
} lin_msg_type_t;

#define LIN_DATA_LEN_MAX 8

typedef struct : msg_header_t
{
    uint8_t id;
    can_msg_type_t type;
    uint8_t data[LIN_DATA_LEN_MAX];
    uint8_t crc;
    msg_brk_reason_lin_t reason; ///< Причина окончания сообщения (или ошибка отправки)
} msg_lin_t;

// Структуры тела сообщения UART
//------------------------------

/// Причины завершения фрейма UART-сообщения
typedef enum : uint8_t
{
    MSG_BRK_UART_TIMEOUT,     ///< Завершение по таймауту
    MSG_BRK_UART_LAST_ID,     ///< Детектирован байт завершения
    MSG_BRK_UART_FIRST_ID,    ///< Детектирован байт начала следующей посылки
    MSG_BRK_UART_LENGTH,      ///< Достигнута фиксированная длина посылки SERIAL_MSG_LEN_MAX
    MSG_BRK_UART_OVF,         ///< Превышена длина буфера сообщения
    MSG_BRK_UART_FRAME_ERR,   ///< Ошибка фрейма
    MSG_BRK_UART_PARITY_ERR,  ///< Ошибка паритета
} msg_brk_reason_uart_t;

#define UART_DATA_LEN_MAX 16

typedef struct : msg_header_t
{
    uint8_t data[UART_DATA_LEN_MAX];
    msg_brk_reason_lin_t reason; ///< Причина окончания сообщения (или ошибка отправки)
} msg_uart_t;

/// Структура сообщения универсальная
//-----------------------------------
typedef union
{
    ms_can_t  can;
    ms_lin_t  lin;
    ms_uart_t uart;
}
} msg_t;

#endif /* _IFACE_ */
