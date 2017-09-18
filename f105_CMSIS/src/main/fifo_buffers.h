//------------------------------------------------------------------------------
// Буферы
//------------------------------------------------------------------------------

#ifndef _FIFO_BUFFERS_H_
#define _FIFO_BUFFERS_H_

#include "stdint.h"

#include "fifo.h"
#include "iface.h"

// Промежуточный буфер для последовательного порта

#define RX_BUFFER_SIZE FIFO_SIZE_16
#define TX_BUFFER_SIZE FIFO_SIZE_16

class fifo_serial
{
	public:
		cpp_fifo<msg_serial_t, RX_BUFFER_SIZE> rx;
		cpp_fifo<msg_serial_t, TX_BUFFER_SIZE> tx;
		fifo_serial(void);
};

#endif /* _FIFO_BUFFERS_H_ */
