//------------------------------------------------------------------------------
// Абстрактный класс модуля ввода/вывода
//------------------------------------------------------------------------------

#include "misc.h"

#include "list.h"
#include "bsp_unit.h"

bsp_unit::bsp_unit(void *_unit_ptr, uint16_t _class_type, uint16_t _object_name):
    cpp_list(_class_type, _object_name),
    unit_ptr(_unit_ptr)
{
};

bsp_unit* bsp_unit::object_search(void *unit)
{
    bsp_unit * result = (bsp_unit *) bsp_unit::get_last_pointer();
    
    while (result != NULL
           && result->unit_ptr != unit)
    {
        result = (bsp_unit *)result->get_prev_pointer();
    }
    
    return result;
}
