#ifndef _BSP_UNIT_H_
#define _BSP_UNIT_H_

//------------------------------------------------------------------------------
// ����������� ����� ������ �����/������
//------------------------------------------------------------------------------

#include <string.h>

#include "list.h"

class bsp_unit : public cpp_list<LIST_TYPE_UNIT>
{
    private:    
    protected:
        void *unit_ptr;                // ��������� �� ���������� ������ �����/������
        virtual void callback(void *msg, uint32_t flags) = NULL; // ���������� �� ����������. �� ���� - ��������������� ����������.
    public:
        bsp_unit(void *_unit_ptr, uint16_t _class_type = NULL, uint16_t _object_name = NULL);
        static bsp_unit *object_search(void *unit);  // ����� ������� �� ����� ������ (����� ��� ������ �� ����������)
        virtual void send_sett(void *sett) = NULL;   // ���������� ����� �������� ������
        virtual void *get_sett(void) = NULL;         // ��������� �������� ������
        virtual bool send_msg(void *msg) = NULL;     // �������� ������
        virtual void interrupt_handler(void) = NULL; // ���������� ����������, �������� ����������� � �������. �� ���� ���������� callback
};

#endif /* _BSP_UNIT_H_ */
