#pragma once

#include <stdint.h>
#include <string.h>

#include "thread.h"

/**
 *  @brief   ����� ������ ��������������� ���������
 *  @details
 */
class thread_route : public thread
{
    protected:
        thread_route();
    public:
        virtual void routine(void);
};
