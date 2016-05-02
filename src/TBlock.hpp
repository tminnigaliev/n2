#ifndef _TBLOCK_HPP_
#define _TBLOCK_HPP_

/*!
 * \file TBlock.hpp
 * \brief описание класса TBlock (может быть сложным блоком или предком простого блока)
 */

#include <string>
#include <map>
#include "IPort.hpp"
#include "IBlock.hpp"
#include "TPortIn.hpp"
#include "TPortOut.hpp"

typedef std::list<IBlock*>		subblock_list_t;
typedef std::map<std::string, IPort*>	name_to_port_map_t;
typedef std::map<IPort*, std::string>   port_to_name_map_t;
typedef std::map<std::string, IBlock*>	name_to_block_map_t;
typedef std::map<IBlock*, std::string>	block_to_name_map_t;

class TPortOut;

typedef enum{
	/*!
	 * проверяется направление, имя, размерность для каждого входа
	 */
	ACT_INPUTS_ASSIGN = 0,
	ACT_OUTPUTS_ASSIGN = 1,

	/*!
	 * пайплайнятся один к одному с проверкой размерности и имени
	 */
	ACT_ONE_ONE_PIPELINE = 2,

	/*!
	 * пайплайнятся с разветвлением, если необходимо, с проверкой только размерности
	 */
	ACT_SCATTER_DIMS_PIPELINE= 3,

	/*!
	 * пайплайнятся с разветвлением, если необходимо, с проверкой размерности и имени
	 */
	ACT_SCATTER_PIPELINE = 4,
}TAutoConnectingType;

/*
 * конструктор принимает в качестве параметров кол-во входных и выходных портов.
 * операция соединения портов блоков одного ранга называется pipeline. При этой операции
 * соединяться должны всегда разнородные порты (выходной и входной).
 *
 * операция соединения портов внешнего блока с портами внутренних блоков называется assign
 * соединяться должны всегда однородные порты (входной со входным, выходной с выходным) и
 * не должно быть никаких разветвлений.
 */

/*!
 * \brief этот класс используется двояко: как сложный блок (в который можно добавлять простые блоки) и как предшественник простых блоков
 */
class TBlock : public IBlock{
public:
	//calculates all output ports for time t
	virtual IData&		simulate(int32_t t, int32_t ttl, TPortOut * port, std::string where);
	virtual IData&		simulate_port_name(int32_t t, int32_t ttl, std::string port_name, std::string where);
	virtual void		save_to_ptree(boost::property_tree::ptree &pt, std::string where);
	virtual void		load_from_ptree(boost::property_tree::ptree &pt, std::string where);

	virtual TPortIn*	add_in_port(std::string name, IData& sample_value, int delay = 0);
	virtual TPortOut*	add_out_port(std::string name, IData& default_value = TPortOut::std_default_value);
	virtual void		remove_port(std::string name);
	virtual	in_port_list_t&	get_in_ports_list(){return *in_ports;};
	virtual	out_port_list_t&get_out_ports_list(){return *out_ports;};

	virtual TPortIn		*get_in_port(std::string name);
	virtual TPortOut	*get_out_port(std::string name);

	virtual void		add_subblock(IBlock *subblock, std::string name);
	virtual void		connect_subblock(std::string name, TAutoConnectingType type);
	virtual void		remove_block(IBlock *subblock);
	virtual subblock_list_t&get_subblock_list(){return *subblocks;};
	virtual IBlock	*get_subblock_by_name(std::string name){return (*str_to_block)[name];};

				TBlock(std::string sname);
	virtual			~TBlock();
private:
	in_port_list_t		*in_ports;
	in_port_list_t		*free_in_subports;
	out_port_list_t		*out_ports;
	out_port_list_t		*free_out_subports;
	name_to_port_map_t	*str_to_port;

	subblock_list_t		*subblocks;
	name_to_block_map_t	*str_to_block;
	virtual void		connect_subblock_inputs_assign(std::string name);
	virtual void		connect_subblock_outputs_assign(std::string name);
	virtual void		connect_subblock_pipeline(std::string name, bool connect_first, bool match_names);
};

#endif

