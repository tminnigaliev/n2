#ifndef _TREADER_FROM_OCT_MATRIX_HPP_
#define _TREADER_FROM_OCT_MATRIX_HPP_

class IPort;
#include"TBlock.hpp"
#include"TPortOut.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include "Tcnexception.hpp"

/*!
 * \brief блок, который возвращает константу через единственный выходной порт (A).
 */
class TReaderFromOctMatrix: public TBlock{
public:
	//calculates all output ports for time t
	virtual IData&		simulate(int32_t t, int32_t ttl, TPortOut * port, std::string where){
					if (port==this->get_out_port(out)){

						return read_row(t, where);
					}
					else throw Tcnexception(this->get_name()+": non-existing port requested ("+where+")");
				};
	//virtual IData&		simulate(int32_t t, int32_t ttl, std::string port_name, std::string where);
				TReaderFromOctMatrix(std::string sname, std::string fname, int buffer_depth, IData& value, std::string out="A"):
				TBlock(sname),
				out(out),
				data_file(fname),
				rows(-1),
				cols(-1),
				biggest_known_row_number(-1),
				value(value.clone()){
					this->add_out_port(out, value);
					pos_cache = new std::map<int,int>;
					pre_read();
					if (cols != value.get_dim(0)){
						throw Tcnexception(sname+": when opening file "+fname+" amount of columns not equal to dimension of vector");
					}
				};
	virtual			~TReaderFromOctMatrix(){
					this->remove_port(out);
					delete value;
					delete pos_cache;
				};
private:
	IData			*value;
	std::map<int,int>*	pos_cache;
	int			biggest_known_row_number;
	std::string		out;
	std::ifstream		data_file;
	int			rows;
	int			cols;
	virtual void		pre_read();
	virtual IData&		read_row(int32_t, std::string where);
};

#endif
