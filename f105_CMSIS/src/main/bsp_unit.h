#ifndef _BSP_UNIT_H_
#define _BSP_UNIT_H_

//------------------------------------------------------------------------------
// ����������� ����� ������ �����/������
//------------------------------------------------------------------------------

#include <string.h>

#include "bsp_io.h"

typedef void(bsp_unit_callback_t)(void);

class bsp_unit
{
    private:    
        static bsp_unit *last_pointer; // ��������� �� ��������� ������� � ������
        bsp_unit *prev_pointer; // ���������� ������� ������
    protected:
        bsp_unit_callback_t *callback; // callback �� ������ ������
        void *unit_ptr; // ��������� �� ���������� ������ �����/������
    public:
        bsp_unit(void *unit, void (*clbck)(void));
        bsp_unit* get_prev_pointer(void);
        static bsp_unit* get_last_pointer(void);
        static bsp_unit *object_search(void *unit); // ����� ������� �� ����� ������ (����� ��� ������ �� ����������)
        virtual void send_sett(); // ���������� ����� �������� ������
        virtual void *get_sett(); // ��������� �������� ������
        virtual bool send_msg(); // �������� ������
        virtual void interrupt_handler(void); // ���������� ����������, �������� ����������� � �������. �� ���� ���������� callback
};

#endif /* _BSP_UNIT_H_ */
