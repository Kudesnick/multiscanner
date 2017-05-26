/********************************************************************
 * Основная программа
 ********************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "ifaces_types.h"

int main (void)
{
    static volatile uint8_t tmp = 0;

    while(true)
    {
        tmp = ~tmp;
    }
}
