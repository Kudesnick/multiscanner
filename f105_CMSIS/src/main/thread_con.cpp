//------------------------------------------------------------------------------
// ����� �������
//------------------------------------------------------------------------------

#include <stdint.h>
#include <string.h>

#include "thread.h"
#include "bsp_con.h"
#include "fifo_con.h"
#include "thread_con.h"
#include "units_config.h"

thread_con::thread_con(void (* _parse)(char * str)):
    thread(IFACE_TYPE_CON, IFACE_NAME_CON),
    buf(),
    unit(CON_UNIT, &buf),
    parse(_parse)
{
};

void thread_con::set_parser(void (* _parse)(char * str))
{
    parse = _parse;
};

bool thread_con::send_str(const char * str)
{
    return unit.send(str);
};

void thread_con::routine(void)
{
    if (buf.rx.get_str_count() > 0)
    { // � ������ ����������� �������

        static char str_buf[CON_RX_BUFFER_SIZE];

        for(uint16_t i = 0; i < sizeof(str_buf)/sizeof(str_buf[0]); i++)
        {
            str_buf[i] = buf.rx.extract();

            if (str_buf[i] == '\0')
            {
                if (parse != NULL)
                {
                    parse(str_buf);
                }
                break;
            }
        }
    }
};

bool console_send_string(const char * str)
{
    bool result = false;

    thread_con * ptr = (thread_con *)thread::get_last_pointer();

    while (ptr != NULL)
    {
        if (ptr->get_class_type() == IFACE_TYPE_CON)
        {
            if(ptr->send_str(str) == true)
            {
                result = true;
            }
        }
        ptr = (thread_con *)ptr->get_prev_pointer();
    }

    return result;
}
