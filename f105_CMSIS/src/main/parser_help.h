//------------------------------------------------------------------------------
// ����� ������� � ���� ��� ������� �����
//------------------------------------------------------------------------------

#pragma once

#include <stdint.h>
#include <string.h>

#include "parser.h"

bool parser_help(char ** str, const void * const param, void * const result); // �������� ������� � ���������
bool parser_cpuinfo(char ** str, const void * const param, void * const result); // ������� ���������� � ����������������
