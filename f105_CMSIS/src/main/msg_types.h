/**
 *  @file msg_types.h
 *
 *  @brief типы для работы с сообщениями
 *  @details перечислены все типы сообщений и возвращаемые коды ошибок
 */

#pragma once

#include <stdint.h>

#include "units_config.h"

#define CAN_DATA_LEN_MAX  FIFO_SIZE_8
#define LIN_DATA_LEN_MAX  FIFO_SIZE_8
#define UART_DATA_LEN_MAX FIFO_SIZE_16

#pragma pack(1) // пакуем все структуры в этом модуле. Они используются в буферах.
#pragma anon_unions

/// Направление передачи сообщения
typedef enum : uint8_t
{
    MSG_TX,
    MSG_RX
} msg_direction_t;

/// Заголовок сообщения, подготовленного для передачи, универсальный для всех интерфейсов
struct msg_header_t
{
    iface_type_t msg_type;  ///< Тип сообщения
    iface_name_t route;     ///< В какой интерфейс передаем
    msg_direction_t direct; ///< Направление передачи
    union
    {
        uint64_t rx_timestamp;
        struct
        {
            uint32_t counter;      ///< Отсчет времеи до следующей передачи
            uint16_t interval;     ///< Интервал для повторений
            uint16_t count;        ///< Счетчик повторений
        };
    };
    
};

// Структуры тела сообщения CAN
//-----------------------------

/// Причины завершения фрейма CAN-сообщения
typedef enum : uint8_t
{
    MSG_BRK_CAN_COMPLETE            , ///< Сообщение принято без ошибок
    MSG_BRK_CAN_TX_TIMEOUT          , ///< Передача сообщения прервана по таймауту
    MSG_BRK_CAN_TX_ERR              , ///< Неизвестная ошибка передачи
    MSG_BRK_CAN_RX_STUFF_ERR        ,  
    MSG_BRK_CAN_RX_FORM_ERR         ,  
    MSG_BRK_CAN_RX_ACK_ERR          ,  
    MSG_BRK_CAN_RX_BIT_RECESSIVE_ERR,
    MSG_BRK_CAN_RX_BIT_DOMINANT_ERR ,
    MSG_BRK_CAN_RX_CRC_ERR          ,
    MSG_BRK_CAN_RX_SOFTWARE_SET_ERR ,
} msg_brk_reason_can_t;

typedef enum : uint8_t
{
    CAN_TYPE_STD,
    CAN_TYPE_EXT,
} can_msg_type_t;

typedef uint32_t can_id_t;

typedef struct
{
    can_id_t id;
    can_msg_type_t type;
    uint8_t len;
    uint8_t data[CAN_DATA_LEN_MAX];
    msg_brk_reason_can_t reason; ///< Причина окончания сообщения (или ошибка отправки)
} msg_can_t;

// Структуры тела сообщения LIN
//-----------------------------

/// Причины завершения фрейма LIN-сообщения
typedef enum : uint8_t
{
    MSG_BRK_LIN_COMPLETE,       ///< Сообщение принято без ошибок
    MSG_BRK_LIN_TIMEOUT,        ///< Завершение по таймауту
    MSG_BRK_LIN_NON_BREAK,      ///< Не было импульса BRAKE перед началом передачи данных
    MSG_BRK_LIN_NON_SYNC,       ///< Не было синхронизирующей последовательности после импульса BRAKE
    MSG_BRK_LIN_ID_CHCK_ERR,    ///< Ошибка паритета ID пакета
    MSG_BRK_LIN_CHCK_ERR,       ///< Ошибка контрольной суммы пакета
    MSG_BRK_LIN_FRAME_ERR,      ///< Ошибка фрейма
} msg_brk_reason_lin_t;

typedef enum : uint8_t
{
    LIN_TYPE_V1,
    LIN_TYPE_V2,
} lin_msg_type_t;

typedef struct
{
    uint8_t id;
    can_msg_type_t type;
    uint8_t len;
    uint8_t data[LIN_DATA_LEN_MAX];
    uint8_t crc;
    msg_brk_reason_lin_t reason; ///< Причина окончания сообщения (или ошибка отправки)
} msg_lin_t;

// Структуры тела сообщения UART
//------------------------------

/// Причины завершения фрейма UART-сообщения
typedef enum : uint8_t
{
    MSG_BRK_UART_NA,
    MSG_BRK_UART_TIMEOUT,     ///< Завершение по таймауту
    MSG_BRK_UART_LAST_ID,     ///< Детектирован байт завершения
    MSG_BRK_UART_FIRST_ID,    ///< Детектирован байт начала следующей посылки
    MSG_BRK_UART_LENGTH,      ///< Достигнута фиксированная длина посылки SERIAL_MSG_LEN_MAX
    MSG_BRK_UART_OVF,         ///< Превышена длина буфера сообщения
    MSG_BRK_UART_FRAME_ERR,   ///< Ошибка фрейма
    MSG_BRK_UART_PARITY_ERR,  ///< Ошибка паритета
} msg_brk_reason_uart_t;

typedef struct
{
    uint8_t len;
    uint8_t data[UART_DATA_LEN_MAX];
    uint16_t baud; ///< Истинный бодрейт пакета, измеренный по синхроимпульсу
    msg_brk_reason_uart_t reason; ///< Причина окончания сообщения (или ошибка отправки)
} msg_uart_t;

/// Структура сообщения универсальная
//-----------------------------------
typedef struct : msg_header_t
{
    union
    {
        msg_can_t  can;
        msg_lin_t  lin;
        msg_uart_t uart;
    };
} msg_t;
