//------------------------------------------------------------------------------
// Общие команды и типы для парсера строк
//------------------------------------------------------------------------------

#pragma once

#include <stdint.h>
#include <string.h>

#include "parser.h"

bool parser_help(char ** str, const void * const param, void * const result); // Получить справку о программе
bool parser_cpuinfo(char ** str, const void * const param, void * const result); // Сводная информация о микроконтроллере
