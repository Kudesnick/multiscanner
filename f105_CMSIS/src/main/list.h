/**
 *  @file    list.h
 *
 *  @brief   ������ ������ ������
 *  @details ������������ ��� �������������� ��������� ������� ��������
 *
 *  @author  
 *  @date    
 */

#pragma once

#include <stdint.h>
#include <string.h>

#include "units_config.h"

/**
 *  @brief ���� ������� ��� ������������� ������� ��������
 *  @details � ����������� �� ����, ����� list_type_t ����������� ��� ������������ �������,
 *  ������ �������� � ��� ��� ���� ������. �������� ������������� ������������,
 *  ����� ������ �������� � ��������� �������.
 */
typedef enum : uint8_t
{
    LIST_TYPE_DEFAULT,  ///< ��� �� ���������
    LIST_TYPE_UNIT   ,  ///< ����/�����. (������������ ������� ������������ ������ �����-������)
    LIST_TYPE_THREAD ,  ///< �����. (������������ ������� ������-������)
    LIST_TYPE_PARSER ,  ///< ������. (������������ ��� �������� ������� ���������� ������)
    LIST_TYPE_BUFFER ,  ///< ������ �����-������
    LIST_TYPE_SYS_CLK,  ///< ��������� ������
    LIST_TYPE_TMR    ,  ///< �������
} list_type_t;

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
        static const list_type_t obj_list_type = list_type;
        static cpp_list *last_pointer; ///< ��������� �� ��������� ������� � ������
        cpp_list *prev_pointer;        ///< ���������� ������� ������
        iface_type_t class_type;       ///< ��� ������ (������)
        iface_name_t object_name;      ///< ��� �������
    protected:
    public:
        cpp_list(iface_type_t _class_type = IFACE_TYPE_DEF, iface_name_t _object_name = IFACE_NAME_DEF);
        cpp_list* get_prev_pointer(void);            ///< �������� ��������� �� ���������� ������� ������
        static cpp_list* get_last_pointer(void);     ///< �������� ��������� �� ��������� ������� � ������
        list_type_t get_list_type(void);             ///< ���������� ��� ������
        iface_type_t get_class_type(void);           ///< ���������� ��� ������ (�������� � ������������ ������������ ������)
        iface_name_t get_object_name(void);          ///< ���������� ��� ������� (�������� ��� �������� ����������� �������, ���� ����)
        static cpp_list* get_object(iface_type_t _class_type, iface_name_t _object_name); ///< ����� ������� �� �����
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
template <const list_type_t list_type> cpp_list<list_type>::cpp_list(iface_type_t _class_type, iface_name_t _object_name):
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
template <const list_type_t list_type> iface_type_t cpp_list<list_type>::get_class_type(void)
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
template <const list_type_t list_type> iface_name_t cpp_list<list_type>::get_object_name(void)
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
template <const list_type_t list_type> cpp_list<list_type> * cpp_list<list_type>::get_object(iface_type_t _class_type, iface_name_t _object_name)
{
    for(cpp_list * ptr = get_last_pointer();
        ptr != NULL;
        ptr = ptr->get_prev_pointer()
        )
    {
        if (   ptr->get_object_name() == _object_name
            && (   _class_type == NULL
                || ptr->get_class_type() == _class_type
                )
            )
        {
            return ptr;
        }
    }
    
    return NULL;
};
