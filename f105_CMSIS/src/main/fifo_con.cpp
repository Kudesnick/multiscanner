//------------------------------------------------------------------------------
// Промежуточный буфер для консоли
//------------------------------------------------------------------------------

#include "stdint.h"
#include "string.h"

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

uint16_t fifo_con_rx_buffer::get_word_count(void)
{
    return word_counter;
};

uint16_t fifo_con_rx_buffer::get_str_count(void)
{
    return string_counter;
};

void fifo_con_rx_buffer::add(char data)
{
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
                if (!is_full())
                {
                    cpp_fifo::add('\0');
#warning решить вопрос атомарности
                    string_counter++;
                    word_counter++;
                }
            }
            break;
        }
        
        default:
        {
            if (last_char == ' ')
            {
                if (!is_full())
                {
                    cpp_fifo::add(' ');
#warning решить вопрос атомарности
                    word_counter++;
                }
            }
            if (!is_full())
            {
                cpp_fifo::add(data);
            }
        }
    }
    last_char = data;
};

char fifo_con_rx_buffer::extract(void)
{
    char result = (!is_empty()) ? cpp_fifo::extract() : '\0';
    
    switch(result)
    {
#warning решить вопрос атомарности
        case '\0': if (string_counter > 0) string_counter--;
        case ' ': if (word_counter > 0) word_counter--;
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
