#ifndef _PARSER_SET_H_
#define _PARSER_SET_H_

//------------------------------------------------------------------------------
// Задание настроек интерфейсов
//------------------------------------------------------------------------------

#include <stdint.h>
#include <string.h>

bool parser_set(char ** str, const void * const param, void * const result); // Настроить интерфейс

#endif /* _PARSER_SET_H_ */
