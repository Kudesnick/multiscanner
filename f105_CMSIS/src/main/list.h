/**
 *  @file    list.h
 *
 *  @brief   шаблон класса списка
 *  @details используется для автоматической генерации списков объектов
 *
 *  @author  
 *  @date    
 */

#pragma once

#include <stdint.h>
#include <string.h>

#include "units_config.h"

/**
 *  @brief Типы списков для разграничения списков объектов
 *  @details В зависимости от того, какой list_type_t указывается при наследовании шаблона,
 *  объект попадает в тот или иной список. Возможно множественное наследование,
 *  тогда объект попадает в несколько списков.
 */
typedef enum : uint8_t
{
    LIST_TYPE_DEFAULT,  ///< Тип по умолчанию
    LIST_TYPE_UNIT   ,  ///< Ввод/вывод. (Используется модулем абстрактного класса ввода-вывода)
    LIST_TYPE_THREAD ,  ///< Поток. (Используется модулем класса-потока)
    LIST_TYPE_PARSER ,  ///< Парсер. (Используется для объектов парсера консольных команд)
    LIST_TYPE_BUFFER ,  ///< Буфера ввода-вывода
    LIST_TYPE_SYS_CLK,  ///< Системный таймер
    LIST_TYPE_TMR    ,  ///< Таймеры
} list_type_t;

/**
 *  @brief Шаблон класса списка
 *  @details Объекты, наследуемые от этого шаблона автоматически при создании попадают в тот или иной список,
 *  в зависимости от значения list_type. Допускается множественное наследование для попадания
 *  объекта сразу в несколько списков
 *  
 *  @param[in] list_type Тип списка
 *  
 *  @warning Деструктора нет. Использовать только со статически создаваемыми объектами
 */
template <const list_type_t list_type> class cpp_list
{
    private:
        static const list_type_t obj_list_type = list_type;
        static cpp_list *last_pointer; ///< Указатель на последний элемент в списке
        cpp_list *prev_pointer;        ///< Предыдущий элемент списка
        iface_type_t class_type;       ///< Тип модуля (класса)
        iface_name_t object_name;      ///< Имя объекта
    protected:
    public:
        cpp_list(iface_type_t _class_type = IFACE_TYPE_DEF, iface_name_t _object_name = IFACE_NAME_DEF);
        cpp_list* get_prev_pointer(void);            ///< Получить указатель на предыдущий элемент списка
        static cpp_list* get_last_pointer(void);     ///< Получить указатель на последний элемент в списке
        list_type_t get_list_type(void);             ///< Возвращает тип списка
        iface_type_t get_class_type(void);           ///< Возвращает тип класса (задается в конструкторе производного класса)
        iface_name_t get_object_name(void);          ///< Возвращает имя объекта (задается при создании конкретного объекта, если надо)
        static cpp_list* get_object(iface_type_t _class_type, iface_name_t _object_name); ///< Поиск объекта по имени
};

/**
 *  @brief Указатель на последний элемент в списке
 *  @details элемент указывает на последний созданный объект класса-наследника списка
 */
template <const list_type_t list_type> cpp_list<list_type> * cpp_list<list_type>::last_pointer = NULL;

/**
 *  @brief Конструктор
 *  @details Помимо типа списка, жестко задаваемого типизацией шаблона, мы можем разделить элементы в списке
 *           по их типу и имени. И то и другое - перечисляемые параметры. Они не влияют на функционал объекта,
 *           но могут быть использованы для построения сложной иерархии объектов
 *
 *  @param[in] _class_type тип объекта списка
 *  @param[in] _object_name имя объекта
 *
 *  @return указатель на объект
 */
template <const list_type_t list_type> cpp_list<list_type>::cpp_list(iface_type_t _class_type, iface_name_t _object_name):
    class_type(_class_type),
    object_name(_object_name)
{
    prev_pointer = last_pointer;
    last_pointer = this;
};

/**
 *  @brief Получить указатель на предыдущий элемент списка
 *
 *  @return указатель на объект. NULL, если текущий объект - первый в списке
 */
template <const list_type_t list_type> cpp_list<list_type> * cpp_list<list_type>::get_prev_pointer(void)
{
    return prev_pointer;
};

/**
 *  @brief Получить указатель на последний элемент в списке
 *
 *  @return указатель на объект. NULL, если список пуст
 */
template <const list_type_t list_type> cpp_list<list_type> * cpp_list<list_type>::get_last_pointer(void)
{
    return last_pointer;
};

/**
 *  @brief Возвращает тип списка
 *  @details используется для определения типа списка к которому пренадлежит какой-либо конкретный объект.
 *           Единственное ограничение - объект должен наследоваться от данного шаблона.
 *           Предназначен для детектирования списка, если тип объекта неизвестен.
 *
 *  @return Тип списка
 */
template <const list_type_t list_type> list_type_t cpp_list<list_type>::get_list_type(void)
{
    return obj_list_type;
};

/**
 *  @brief Возвращает тип класса (задается в конструкторе производного класса)
 *  @details используется для определения типа класса к которому пренадлежит какой-либо конкретный объект.
 *           Единственное ограничение - объект должен наследоваться от данного шаблона.
 *
 *  @return Тип класса
 */
template <const list_type_t list_type> iface_type_t cpp_list<list_type>::get_class_type(void)
{
    return class_type;
};

/**
 *  @brief Возвращает имя объекта (задается при создании конкретного объекта, если надо)
 *  @details используется для определения имени конкретного объекта.
 *           Единственное ограничение - объект должен наследоваться от данного шаблона.
 *
 *  @warning Следует учесть, что имена задаются в консрукторе объекта произвольно и никак не привязаны
 *           к его свойствам. Также не гарантируется уникальность имен.
 *
 *  @return Имя объекта
 */
template <const list_type_t list_type> iface_name_t cpp_list<list_type>::get_object_name(void)
{
    return object_name;
};

/**
 *  @brief Поиск объекта по имени
 *  @details находит последний добавленный в список объект с совпадающим именем и типом объекта и возвращает указатель на него.
 *           если _class_type указан как NULL, то ищет только по имени (игнорируя тип). Если объект не найден - возвращает NULL.
 *
 *  @warning Следует учесть, что имена задаются в консрукторе объекта произвольно и никак не привязаны
 *           к его свойствам. Также не гарантируется уникальность имен.
 *
 *  @param[in] _class_type Тип класса искомого объекта. Если NULL - любой тип
 *  @param[in] _object_name имя искомого объекта.
 *
 *  @return Указатель на объект. Ели не найден - возвращает NULL
 */
template <const list_type_t list_type> cpp_list<list_type> * cpp_list<list_type>::get_object(iface_type_t _class_type, iface_name_t _object_name)
{
    for(cpp_list * ptr = get_last_pointer();
        ptr != NULL;
        ptr = ptr->get_prev_pointer()
        )
    {
        if (   ptr->get_object_name() == _object_name
            && (   _class_type == NULL
                || ptr->get_class_type() == _class_type
                )
            )
        {
            return ptr;
        }
    }
    
    return NULL;
};
