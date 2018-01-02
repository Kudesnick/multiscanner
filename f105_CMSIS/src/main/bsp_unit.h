/**
 *  @file    ��������� ����� ������ �����/������
 *
 *  @brief   
 *  @details 
 *
 *  @author  
 *  @date    
 */
#pragma once

#include <stdbool.h>

#include "list.h"
#include "bsp_clk.h"

template <typename unit_t, typename message_t, typename settings_t> class bsp_unit : public cpp_list<LIST_TYPE_UNIT>
{
    private:
    protected:
        static bsp_clk *timer; ///< ��������� ������ - ������ ��� ���� �����������
        unit_t *unit_ptr; ///< ��������� �� ���������� ������ �����/������
        bool tx_empty;
        /// ���������� �� ����������. �� ���� - ��������������� ����������.
        virtual void callback(void)              = NULL;
        virtual void send_sett(settings_t *sett) = NULL; ///< ���������� ����� �������� ������
        virtual settings_t *get_sett(void)       = NULL; ///< ��������� �������� ������
        virtual bool send_msg(message_t *msg)    = NULL; ///< �������� ������
        virtual message_t *get_msg(void)         = NULL; ///< ��������� ������
    public:
        /// �����������
        bsp_unit(unit_t *_unit_ptr, iface_type_t _class_type = IFACE_TYPE_DEF, iface_name_t _object_name = IFACE_NAME_DEF);
};

extern bsp_clk * timer_create(void);

template <typename unit_t, typename message_t, typename settings_t> bsp_clk * bsp_unit<unit_t, message_t, settings_t>::timer;

template <typename unit_t, typename message_t, typename settings_t> bsp_unit<unit_t, message_t, settings_t>::bsp_unit(unit_t *_unit_ptr, iface_type_t _class_type, iface_name_t _object_name):
    cpp_list(_class_type, _object_name),
    unit_ptr(_unit_ptr),
    tx_empty(true)
{
    timer = timer_create();
};
