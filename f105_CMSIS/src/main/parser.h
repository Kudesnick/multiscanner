
#pragma once

//------------------------------------------------------------------------------
// Общие команды и типы для парсера строк
//------------------------------------------------------------------------------

#include <stdint.h>
#include <string.h>

#include "bsp_con.h"

#define countof_arr(a) (sizeof(a)/sizeof(a[0])) // Вычисление количества элементов массива

#define TAG_RED    "\x1b[31m"    // Раскрашиваем вывод красным
#define TAG_RED_B  "\x1b[1;31m"  // Красный полужирный
#define TAG_GREEN  "\x1b[32m"    // Зеленый
#define TAG_BLUE   "\x1b[34m"    // Синий
#define TAG_BLUE_U "\x1b[4;34m"  // Синий подчеркнутый

#define TAG_DEF    "\x1b[0m"     // Возвращаем дефолтный цвет
#define TAG_LT     " \"\x1b[34m" // Открываем кавычки
#define TAG_GT     "\x1b[0m\" "  // Закрываем кавычки

// Ячейка автомата для парсинга
struct parse_fsm_steps_t
{
    const char * const name;                            // Сигнатура команды
    bool(* const func)(char ** str, const void * param); // Функция обработчик
    const void * const param;                                 // Дополнительный параметр
};

// Строки кодов ошибок
const char parser_str_err_bad_cmd[]    = TAG_RED "Error!" TAG_DEF " This command is invalid.\r\n";
const char parser_str_err_null_cmd[]   = TAG_RED "Error!" TAG_DEF " This command is " TAG_RED_B "not realised" TAG_DEF ".\r\n";
const char parser_str_err_syntax_cmd[] = TAG_RED "Error!" TAG_DEF " This command syntax is invalid. Print" TAG_LT "help" TAG_GT ".\r\n";
const char parser_str_err_iface_fnd[]  = TAG_RED "Error!" TAG_DEF " This interface not found.\r\n";

char *   parser_uint_to_str(uint32_t num); // перевод числа в строку
char *   parser_uint_to_hex(uint64_t num, uint8_t size); // Перевод числа в hex-строку
uint32_t parser_str_to_uint(char ** str);   // перевод строки в число
void     parser_lowercase(char *buf);      // Приведение к нижнему регистру
void     parser_all_param_to_str(const parse_fsm_steps_t * cmd_list, uint16_t cmd_list_len, const char delim = '|'); // Послать все параметры на консоль
int16_t  parser_find(char ** str, const parse_fsm_steps_t * cmd_list, uint16_t cmd_list_len);                        // Поиск команд в таблице
void     parser_recursion(char ** str, const parse_fsm_steps_t * cmd_list, uint16_t cmd_list_len);                   // Перебор по таблице, как узлу дерева
void     parser_parse(char * str);         // Собственно, парсим строку
