#ifndef _PARSER_GET_H_
#define _PARSER_GET_H_

//------------------------------------------------------------------------------
// Общие команды и типы для парсера строк
//------------------------------------------------------------------------------

#include <stdint.h>
#include <string.h>

#include "parser_get.h"

bool parser_get(char * str, const void * param); // Получить настройки интерфейса

#endif /* _PARSER_GET_H_ */
