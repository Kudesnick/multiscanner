/**
 *  @file    thread_serial.h
 *
 *  @brief   
 *  @details 
 *
 *  @author  
 *  @date    
 */

#pragma once

#include <stdint.h>
#include <string.h>

#include "thread.h"
#include "thread_iface.h"
#include "fifo_buffers.h"
#include "msg_types.h"
#include "bsp_serial.h"

/**
 *  @brief   ����� ������ ����������
 *  @details ������������� ����������� ����� ������ ����������. ������������� ��������� ��� �������� ���������
 */
class thread_serial : public thread_iface
{
    private:
        bsp_serial unit;
    protected:
        virtual bool send_msg(msg_t *msg); ///< �������� ��������� ��������������� � ������������ ������
        virtual bool send_msg_rdy(void); ///< ������ ���������� ����� ��� �������� ���������
    public:
        thread_serial(iface_name_t _object_name = IFACE_NAME_DEF);
};
