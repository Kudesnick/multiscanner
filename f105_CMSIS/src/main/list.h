/**
 *  @file list.h
 *
 *  @brief ������ ������ ������
 *  @details ������������ ��� �������������� ��������� ������� ��������
 */

#ifndef _LIST_H_
#define _LIST_H_

#include <stdint.h>
#include <string.h>

/**
 *  @brief ���� ������� ��� ������������� ������� ��������
 *  @details � ����������� �� ����, ����� list_type_t ����������� ��� ������������ �������,
 *  ������ �������� � ��� ��� ���� ������. �������� ������������� ������������,
 *  ����� ������ �������� � ��������� �������.
 */
typedef enum : uint8_t
{
    LIST_TYPE_DEFAULT,  ///< ��� �� ���������
    LIST_TYPE_UNIT,     ///< ��� �����/������. (������������ ������� ������������ ������ �����-������)
    LIST_TYPE_THREAD,   ///< ��� ������. (������������ ������� ������-������)
    LIST_TYPE_PARSER    ///< ��� �������. (������������ ��� �������� ������� ���������� ������)
}list_type_t;

/**
 *  @brief ������ ������ ������
 *  @details �������, ����������� �� ����� ������� ������������� ��� �������� �������� � ��� ��� ���� ������,
 *  � ����������� �� �������� list_type. ����������� ������������� ������������ ��� ���������
 *  ������� ����� � ��������� �������
 *  
 *  @param[in] list_type ��� ������
 *  
 *  @warning ����������� ���. ������������ ������ �� ���������� ������������ ���������
 */
template <const list_type_t list_type> class cpp_list
{
    private:
#warning �������� uint16_t �� list_type_t � ����� ��� class_type � object_name
        static const list_type_t obj_list_type = list_type;
        static cpp_list *last_pointer; ///< ��������� �� ��������� ������� � ������
        cpp_list *prev_pointer;        ///< ���������� ������� ������
        uint16_t class_type;           ///< ��� ������ (������)
        uint16_t object_name;          ///< ��� �������
    protected:
    public:
        cpp_list(uint16_t _class_type = NULL, uint16_t _object_name = NULL);
        cpp_list* get_prev_pointer(void);            ///< �������� ��������� �� ���������� ������� ������
        static cpp_list* get_last_pointer(void);     ///< �������� ��������� �� ��������� ������� � ������
        list_type_t get_list_type(void);                ///< ���������� ��� ������
        uint16_t get_class_type(void);               ///< ���������� ��� ������ (�������� � ������������ ������������ ������)
        uint16_t get_object_name(void);              ///< ���������� ��� ������� (�������� ��� �������� ����������� �������, ���� ����)
        static cpp_list* get_object(uint16_t _class_type, uint16_t _object_name); ///< ����� ������� �� �����
};

/**
 *  @brief ��������� �� ��������� ������� � ������
 *  @details ������� ��������� �� ��������� ��������� ������ ������-���������� ������
 */
template <const list_type_t list_type> cpp_list<list_type> * cpp_list<list_type>::last_pointer = NULL;

/**
 *  @brief �����������
 *  @details ������ ���� ������, ������ ����������� ���������� �������, �� ����� ��������� �������� � ������
 *           �� �� ���� � �����. � �� � ������ - ������������� ���������. ��� �� ������ �� ���������� �������,
 *           �� ����� ���� ������������ ��� ���������� ������� �������� ��������
 *
 *  @param[in] _class_type ��� ������� ������
 *  @param[in] _object_name ��� �������
 *
 *  @return ��������� �� ������
 */
template <const list_type_t list_type> cpp_list<list_type>::cpp_list(uint16_t _class_type, uint16_t _object_name):
    class_type(_class_type),
    object_name(_object_name)
{
    prev_pointer = last_pointer;
    last_pointer = this;
};

/**
 *  @brief �������� ��������� �� ���������� ������� ������
 *
 *  @return ��������� �� ������. NULL, ���� ������� ������ - ������ � ������
 */
template <const list_type_t list_type> cpp_list<list_type> * cpp_list<list_type>::get_prev_pointer(void)
{
    return prev_pointer;
};

/**
 *  @brief �������� ��������� �� ��������� ������� � ������
 *
 *  @return ��������� �� ������. NULL, ���� ������ ����
 */
template <const list_type_t list_type> cpp_list<list_type> * cpp_list<list_type>::get_last_pointer(void)
{
    return last_pointer;
};

/**
 *  @brief ���������� ��� ������
 *  @details ������������ ��� ����������� ���� ������ � �������� ����������� �����-���� ���������� ������.
 *           ������������ ����������� - ������ ������ ������������� �� ������� �������.
 *           ������������ ��� �������������� ������, ���� ��� ������� ����������.
 *
 *  @return ��� ������
 */
template <const list_type_t list_type> list_type_t cpp_list<list_type>::get_list_type(void)
{
    return obj_list_type;
};

/**
 *  @brief ���������� ��� ������ (�������� � ������������ ������������ ������)
 *  @details ������������ ��� ����������� ���� ������ � �������� ����������� �����-���� ���������� ������.
 *           ������������ ����������� - ������ ������ ������������� �� ������� �������.
 *
 *  @return ��� ������
 */
template <const list_type_t list_type> uint16_t cpp_list<list_type>::get_class_type(void)
{
    return class_type;
};

/**
 *  @brief ���������� ��� ������� (�������� ��� �������� ����������� �������, ���� ����)
 *  @details ������������ ��� ����������� ����� ����������� �������.
 *           ������������ ����������� - ������ ������ ������������� �� ������� �������.
 *
 *  @warning ������� ������, ��� ����� �������� � ����������� ������� ����������� � ����� �� ���������
 *           � ��� ���������. ����� �� ������������� ������������ ����.
 *
 *  @return ��� �������
 */
template <const list_type_t list_type> uint16_t cpp_list<list_type>::get_object_name(void)
{
    return object_name;
};

/**
 *  @brief ����� ������� �� �����
 *  @details ������� ��������� ����������� � ������ ������ � ����������� ������ � ����� ������� � ���������� ��������� �� ����.
 *           ���� _class_type ������ ��� NULL, �� ���� ������ �� ����� (��������� ���). ���� ������ �� ������ - ���������� NULL.
 *
 *  @warning ������� ������, ��� ����� �������� � ����������� ������� ����������� � ����� �� ���������
 *           � ��� ���������. ����� �� ������������� ������������ ����.
 *
 *  @param[in] _class_type ��� ������ �������� �������. ���� NULL - ����� ���
 *  @param[in] _object_name ��� �������� �������.
 *
 *  @return ��������� �� ������. ��� �� ������ - ���������� NULL
 */
template <const list_type_t list_type> cpp_list<list_type> * cpp_list<list_type>::get_object(uint16_t _class_type, uint16_t _object_name)
{
    cpp_list * ptr = get_last_pointer();

#warning ������� ��������, �� ������ ������ �� ����������� - ���� ������� � �����������   
    while (ptr != NULL)
    {
        if (   ptr->get_object_name() == _object_name
            && (   _class_type == NULL
                || ptr->get_class_type() == _class_type
                )
            )
        {
            break;
        }
        else
        {
            ptr = ptr->get_prev_pointer();
        }
    }
    
    return ptr;
};

#endif /* _LIST_H_ */
