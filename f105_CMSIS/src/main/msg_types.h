/**
 *  @file msg_types.h
 *
 *  @brief ���� ��� ������ � �����������
 *  @details ����������� ��� ���� ��������� � ������������ ���� ������
 */

#pragma once

#include <stdint.h>

#include "units_config.h"

#define CAN_DATA_LEN_MAX 8
#define LIN_DATA_LEN_MAX 8
#define UART_DATA_LEN_MAX 16

#pragma pack(1) // ������ ��� ��������� � ���� ������. ��� ������������ � �������.
#pragma anon_unions

/// ����������� �������� ���������
typedef enum : uint8_t
{
    MSG_TX,
    MSG_RX
} msg_direction_t;

/// ��������� ���������, ��������������� ��� ��������, ������������� ��� ���� �����������
struct msg_header_t
{
    iface_type_t msg_type;  ///< ��� ���������
    iface_name_t route;     ///< � ����� ��������� ��������
    msg_direction_t direct; ///< ����������� ��������
    union
    {
        uint64_t rx_timestamp;
        struct
        {
            uint32_t counter;      ///< ������ ������ �� ��������� ��������
            uint16_t interval;     ///< �������� ��� ����������
            uint16_t count;        ///< ������� ����������
        };
    };
    
};

// ��������� ���� ��������� CAN
//-----------------------------

/// ������� ���������� ������ CAN-���������
typedef enum : uint8_t
{
    MSG_BRK_CAN_COMPLETE,       ///< ��������� ������� ��� ������
    MSG_BRK_CAN_TIMEOUT,        ///< �������� ��������� �������� �� ��������
#warning �������� ��� ��������� ���� ������ ��� ���������� CAN
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
    msg_brk_reason_can_t reason; ///< ������� ��������� ��������� (��� ������ ��������)
} msg_can_t;

// ��������� ���� ��������� LIN
//-----------------------------

/// ������� ���������� ������ LIN-���������
typedef enum : uint8_t
{
    MSG_BRK_LIN_COMPLETE,       ///< ��������� ������� ��� ������
    MSG_BRK_LIN_TIMEOUT,        ///< �������� ��������� �������� �� ��������
#warning �������� ��� ��������� ���� ������ ��� ���������� LIN
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
    msg_brk_reason_lin_t reason; ///< ������� ��������� ��������� (��� ������ ��������)
} msg_lin_t;

// ��������� ���� ��������� UART
//------------------------------

/// ������� ���������� ������ UART-���������
typedef enum : uint8_t
{
    MSG_BRK_UART_TIMEOUT,     ///< ���������� �� ��������
    MSG_BRK_UART_LAST_ID,     ///< ������������ ���� ����������
    MSG_BRK_UART_FIRST_ID,    ///< ������������ ���� ������ ��������� �������
    MSG_BRK_UART_LENGTH,      ///< ���������� ������������� ����� ������� SERIAL_MSG_LEN_MAX
    MSG_BRK_UART_OVF,         ///< ��������� ����� ������ ���������
    MSG_BRK_UART_FRAME_ERR,   ///< ������ ������
    MSG_BRK_UART_PARITY_ERR,  ///< ������ ��������
} msg_brk_reason_uart_t;

typedef struct
{
    uint8_t len;
    uint8_t data[UART_DATA_LEN_MAX];
    msg_brk_reason_uart_t reason; ///< ������� ��������� ��������� (��� ������ ��������)
} msg_uart_t;

/// ��������� ��������� �������������
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
