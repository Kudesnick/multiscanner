//------------------------------------------------------------------------------
// Абстрактный класс модуля ввода/вывода
//------------------------------------------------------------------------------

#include "misc.h"

#include "bsp_unit.h"

bsp_unit *bsp_unit::last_pointer = 0;

bsp_unit::bsp_unit(void *_unit_ptr, bsp_unit_callback_t *_callback, uint16_t _class_type, uint16_t _object_name):
    unit_ptr(_unit_ptr),
    callback(_callback),
    class_type(_class_type),
    object_name(_object_name)
{
    prev_pointer = last_pointer;
    last_pointer = this;
};

bsp_unit* bsp_unit::get_prev_pointer(void)
{
    return prev_pointer;
};

bsp_unit* bsp_unit::get_last_pointer(void)
{
    return last_pointer;
};

bsp_unit* bsp_unit::object_search(void *unit)
{
    bsp_unit* result = last_pointer;
    
    while (result != NULL
           && result->unit_ptr != unit)
    {
        result = result->get_prev_pointer();
    }
    
    return result;
}

uint16_t bsp_unit::get_class_type(void)
{
    return class_type;
}

uint16_t bsp_unit::get_object_name(void)
{
    return object_name;
}
