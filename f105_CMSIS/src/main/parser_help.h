#ifndef _PARSER_HELP_H_
#define _PARSER_HELP_H_

//------------------------------------------------------------------------------
// Общие команды и типы для парсера строк
//------------------------------------------------------------------------------

#include <stdint.h>
#include <string.h>

#include "parser.h"

bool parser_help(char * str, const void * param); // Получить справку о программе
bool parser_info(char * str, const void * param); // Сводная информация об устройстве (версия, поддерживаемые интерфейсы и пр.)

#endif /* _PARSER_H_ */
