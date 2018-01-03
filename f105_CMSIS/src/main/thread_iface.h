/**
 *  @file    thread_iface.h
 *
 *  @brief   ����� ������ ����������
 *  @details ������������� ����������� ����� ������ ����������. ������������� ��������� ��� �������� ���������
 *
 *  @author  
 *  @date    22.11.2017
 */

#pragma once

#include <stdint.h>
#include <string.h>

#include "thread.h"
#include "fifo_buffers.h"
#include "msg_types.h"

/**
 *  @brief   ����� ������ ����������
 *  @details ������������� ����������� ����� ������ ����������. ������������� ��������� ��� �������� ���������
 */
class thread_iface : public thread
{
    protected:
        fifo_buff buf;
        virtual bool send_msg(msg_t *msg) = NULL; ///< �������� ��������� ��������������� � ������������ ������
        virtual bool send_msg_rdy(void)   = NULL; ///< ������ ���������� ����� ��� �������� ���������
    public:
        thread_iface(iface_type_t _class_type = IFACE_TYPE_DEF, iface_name_t _object_name = IFACE_NAME_DEF);
        msg_t *msg_get(void);
        virtual bool msg_send(msg_t *msg);
        virtual void routine(void);
};
