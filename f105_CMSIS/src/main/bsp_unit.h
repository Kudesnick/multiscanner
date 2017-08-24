#ifndef _BSP_UNIT_H_
#define _BSP_UNIT_H_

//------------------------------------------------------------------------------
// ����������� ����� ������ �����/������
//------------------------------------------------------------------------------

#include <string.h>

#include "bsp_io.h"

class bsp_unit
{
    private:    
        static bsp_unit *last_pointer; // ��������� �� ��������� ������� � ������
        bsp_unit *prev_pointer;        // ���������� ������� ������
    protected:
        void *unit_ptr;                // ��������� �� ���������� ������ �����/������
        uint16_t class_type;           // ��� ������ (������)
        uint16_t object_name;          // ��� �������
        virtual void callback(void *msg, uint32_t flags) = NULL; // ���������� �� ����������. �� ���� - ��������������� ����������.
    public:
        bsp_unit(void *_unit_ptr, uint16_t _class_type = NULL, uint16_t _object_name = NULL);
        bsp_unit* get_prev_pointer(void);            // �������� ��������� �� ���������� ������� ������
        static bsp_unit* get_last_pointer(void);     // �������� ��������� �� ��������� ������� � ������
        static bsp_unit *object_search(void *unit);  // ����� ������� �� ����� ������ (����� ��� ������ �� ����������)
        uint16_t get_class_type(void);               // ���������� ��� ������ (�������� � ������������ ������������ ������)
        uint16_t get_object_name(void);              // ���������� ��� ������� (�������� ��� �������� ����������� �������, ���� ����)
        virtual void send_sett(void *sett) = NULL;   // ���������� ����� �������� ������
        virtual void *get_sett(void) = NULL;         // ��������� �������� ������
        virtual bool send_msg(void *msg) = NULL;     // �������� ������
        virtual void interrupt_handler(void) = NULL; // ���������� ����������, �������� ����������� � �������. �� ���� ���������� callback
};

#endif /* _BSP_UNIT_H_ */
