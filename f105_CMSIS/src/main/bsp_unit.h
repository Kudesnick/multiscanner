
#pragma once

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
        bsp_unit(void *_unit_ptr, iface_type_t _class_type = IFACE_TYPE_DEF, iface_name_t _object_name = IFACE_NAME_DEF);
        static bsp_unit *object_search(void *unit);  // Поиск объекта по имени модуля (нужно для вызова из прерываний)
        virtual void send_sett(void *sett) = NULL;   // Применение новых настроек модуля
        virtual void *get_sett(void) = NULL;         // Получение настроек модуля
        virtual bool send_msg(void *msg) = NULL;     // Отправка данных
        virtual void interrupt_handler(void) = NULL; // Обработчик прерывания, основные манипуляции с флагами. Из него вызывается callback
};
