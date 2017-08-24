#ifndef _PARSER_H_
#define _PARSER_H_

//------------------------------------------------------------------------------
// ����� ������� � ���� ��� ������� �����
//------------------------------------------------------------------------------

#include <stdint.h>
#include <string.h>

#define PARAM_ERR ((int32_t)-1) // ��������� ������
#define countof_arr(a) (sizeof(a)/sizeof(a[0])) // ���������� ���������� ��������� �������

// ������ �������� ��� ��������
struct fsm_steps_t
{
    const char * const name;                            // ��������� �������
    const bool(* const func)(char * str, void * param); // ������� ����������
    const void * param;                                 // �������������� ��������
};

// ������ ����� ������
const char parser_str_err_bad_cmd[]    = "\x1b[31mError! This command is invalid.\x1b[0m\r\n";
const char parser_str_err_null_cmd[]   = "\x1b[31mError! This command is not realised.\x1b[0m\r\n";
const char parser_str_err_syntax_cmd[] = "\x1b[31mError! This command syntax is invalid. Print help.\x1b[0m\r\n";

char *   parser_uint_to_str(uint32_t num);       // ������� ����� � ������
uint32_t parser_str_to_uint(char * str);         // ������� ������ � �����
void     parser_lowercase(char *buf);            // ���������� � ������� ��������
void     parser_parse(char * str, void * param); // ����������, ������ ������

#endif /* _PARSER_H_ */
