/********************************************************************
 * cpp_iface ����� ����������
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
    static const iface_type_t msg_type = IFACE_TYPE_DEF; // ��� ���������
    iface_name_t route; // � ����� ��������� ��������
     uint8_t len;       // �������� ����� ���������
    uint64_t time;      // ����� �������
    uint64_t interval;  // �������� ��� ����������
    uint16_t count;     // ������� ����������
    uint16_t reason;    // ������� ��������� ��������� (��� ������ ��������)
};

// ������� ���������� ������
typedef enum
{
    SERIAL_MSG_BRK_TIMEOUT,     // ���������� �� ��������
    SERIAL_MSG_BRK_LAST_ID,     // ������������ ���� ����������
    SERIAL_MSG_BRK_FIRST_ID,    // ������������ ���� ������ ��������� �������
    SERIAL_MSG_BRK_LENGTH,      // ���������� ������������� ����� ������� SERIAL_MSG_LEN_MAX
    SERIAL_MSG_BRK_OVF,         // ��������� ����� ������ ���������
    SERIAL_MSG_BRK_FRAME_ERR,   // ������ ������
    SERIAL_MSG_BRK_PARITY_ERR,  // ������ ��������
} msg_serial_brk_reason_t;

struct msg_serial_t : msg_t
{
    static const iface_type_t msg_type = IFACE_TYPE_UART; // ��� ���������
    uint8_t data[16]; // ������
};

#endif /* _IFACE_ */
