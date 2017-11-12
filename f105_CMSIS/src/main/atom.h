//------------------------------------------------------------------------------
// Атомарные операции
//------------------------------------------------------------------------------

#ifndef _ATOM_H_
#define _ATOM_H_

#include <stdint.h>

#include <misc.h>

#define atom_inc(var) __atom_inc(&var)
#define atom_dec_to_zero(var) __atom_dec_to_zero(&var)

void __atom_inc(uint32_t * ptr);
void __atom_dec_to_zero(uint32_t * ptr);

#endif /* _ATOM_H_ */
