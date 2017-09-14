#ifndef _BSP_SERIAL_N_
#define _BSP_SERIAL_N_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "fifo_con.h"
#include "bsp_usart.h"

// ������� ���������� ������
enum
{
    SERIAL_MSG_BRK_TIMEOUT, // ���������� �� ��������
    SERIAL_MSG_BRK_LAST_ID, // ������������ ���� ����������
    SERIAL_MSG_BRK_FIRST_ID, // ������������ ���� ������ ��������� �������
    SERIAL_MSG_BRK_LENGTH, // ���������� ������������� ����� �������
    SERIAL_MSG_BRK_OVF, // ��������� ����� ������ ���������
    SERIAL_MSG_BRK_FRAME_ERR, // ������ ������
    SERIAL_MSG_BRK_PARITY_ERR, // ������ ��������
} msg_brk_reason_t;

typedef struct
{
            uint64_t timestamp;
             uint8_t data[16];
    msg_brk_reason_t reason;
} msg_serial_t;

typedef struct
{
    uint32_t baudrate;  // �������
    uint16_t parity;    // �������
    uint16_t stop_bits; // ���������� �������� �����
        bool echo;      // ��� �������� ���������
        bool color;     // ��������� ������ � �������
} bsp_serial_config_t;

class bsp_serial : private bsp_usart
{
    private:
        fifo_serial * bufer;
        static bsp_serial_config_t default_sett;
        bsp_serial_config_t setting;
        virtual void callback(void * msg, uint32_t flags); // � ���� ������ ������������� (void *)->(msg_serial_t *)
    protected:
    public:
        bsp_serial(USART_TypeDef *_unit_ptr, fifo_serial * buf, bsp_serial_config_t * _setting = &default_sett);
        bool send(msg_serial_t *msg);
        bsp_serial_config_t *get_setting(void);
        void set_setting(bsp_serial_config_t * sett);
        uint32_t round_baud(uint32_t baud); // ���������� ��������� �������� ����� ���������� ��������
};

#endif  /* _BSP_SERIAL_N_ */ 
