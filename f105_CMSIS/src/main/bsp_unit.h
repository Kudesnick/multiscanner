#ifndef _BSP_UNIT_H_
#define _BSP_UNIT_H_

//------------------------------------------------------------------------------
// Абстрактный класс модуля ввода/вывода
//------------------------------------------------------------------------------

#include <string.h>

#include "bsp_io.h"

class bsp_unit
{
    private:    
        static bsp_unit *last_pointer; // Указатель на последний элемент в списке
        bsp_unit *prev_pointer;        // Предыдущий элемент списка
    protected:
        void *unit_ptr;                // Указатель на физический модуль ввода/вывода
        uint16_t class_type;           // Тип модуля (класса)
        uint16_t object_name;          // Имя объекта
        virtual void callback(void *msg, uint32_t flags) = NULL; // Вызывается из прерывания. По сути - высокоуровневый обработчик.
    public:
        bsp_unit(void *_unit_ptr, uint16_t _class_type = NULL, uint16_t _object_name = NULL);
        bsp_unit* get_prev_pointer(void);            // Получить указатель на предыдущий элемент списка
        static bsp_unit* get_last_pointer(void);     // Получить указатель на последний элемент в списке
        static bsp_unit *object_search(void *unit);  // Поиск объекта по имени модуля (нужно для вызова из прерываний)
        uint16_t get_class_type(void);               // Возвращает тип класса (задается в конструкторе производного класса)
        uint16_t get_object_name(void);              // Возвращает имя объекта (задается при создании конкретного объекта, если надо)
        virtual void send_sett(void *sett) = NULL;   // Применение новых настроек модуля
        virtual void *get_sett(void) = NULL;         // Получение настроек модуля
        virtual bool send_msg(void *msg) = NULL;     // Отправка данных
        virtual void interrupt_handler(void) = NULL; // Обработчик прерывания, основные манипуляции с флагами. Из него вызывается callback
};

#endif /* _BSP_UNIT_H_ */
