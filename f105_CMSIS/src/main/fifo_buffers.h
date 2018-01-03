//------------------------------------------------------------------------------
// Буферы
//------------------------------------------------------------------------------

#pragma once

#include <stdint.h>

#include "fifo.h"
#include "list.h"
#include "msg_types.h"
#include "units_config.h"

class fifo_buff : cpp_list<LIST_TYPE_BUFFER>
{
	public:
		cpp_fifo<msg_t, IFACES_TX_BUFFER_SIZE> tx;
		cpp_fifo<msg_t, IFACES_RX_BUFFER_SIZE> rx;
		fifo_buff(void);
};
