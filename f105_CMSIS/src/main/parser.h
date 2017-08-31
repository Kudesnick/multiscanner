#ifndef _PARSER_H_
#define _PARSER_H_

//------------------------------------------------------------------------------
// Общие команды и типы для парсера строк
//------------------------------------------------------------------------------

#include <stdint.h>
#include <string.h>

#define PARAM_ERR ((int32_t)-1) // Константа ошибки
#define countof_arr(a) (sizeof(a)/sizeof(a[0])) // Вычисление количества элементов массива

// Ячейка автомата для парсинга
struct parse_fsm_steps_t
{
    const char * const name;                            // Сигнатура команды
    bool(* const func)(char * str, const void * param); // Функция обработчик
    const void * param;                                 // Дополнительный параметр
};

// Строки кодов ошибок
const char parser_str_err_bad_cmd[]    = "\x1b[31mError! This command is invalid.\x1b[0m\r\n";
const char parser_str_err_null_cmd[]   = "\x1b[31mError! This command is not realised.\x1b[0m\r\n";
const char parser_str_err_syntax_cmd[] = "\x1b[31mError! This command syntax is invalid. Print help.\x1b[0m\r\n";

char *   parser_uint_to_str(uint32_t num);       // перевод числа в строку
uint32_t parser_str_to_uint(char * str);         // перевод строки в число
void     parser_lowercase(char *buf);            // Приведение к нижнему регистру
int16_t  parser_find(char * str, const parse_fsm_steps_t * cmd_list, uint16_t cmd_list_len); // Поиск команд в таблице
void     parser_parse(char * str);               // Собственно, парсим строку

#endif /* _PARSER_H_ */
