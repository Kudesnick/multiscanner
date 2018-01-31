/**
 *  @file    main.cpp
 *
 *  @brief   Основная программа
 *  @details Здесь просто запускаем основной процесс
 *
 *  @author  
 *  @date    
 */

#include "rtt.h"
#include "multiscanner.h"

int main(void)
{
    rtt::send("Main function runing.\r\n");
    
    return cpp_multiscanner::routine();
};
