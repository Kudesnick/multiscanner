#ifndef _BSP_USART_H_
#define _BSP_USART_H_

#include "bsp_unit.h"

//------------------------------------------------------------------------------
// ����� ����� �����/������ UART
//------------------------------------------------------------------------------

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
        bool USART_Half_Duplex;
} bsp_usart_setting_t;

class bsp_usart : public bsp_unit
{
    private:
    protected:
    public:
        bsp_usart(USART_TypeDef *unit, void (*clbck)(USART_TypeDef *unit));
        virtual void send_sett(bsp_usart_setting_t *sett); // ���������� ����� �������� ������
        virtual void *get_sett(void); // ��������� �������� ������
        virtual bool send_msg(uint16_t data); // �������� ������
        virtual void interrupt_handler(void); // ���������� ����������, �������� ����������� � �������. �� ���� ���������� callback
};

#endif /* _BSP_USART_H_ */
