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
        bsp_unit *prev_pointer; // Предыдущий элемент списка
    protected:
        void (*callback)(void); // callback по приему данных
        void *unit_ptr; // Указатель на физический модуль ввода/вывода
    public:
        bsp_unit(void *unit, void (*clbck)(void));
        bsp_unit* get_prev_pointer(void);
        static bsp_unit* get_last_pointer(void);
        static bsp_unit *object_search(void *unit); // Поиск объекта по имени модуля (нужно для вызова из прерываний)
        virtual void send_sett(); // Применение новых настроек модуля
        virtual void *get_sett(); // Получение настроек модуля
        virtual bool send_msg(); // Отправка данных
        virtual void interrupt_handler(void); // Обработчик прерывания, основные манипуляции с флагами. Из него вызывается callback
};

#endif /* _BSP_UNIT_H_ */
