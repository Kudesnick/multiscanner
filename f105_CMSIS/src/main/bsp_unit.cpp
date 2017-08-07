//------------------------------------------------------------------------------
// Абстрактный класс модуля ввода/вывода
//------------------------------------------------------------------------------

#include "misc.h"

#include "bsp_unit.h"

bsp_unit::bsp_unit(void *unit, bsp_unit_callback_t *clbck):
    unit_ptr(unit),
    callback(clbck)
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
    bsp_unit* result = bsp_unit::last_pointer;
    
    while (result != NULL
           && result->unit_ptr != unit)
    {
        result = result->get_prev_pointer();
    }
    
    return result;
}
