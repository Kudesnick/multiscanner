//------------------------------------------------------------------------------
// Атомарные операции
//------------------------------------------------------------------------------

#include <stdint.h>

#include "misc.h"
#include "atom.h"

void __atom_inc(uint32_t * ptr)
{
    register uint32_t tmp;

    do
    {
        tmp = __LDREXW(ptr);
        tmp++;
    }
    while(__STREXW(tmp, ptr));
};

void __atom_dec_to_zero(uint32_t * ptr)
{
    register uint32_t tmp;

    do
    {
        tmp = __LDREXW(ptr);
        if (tmp != 0) tmp--;
    }
    while(__STREXW(tmp, ptr));
};
