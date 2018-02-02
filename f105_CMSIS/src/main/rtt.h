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

#pragma once

#ifdef DEBUG
    #define RTT_CREATE_LOG {rtt::send("Obj was created: \"");rtt::send(typeid(*this).name());rtt::send("\"\r\n");}
#else
    #define RTT_CREATE_LOG {}
#endif

class rtt
{
    private:
        rtt(void);
        static int buf_create;
    public:
        static void send(const char * str);
};
