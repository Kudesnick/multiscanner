//------------------------------------------------------------------------------
// Абстрактный класс потока
//------------------------------------------------------------------------------

#pragma once

#include <stdint.h>
#include <string.h>

#include "list.h"

class thread : public cpp_list<LIST_TYPE_THREAD>
{
    private:
    protected:
    public:
        thread(iface_type_t _class_type = IFACE_TYPE_DEF, iface_name_t _object_name = IFACE_NAME_DEF);
        virtual void routine(void) = NULL;
};
