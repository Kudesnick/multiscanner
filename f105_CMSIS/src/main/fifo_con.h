//------------------------------------------------------------------------------
// Промежуточный буфер для консоли
//------------------------------------------------------------------------------

#ifndef _FIFO_CON_H_
#define _FIFO_CON_H_

#include "stdint.h"

#include "fifo.h"

#define RX_BUFFER_SIZE FIFO_SIZE_256
#define TX_BUFFER_SIZE FIFO_SIZE_512

class fifo_con_rx_buffer: public cpp_fifo<char, RX_BUFFER_SIZE>
{
	private:
		uint32_t word_counter;
		uint32_t string_counter;
		char last_char;
	public:
		fifo_con_rx_buffer(void);
		uint32_t get_word_count(void);
		uint32_t get_str_count(void);
		virtual void add(char data);
        virtual char extract(void);
};

class fifo_con_tx_buffer: public cpp_fifo<char, TX_BUFFER_SIZE>
{
	public:
		fifo_con_tx_buffer(void);
		bool send_str(const char * str, const bool escape_not_del);
};

class fifo_con
{
	public:
		fifo_con_rx_buffer rx;
		fifo_con_tx_buffer tx;
		fifo_con(void);
};

#endif /* _FIFO_H_ */
