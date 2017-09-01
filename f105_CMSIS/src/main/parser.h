#ifndef _PARSER_H_
#define _PARSER_H_

//------------------------------------------------------------------------------
// ����� ������� � ���� ��� ������� �����
//------------------------------------------------------------------------------

#include <stdint.h>
#include <string.h>

#include "bsp_con.h"

#define PARAM_ERR ((int32_t)-1) // ��������� ������
#define countof_arr(a) (sizeof(a)/sizeof(a[0])) // ���������� ���������� ��������� �������
#define TAG_RED    "\x1b[31m" // ������������ ����� �������
#define TAG_RED_B  "\x1b[1;31m"
#define TAG_GREEN  "\x1b[32m"
#define TAG_BLUE   "\x1b[34m"
#define TAG_BLUE_U "\x1b[4;34m"

#define TAG_DEF    "\x1b[0m"  // ���������� ��������� ����
#define TAG_LT     " \"\x1b[34m"
#define TAG_GT     "\x1b[0m\" "

// ������ �������� ��� ��������
struct parse_fsm_steps_t
{
    const char * const name;                            // ��������� �������
    bool(* const func)(char * str, const void * param); // ������� ����������
    const void * param;                                 // �������������� ��������
};

// ������ ����� ������
const char parser_str_err_bad_cmd[]    = TAG_RED "Error!" TAG_DEF " This command is invalid.\r\n";
const char parser_str_err_null_cmd[]   = TAG_RED "Error!" TAG_DEF " This command is " TAG_RED_B "not realised" TAG_RED_B ".\r\n";
const char parser_str_err_syntax_cmd[] = TAG_RED "Error!" TAG_DEF " This command syntax is invalid. Print" TAG_LT "help" TAG_GT ".\r\n";

char *   parser_uint_to_str(uint32_t num); // ������� ����� � ������
uint32_t parser_str_to_uint(char * str);   // ������� ������ � �����
void     parser_lowercase(char *buf);      // ���������� � ������� ��������
int16_t  parser_find(char ** str, const parse_fsm_steps_t * cmd_list, uint16_t cmd_list_len); // ����� ������ � �������
void     parser_parse(char * str);         // ����������, ������ ������

#endif /* _PARSER_H_ */
