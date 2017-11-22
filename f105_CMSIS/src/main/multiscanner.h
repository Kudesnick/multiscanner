/********************************************************************
 * cpp_multiscanner ����� ��������� ����������
 ********************************************************************/

#pragma once

#include "thread_con.h"
#include "parser.h"

class cpp_multiscanner
{
    private:
        thread_con console;
    public:
        // ����������� ���������� �����, ����� ��� ���������� ������ �� ������� ��� �����
        cpp_multiscanner(void):
            console(parser_parse)
        {
        };
        void routine(void);
};
