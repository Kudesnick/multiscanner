#ifndef _BSP_SERIAL_N_
#define _BSP_SERIAL_N_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "fifo_serial.h"
#include "bsp_usart.h"

typedef struct
{
    // ���������� ���������
    uint32_t baudrate;      // �������
    uint16_t word_length;   // ����� ��������� � �����
    uint16_t parity;        // �������
    uint16_t stop_bits;     // ���������� �������� �����
    // ����������� ���������
    uint16_t timeout;       // ������� ��������� ���������, ��.
     int16_t byte_of_begin; // ����, ������������� ��� ������ ��������� (���� < 0 - ������������)
     int16_t byte_of_end;   // ����, ������������� ��� ��������� ��������� (���� < 0 - ������������)
     uint8_t max_len;       // ������������ ����� ��������� (���� 0 - �������������� �������)
        bool echo;          // ��� �������� ���������
        bool enable;        // ���./����. ���������
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
