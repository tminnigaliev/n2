#ifndef _TKOHONEN_HPP_
#define _TKOHONEN_HPP_

/*!
 * \file TKohonen.hpp
 * \brief описание класса для реализации сети Кохонена
 */

#include <string>
#include "TBlock.hpp"
#include "TDataMatrix.hpp"
#include "TDataVector.hpp"

class TKohonen : public TBlock{
public:
	//calculates all output ports for time t
	virtual IData&		simulate(int32_t t, int32_t ttl, TPortOut * port, std::string where);
	virtual void		save_to_ptree(boost::property_tree::ptree &pt, std::string where);
	virtual void		load_from_ptree(boost::property_tree::ptree &pt, std::string where);

				TKohonen(std::string sname, std::string inp_name, std::string out_name,
				         IData& default_inp_value=TPortOut::std_default_value, IData& default_out_value=TPortOut::std_default_value, double alpha_step=0.0001, double learning_rate = 0.01);
	virtual			~TKohonen();
private:
	IData			*default_in;
	IData			*default_out;
	std::string		in;
	std::string		out;
	TDataMatrix		*weights;
	TDataVector		*outputs;
	double			alpha;
	double			falpha_step;
	double			flearning_rate;
};

#endif

