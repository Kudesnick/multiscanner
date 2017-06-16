/********************************************************************
 * Основная программа
 ********************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "console.h"

int main (void)
{
    static volatile uint8_t tmp = 0;

    console_init();

    while(true)
    {
        tmp = ~tmp;
    }
}
