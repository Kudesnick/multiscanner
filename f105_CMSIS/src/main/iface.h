/**
 *  @file iface.h
 *
 *  @brief ���� ��� ������ � �����������
 *  @details ����������� ���� � ����� ����������� � ����� ��� ���� ���������
 *           ������������ ���� ������
 */
 
#ifndef _IFACE_
#define _IFACE_

/// ��� ����������.
typedef enum : uint8_t
{
    IFACE_TYPE_DEF,     ///< ������������� ���. �� �������� �� �����������. ��� ������������� ������� �� ������ � ���������
    IFACE_TYPE_CAN,     ///< CAN ���������. ����� �������� ������������ ������� ���������.
    IFACE_TYPE_UART,    ///< UART. ��������� ������� ��� ���������.
    IFACE_TYPE_LIN,     ///< LIN. ���������� ���������� ������ USART.
    IFACE_TYPE_CON,     ///< ��������� �������. ���������� �������.
} iface_type_t;

/// ����� ���������� �����������, ��������� � �������
typedef enum : uint8_t
{
    IFACE_NAME_DEF,    ///< ��� �� ���������. ��� ������������� ������� �� ������ � ���������
    IFACE_NAME_CAN1,
    IFACE_NAME_CAN2,
    IFACE_NAME_UART1,
    IFACE_NAME_UART2,
    IFACE_NAME_LIN1,
    IFACE_NAME_LIN2,
    IFACE_NAME_CON,
} iface_name_t;

/// ����������� �������� ���������
typedef enum : uint8_t
{
    MSG_TX,
    MSG_RX
} msg_direction_t;

/// ��������� ���������, ������������� ��� ���� �����������
struct msg_header_t
{
    iface_type_t msg_type;  ///< ��� ���������
    iface_name_t route;     ///< � ����� ��������� ��������
    msg_direction_t direct; ///< ����������� ��������
     uint8_t len;           ///< �������� ����� ���������
    uint64_t time;          ///< ����� �������
    uint64_t interval;      ///< �������� ��� ����������
    uint16_t count;         ///< ������� ����������
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

#define CAN_DATA_LEN_MAX 8

typedef struct : msg_header_t
{
    can_id_t id;
    can_msg_type_t type;
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

#define LIN_DATA_LEN_MAX 8

typedef struct : msg_header_t
{
    uint8_t id;
    can_msg_type_t type;
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

#define UART_DATA_LEN_MAX 16

typedef struct : msg_header_t
{
    uint8_t data[UART_DATA_LEN_MAX];
    msg_brk_reason_lin_t reason; ///< ������� ��������� ��������� (��� ������ ��������)
} msg_uart_t;

/// ��������� ��������� �������������
//-----------------------------------
typedef union
{
    ms_can_t  can;
    ms_lin_t  lin;
    ms_uart_t uart;
}
} msg_t;

#endif /* _IFACE_ */
