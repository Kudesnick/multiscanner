#ifndef _THREAD_H_
#define _THREAD_H_

//------------------------------------------------------------------------------
// Абстрактный класс потока
//------------------------------------------------------------------------------

#include <stdint.h>
#include <string.h>

#include "list.h"

typedef enum
{
    THREAD_TYPE_DEFAULT,
    THREAD_TYPE_CONSOLE
} thread_type_t;

class thread : public cpp_list<LIST_TYPE_THREAD>
{
    private:
    protected:
    public:
        thread(uint16_t _class_type = NULL, uint16_t _object_name = NULL);
        virtual void routine(void) = NULL;
};

#endif /* _BSP_UNIT_H_ */
