#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

/*!
 * \file config.hpp
 * \brief здесь хранятся настройки проекта
 */
#include<cstdint>

//этот #define включает генерацию исключения в случае, если
//где-то появилось негативное значение времени (если он выключен, тогда
//для негативного времени используется дефолтное значение порта вывода
#define ERROR_ON_NEGATIVE_TIME
#undef ERROR_ON_NEGATIVE_TIME

//#define ERROR_ON_DETECTING_RECURSION
typedef int32_t data_type;
#define dt_one  (1024*1024)

#endif
