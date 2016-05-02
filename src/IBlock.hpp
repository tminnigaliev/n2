#ifndef _IBLOCK_HPP_
#define _IBLOCK_HPP_
#include <string>
#include <map>
#include <list>
#include "config.hpp"
#include "IData.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>


/*!
 * \file IBlock.hpp
 * \brief описание интерфейса (абстрактного класса) IBlock
 */

class IPort;
class IBlock;
class TPortIn;
class TPortOut;

typedef std::list<TPortIn*>		in_port_list_t;
typedef std::list<TPortOut*>		out_port_list_t;
typedef std::map<std::string, IPort*>	name_to_port_map_t;
typedef std::map<std::string, IBlock*>	name_to_block_map_t;

/*
 * операция соединения портов блоков одного ранга называется pipeline. При этой операции
 * соединяться должны всегда разнородные порты (выходной и входной).
 *
 * операция соединения портов внешнего блока с портами внутренних блоков называется assign
 * соединяться должны всегда однородные порты (входной со входным, выходной с выходным) и
 * не должно быть никаких разветвлений.
 */

/*!
 * \brief абстрактный предок всех блоков.
 */
class IBlock{
public:
	//calculates all output ports for time t
	/*!
	 * Производит симуляцию: если блок сложный, то симуляция по входящим в него блокам происходит автоматически
	 * Если простой, то этот метод переопределён. Если метод не переопределён, то получим исключение.
	 * \param[in] t		модельное время, для которого расчитываем результат симуляции
	 * \param[in] ttl	параметр, позволяющий избежать зацикливания, убавляется при каждой новой рекурсии
	 * \param[in] port	выходной порт для которого надо произвести симуляцию
	 * \param[in] where	строка, которая позволяет локализовать ошибку во время симуляции
	 * \param[in] index	индекс на случай, если выходной порт векторный
	 */
	virtual IData&		simulate(int32_t t, int32_t ttl, TPortOut * port, std::string where) = 0;
	//virtual void		modify(int32_t t, IData &error) = 0;

	virtual void		save_to_ptree(boost::property_tree::ptree &pt, std::string where){};
	virtual void		load_from_ptree(boost::property_tree::ptree &pt, std::string where){};
	/*!
	 * \returns		имя блока
	 */
	virtual std::string	get_name(){return name;};

				IBlock(std::string s){name = s;};
	virtual			~IBlock(){};

	/*!
	 * \returns		ссылку на список входных портов
	 */
	virtual	in_port_list_t&	get_in_ports_list() = 0;
	/*!
	 * \returns		ссылку на список выходных портов
	 */
	virtual	out_port_list_t&get_out_ports_list() = 0;

	/*!
	 * \param[in]	name	имя входного порта, указатель на который мы хотим получить
	 * \returns		входной порт по заданному имени
	 */
	virtual TPortIn		*get_in_port(std::string name) = 0;

	/*!
	 * \param[in]	name	имя выходного порта, указатель на который мы хотим получить
	 * \returns		выходной порт по заданному имени
	 */
	virtual TPortOut	*get_out_port(std::string name) = 0;
private:
	/*!
	 * Имя блока
	 */
	std::string		name;
};

#endif

