#pragma once

#include <stdint.h>
#include <string.h>

#include "thread.h"

/**
 *  @brief   класс потока перенаправления сообщений
 *  @details
 */
class thread_route : public thread
{
    protected:
        thread_route(void);
    public:
        static thread_route * instance(void);
        virtual void routine(void);
};
