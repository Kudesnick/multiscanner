#ifndef _BSP_USART_H_
#define _BSP_USART_H_

#include <stdint.h>
#include <string.h>

#include "bsp_io.h"
#include "bsp_unit.h"

//------------------------------------------------------------------------------
// ����� ����� �����/������ UART
//------------------------------------------------------------------------------

#define USART_LIN_BRK_DATA (uint16_t)0x8000;
#define USART_IDLE_DATA    (uint16_t)0xFFFF;

typedef struct
{
    // ���������� ���������
    uint32_t USART_BaudRate;
    uint16_t USART_WordLength;
    uint16_t USART_StopBits;
    uint16_t USART_Parity;
    uint16_t USART_Mode;
    uint16_t USART_LIN_Break_Detection_Length;
    // ���������� ���������
    bool USART_LIN_Enable;
    bool USART_Enable;
} bsp_usart_setting_t;

typedef uint16_t bsp_usart_msg_t;

class bsp_usart: public bsp_unit
{
    private:
        bsp_io pin_rx;
        bsp_io pin_tx;
    protected:
        bsp_usart_setting_t setting;
    public:
        bsp_usart(USART_TypeDef *_unit_ptr, iface_type_t _class_type = IFACE_TYPE_DEF, iface_name_t _object_name = IFACE_NAME_DEF);
        void send_sett(bsp_usart_setting_t *sett);
        virtual void send_sett(void *sett); // ���������� ����� �������� ������
        virtual void *get_sett(void); // ��������� �������� ������
        virtual bool send_msg(void *msg); // �������� ������
        virtual void interrupt_handler(void); // ���������� ����������, �������� ����������� � �������. �� ���� ���������� callback
        uint32_t round_baud(uint32_t baud); // ���������� ��������� �������� ����� ���������� ��������
};

#endif /* _BSP_USART_H_ */
