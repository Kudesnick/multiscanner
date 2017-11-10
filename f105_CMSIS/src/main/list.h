/**
 *  @file list.h
 *
 *  @brief шаблон класса списка
 *
 *  используется для автоматической генерации списков объектов
 */

#ifndef _LIST_H_
#define _LIST_H_

#include <stdint.h>
#include <string.h>

/**
 *  @brief Типы списков для разграничения списков объектов
 *  
 *  В зависимости от того, какой list_type_t указывается при наследовании шаблона,
 *  объект попадает в тот или иной список. Возможно множественное наследование,
 *  тогда объект попадает в несколько списков.
 */
typedef enum
{
    LIST_TYPE_DEFAULT,  /**< Тип по умолчанию */
    LIST_TYPE_UNIT,     /**< Тип ввода/вывода. (Используется модулем абстрактного класса ввода-вывода) */
    LIST_TYPE_THREAD,   /**< Тип потока. (Используется модулем класса-потока) */
    LIST_TYPE_PARSER    /**< Тип парсера. (Используется для объектов парсера консольных команд) */
}list_type_t;

/**
 *  @brief Шаблон класса списка
 *
 *  Объекты, наследуемые от этого шаблона автоматически при создании попадают в тот или иной список,
 *  в зависимости от значения list_type. Допускается множественное наследование для попадания
 *  объекта сразу в несколько списков
 *  
 *  @param[in] list_type Тип списка
 *  
 *  @warning Деструктор не тестировался.
 *  
 *  @todo Переделать, где надо, uint16_t на list_type_t, написать деструктор
 *
 */
template <const uint16_t list_type> class cpp_list
{
    private:
        static const uint16_t obj_list_type = list_type;
        static cpp_list *last_pointer; /**< Указатель на последний элемент в списке */
        cpp_list *prev_pointer;        /**< Предыдущий элемент списка */
        uint16_t class_type;           /**< Тип модуля (класса) */
        uint16_t object_name;          /**< Имя объекта */
    protected:
    public:
        cpp_list(uint16_t _class_type = NULL, uint16_t _object_name = NULL);
        cpp_list* get_prev_pointer(void);            // Получить указатель на предыдущий элемент списка
        static cpp_list* get_last_pointer(void);     // Получить указатель на последний элемент в списке
        uint16_t get_list_type(void);                // Возвращает тип списка
        uint16_t get_class_type(void);               // Возвращает тип класса (задается в конструкторе производного класса)
        uint16_t get_object_name(void);              // Возвращает имя объекта (задается при создании конкретного объекта, если надо)
        static cpp_list* get_object(uint16_t _class_type, uint16_t _object_name); // Поиск объекта по имени
};

template <const uint16_t list_type> cpp_list<list_type> * cpp_list<list_type>::last_pointer = NULL;

template <const uint16_t list_type> cpp_list<list_type>::cpp_list(uint16_t _class_type, uint16_t _object_name):
    class_type(_class_type),
    object_name(_object_name)
{
    prev_pointer = last_pointer;
    last_pointer = this;
};

template <const uint16_t list_type> cpp_list<list_type> * cpp_list<list_type>::get_prev_pointer(void)
{
    return prev_pointer;
};

template <const uint16_t list_type> cpp_list<list_type> * cpp_list<list_type>::get_last_pointer(void)
{
    return last_pointer;
};

template <const uint16_t list_type> uint16_t cpp_list<list_type>::get_list_type(void)
{
    return obj_list_type;
};

template <const uint16_t list_type> uint16_t cpp_list<list_type>::get_class_type(void)
{
    return class_type;
};

template <const uint16_t list_type> uint16_t cpp_list<list_type>::get_object_name(void)
{
    return object_name;
};

template <const uint16_t list_type> cpp_list<list_type> * cpp_list<list_type>::get_object(uint16_t _class_type, uint16_t _object_name)
{
    cpp_list * ptr = get_last_pointer();
    
    while (ptr == NULL)
    {
        if ((ptr->get_class_type() == _class_type)
            && (ptr->get_object_name() == _object_name)
            )
        {
            break;
        }
        else
        {
            ptr = ptr->get_prev_pointer();
        }
    }
    
    return ptr;
};

#endif /* _LIST_H_ */
