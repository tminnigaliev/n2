#ifndef _TCONST_BLOCK_HPP_
#define _TCONST_BLOCK_HPP_

class IPort;
#include"TBlock.hpp"
#include"TPortOut.hpp"

/*!
 * \brief блок, который возвращает константу через единственный выходной порт (A).
 */
class TConstBlock: public TBlock{
public:
	//calculates all output ports for time t
	virtual IData&		simulate(int32_t t, int32_t ttl, TPortOut * port, std::string where){
					if (port==this->get_out_port(out)) return *value;
					else throw Tcnexception(where+": non-existing port requested");
				};
				TConstBlock(std::string sname, IData& value, std::string out="A"):
				TBlock(sname),
				out(out),
				value(value.clone()){
					this->add_out_port(out, value);
				};
	virtual			~TConstBlock(){
					this->remove_port(out);
					delete value;
				};
private:
	IData			*value;
	std::string		out;
};

#endif
