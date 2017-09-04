#ifndef _PARSER_HELP_H_
#define _PARSER_HELP_H_

//------------------------------------------------------------------------------
// Общие команды и типы для парсера строк
//------------------------------------------------------------------------------

#include <stdint.h>
#include <string.h>

#include "parser.h"

bool parser_help(char ** str, const void * param); // Получить справку о программе

#endif /* _PARSER_H_ */
