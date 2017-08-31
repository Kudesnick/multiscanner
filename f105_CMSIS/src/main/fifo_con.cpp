//------------------------------------------------------------------------------
// Промежуточный буфер для консоли
//------------------------------------------------------------------------------

#include <stdint.h>
#include <string.h>

#include "atom.h"
#include "fifo_con.h"

// Класс приемного буфера
//------------------------------------------------------------------------------
fifo_con_rx_buffer::fifo_con_rx_buffer(void):
    word_counter(0),
    string_counter(0),
    last_char('\0')
{
    cpp_fifo();
};

uint32_t fifo_con_rx_buffer::get_word_count(void)
{
    return word_counter;
};

uint32_t fifo_con_rx_buffer::get_str_count(void)
{
    return string_counter;
};

void fifo_con_rx_buffer::add(char data)
{
    bool add_result = true;
    
    switch (data)
    {
        case  ' ':
        {
            if (last_char == '\0')
            {
                data = '\0';
            }
            break;
        }
        
        case '\r':
        case '\n':
        case '\0':
        {
            data = '\0';
            if (last_char != '\0')
            {
                if ((add_result = !is_full()) == true)
                {
                    cpp_fifo::add('\0');
                    atom_inc(string_counter);
                    atom_inc(word_counter);
                }
            }
            break;
        }
        
        default:
        {
            if (last_char == ' ')
            {
                if ((add_result = !is_full()) == true)
                {
                    cpp_fifo::add(' ');
                    atom_inc(word_counter);
                }
            }
            if ((add_result = !is_full()) == true)
            {
                cpp_fifo::add(data);
            }
            break;
        }
    }
    
    last_char = data;
    
    if (!add_result)
    {
#warning предусмотреть действие, связанное с переполнением буфера
    }
};

char fifo_con_rx_buffer::extract(void)
{
    char result = (!is_empty()) ? cpp_fifo::extract() : '\0';
    
    switch(result)
    {
        case '\0': atom_dec_to_zero(string_counter);
        case ' ' : atom_dec_to_zero(word_counter);
    }
    
    return result;
};

// Класс передающего буфера
//------------------------------------------------------------------------------
fifo_con_tx_buffer::fifo_con_tx_buffer(void)
{
	cpp_fifo();
};

bool fifo_con_tx_buffer::send_str(const char * str)
{
    bool result = false;
    
    if ((get_full_count() + strlen(str)) <= get_count())
    {
        for (uint_fast16_t i = 0; i < strlen(str); i++)
        {
            add(str[i]);
        }
        result = true;
    }
    
    return result;
};

// Класс приемопередающего буфера
//------------------------------------------------------------------------------
fifo_con::fifo_con(void):
    rx(),
    tx()
{

};
