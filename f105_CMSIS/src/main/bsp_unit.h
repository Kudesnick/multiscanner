#ifndef _BSP_UNIT_H_
#define _BSP_UNIT_H_

//------------------------------------------------------------------------------
// Абстрактный класс модуля ввода/вывода
//------------------------------------------------------------------------------

#include <string.h>

#include "list.h"

class bsp_unit : public cpp_list<LIST_TYPE_UNIT>
{
    private:    
    protected:
        void *unit_ptr;                // Указатель на физический модуль ввода/вывода
        virtual void callback(void *msg, uint32_t flags) = NULL; // Вызывается из прерывания. По сути - высокоуровневый обработчик.
    public:
        bsp_unit(void *_unit_ptr, uint16_t _class_type = NULL, uint16_t _object_name = NULL);
        static bsp_unit *object_search(void *unit);  // Поиск объекта по имени модуля (нужно для вызова из прерываний)
        virtual void send_sett(void *sett) = NULL;   // Применение новых настроек модуля
        virtual void *get_sett(void) = NULL;         // Получение настроек модуля
        virtual bool send_msg(void *msg) = NULL;     // Отправка данных
        virtual void interrupt_handler(void) = NULL; // Обработчик прерывания, основные манипуляции с флагами. Из него вызывается callback
};

#endif /* _BSP_UNIT_H_ */
