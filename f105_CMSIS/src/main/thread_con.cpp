
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

thread_con::thread_con(void):
    buf(),
    unit(CON_UNIT, &buf)
{
};

void thread_con::routine(void)
{
    if (buf.rx.get_str_count() > 0)
    { // � ������ ����������� �������
        char ch;
        do
        {
            ch = buf.rx.extract();
#warning ��� ����� ���������� �� �������
        }
        while (ch != '\0');
    }
};
