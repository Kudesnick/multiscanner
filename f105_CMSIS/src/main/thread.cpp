//------------------------------------------------------------------------------
// Абстрактный класс потока
//------------------------------------------------------------------------------

#include <stdint.h>
#include <string.h>

#include "list.h"
#include "thread.h"

/// Таймер потоков должен считать с интервалом 1 мс.
bsp_clk thread::timer(bsp_clk::get_tick() / 1000, NULL);

thread::thread(iface_type_t _class_type, iface_name_t _object_name):
    cpp_list(_class_type, _object_name)
{
};
