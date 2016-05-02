#ifndef _TKOHONEN_HPP_
#define _TKOHONEN_HPP_

/*!
 * \file TKohonen.hpp
 * \brief описание класса для реализации сети Кохонена
 */

#include <string>
#include "TBlock.hpp"
#include "TDataMatrix.hpp"

class TKohonen : public TBlock{
public:
	//calculates all output ports for time t
	virtual IData&		simulate(int32_t t, int32_t ttl, TPortOut * port, std::string where);
	virtual IData&		simulate_port_name(int32_t t, int32_t ttl, std::string port_name, std::string where);
	virtual void		save_to_ptree(boost::property_tree::ptree &pt, std::string where);
	virtual void		load_from_ptree(boost::property_tree::ptree &pt, std::string where);

				TKohonen(std::string sname, int inp_cnt, std::string inp_name, int neuron_cnt, std::string out_name);
	virtual			~TKohonen();
private:
	TDataMatrix		*weights;
	TDataVector		*outputs;
	double			alpha;
};

#endif

