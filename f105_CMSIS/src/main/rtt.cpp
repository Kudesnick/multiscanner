/**
 *  @file    
 *
 *  @brief   Вывод в терминал информации через интерфейс отладки
 *  @details Файл для работы с выводом отладочной информации через инструмент SEGGER RTT
 *           Можно использовать любой эмулятор терминала.
 *           Например PUTTY с параметрами putty.exe -telnet -P 19021 127.0.0.1
 *
 *  @author  
 *  @date    
 */

#include <stdio.h>
#include <typeinfo>
#include "SEGGER_RTT.h"

#include "rtt.h"

int rtt::buf_create = SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);

void rtt::send(const char * str)
{
#ifdef RTT
    SEGGER_RTT_WriteString(0, str);
#endif
};
