//------------------------------------------------------------------------------
// Буферы
//------------------------------------------------------------------------------
#include <typeinfo>

#include "rtt.h"
#include "fifo_buffers.h"

fifo_buff::fifo_buff(iface_type_t _class_type, iface_name_t _object_name):
    cpp_list(_class_type, _object_name),
    tx(),
    rx()
{
    RTT_CREATE_LOG;
};
