#ifndef _IPORT_HPP_
#define _IPORT_HPP_

/*!
 * \file IPort.hpp
 * \brief описание интерфейса (абстрактного класса) IPort
 */

#include <cstdint>
#include <string>
#include "config.hpp"
#include "IData.hpp"

class IBlock;

/*!
 * Класс-перечисление для обозначения типа порта (входной или выходной)
 */
enum class PortType
{
	PT_IN,
	PT_OUT,
	PT_UNKNOWN,
};

/*!
 * \brief абстрактный класс, специфицирующий общие свойства и методы всех портов (и входных и выходных).
 */
class IPort
{
private:
	static uint32_t		id_autoinc;
	uint32_t		id;
	std::string		name;
public:

	/*! возвращает порт другой стороны соединения (только для входных портов, для выходных возвращает NULL, потому
	 * что на один выходной порт может приходиться более одного входного, поэтому считаем, что просить выходной
	 * порт назвать его порт-приёмник - это некорректный запрос и всегда возвращаем NULL) */
	virtual IPort		* get_other_port() = 0;
	/*! возвращает имя этого порта */
	virtual std::string	get_name(){return name;};

	/*! возвращает забуферизированное (или расчитывает и буферизирует) значение для времени time
	 *  ttl содержит значение для максимальной глубины рекурсии при вычислении
	 */
	virtual IData&		get_value(const int32_t time, const int32_t ttl, std::string where) = 0;

	/*! возвращает значение, смысл которого, хранить информацию о формате данных, т.е. этот метод используется при
	 * соединениях портов для контроля правильности соединения
	 */
	virtual IData&		get_sample_value() = 0;

	/*! возвращает тип порта (ввод или вывод) */
	virtual PortType	get_port_type() = 0;

	/*! возвращает указатель на кирпичик, которому этот порт принадлежит */
	virtual IBlock		*get_block() = 0;

	/*! возвращает уникальный идентификатор этого порта, возможно, в будущем этот метод будет убран */
	virtual uint32_t	get_id(){return this->id;};

	/*! при конструировании порта обязательно задаются его имя и размерность */
				IPort(std::string sname):
					name(sname){this->id = IPort::id_autoinc++;};
	virtual			~IPort(){};
};

#endif
