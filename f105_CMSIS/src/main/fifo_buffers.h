//------------------------------------------------------------------------------
// Буферы
//------------------------------------------------------------------------------

#pragma once

#include <stdint.h>

#include "fifo.h"
#include "list.h"
#include "msg_types.h"
#include "units_config.h"

class fifo_buff : public cpp_list<LIST_TYPE_BUFFER>
{
	public:
		cpp_fifo<msg_t, IFACES_TX_BUFFER_SIZE> tx;
		cpp_fifo<msg_t, IFACES_RX_BUFFER_SIZE> rx;
		fifo_buff(iface_type_t _class_type = IFACE_TYPE_DEF, iface_name_t _object_name = IFACE_NAME_DEF);
};
