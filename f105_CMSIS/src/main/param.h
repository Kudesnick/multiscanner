/********************************************************************
 * Типы параметров для настройки консоли
 ********************************************************************/
 
#ifndef _CPP_MULTISCANNER_
#define _CPP_MULTISCANNER_

typedef enum
{
    CON_PARAM_CMD,
    CON_PARAM_IFACE,
    
}con_param_type_t;

typedef const struct
{
    const char* name;
    const uint32_t val;
} con_param_enum_t;

typedef const struct
{
    const char* name;
    const uint32_t min_val;
    const uint32_t max_val;
} con_param_uint_t;


class cpp_multiscanner
{
    private:
    public:
        cpp_multiscanner(void);
        void run(void);
    protected:
};

#endif /* _CPP_MULTISCANNER_ */
