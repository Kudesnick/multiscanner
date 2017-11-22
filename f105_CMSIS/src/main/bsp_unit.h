
#pragma once

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
        bsp_unit(void *_unit_ptr, iface_type_t _class_type = IFACE_TYPE_DEF, iface_name_t _object_name = IFACE_NAME_DEF);
        static bsp_unit *object_search(void *unit);  // ����� ������� �� ����� ������ (����� ��� ������ �� ����������)
        virtual void send_sett(void *sett) = NULL;   // ���������� ����� �������� ������
        virtual void *get_sett(void) = NULL;         // ��������� �������� ������
        virtual bool send_msg(void *msg) = NULL;     // �������� ������
        virtual void interrupt_handler(void) = NULL; // ���������� ����������, �������� ����������� � �������. �� ���� ���������� callback
};
