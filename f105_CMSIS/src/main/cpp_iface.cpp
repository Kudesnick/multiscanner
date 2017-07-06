/********************************************************************
 * cpp_iface класс интерфейса
 ********************************************************************/

#include "stdint.h"
#include "cpp_iface.h"
#include "fifo.h"

cpp_iface::cpp_iface(iface_name_t i_name):name = i_name
{
    FIFO_CLEAR(rx_buffer);
    FIFO_CLEAR(tx_buffer);
};

void cpp_iface::init(iface_sett_t *i_sett):sett = *i_sett
{
    
};

int32_t cpp_iface::send_msg(msg_t *i_msg)
{
    if (*i_msg == NULL)
    {
        return FIFO_GET_FULL(tx_buffer);
    }
    else if (FIFO_GET_FULL(tx_buffer) < FIFO_GET_SIZE(tx_buffer))
    {
        FIFO_ADD(tx_buffer, *i_msg);
        return FIFO_GET_FULL(tx_buffer);
    }
    else
    {
        return -1;
    }
};

msg_t *cpp_iface::get_msg(void)
{
    return (FIFO_GET_FULL(rx_buffer) > 0) ? &FIFO_EXTRACT(rx_buffer) : NULL;
};

void cpp_iface::clear_rx_buf(void)
{
    FIFO_CLEAR(rx_buffer);
};

void cpp_iface::clear_tx_buf(void)
{
    FIFO_CLEAR(tx_buffer);
};

iface_name_t cpp_iface::get_name(void)
{
    return name;
};

iface_type_t cpp_iface::get_type(void)
{
    return iface_name_to_type(name);
};

