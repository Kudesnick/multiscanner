/**
 *  @file    main.cpp
 *
 *  @brief   �������� ���������
 *  @details ����� ������ ��������� �������� �������
 *
 *  @author  
 *  @date    
 */

#include <stdio.h>
#include "SEGGER_RTT.h"

#include "multiscanner.h"

int main(void)
{
#ifdef DEBUG
    SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);
    SEGGER_RTT_WriteString(0, "SEGGER RTT Debugging is run.\r\n");
#endif
    
    return cpp_multiscanner::routine();
};
