//------------------------------------------------------------------------------
// ������ ��������
//------------------------------------------------------------------------------
#ifndef _LIST_H_
#define _LIST_H_

#include <stdint.h>
#include <string.h>

typedef enum
{
    LIST_TYPE_DEFAULT,
    LIST_TYPE_UNIT,
    LIST_TYPE_THREAD,
    LIST_TYPE_PARSER
}list_type_t;

template <const uint16_t list_type> class cpp_list
{
    private:
        static const uint16_t obj_list_type = list_type;
        static cpp_list *last_pointer; // ��������� �� ��������� ������� � ������
        cpp_list *prev_pointer;        // ���������� ������� ������
        uint16_t class_type;           // ��� ������ (������)
        uint16_t object_name;          // ��� �������
    protected:
    public:
        cpp_list(uint16_t _class_type = 0, uint16_t _object_name = 0);
        cpp_list* get_prev_pointer(void);            // �������� ��������� �� ���������� ������� ������
        static cpp_list* get_last_pointer(void);     // �������� ��������� �� ��������� ������� � ������
        uint16_t get_list_type(void);                // ���������� ��� ������
        uint16_t get_class_type(void);               // ���������� ��� ������ (�������� � ������������ ������������ ������)
        uint16_t get_object_name(void);              // ���������� ��� ������� (�������� ��� �������� ����������� �������, ���� ����)
};

template <const uint16_t list_type> cpp_list<list_type> * cpp_list<list_type>::last_pointer = NULL;

template <const uint16_t list_type> cpp_list<list_type>::cpp_list(uint16_t _class_type, uint16_t _object_name):
    class_type(_class_type),
    object_name(_object_name)
{
    prev_pointer = last_pointer;
    last_pointer = this;
};

template <const uint16_t list_type> cpp_list<list_type> * cpp_list<list_type>::get_prev_pointer(void)
{
    return prev_pointer;
};

template <const uint16_t list_type> cpp_list<list_type> * cpp_list<list_type>::get_last_pointer(void)
{
    return last_pointer;
};

template <const uint16_t list_type> uint16_t cpp_list<list_type>::get_list_type(void)
{
    return obj_list_type;
};

template <const uint16_t list_type> uint16_t cpp_list<list_type>::get_class_type(void)
{
    return class_type;
};

template <const uint16_t list_type> uint16_t cpp_list<list_type>::get_object_name(void)
{
    return object_name;
};

#endif /* _LIST_H_ */