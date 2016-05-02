#ifndef _TMULTIPLIER_BLOCK_HPP_
#define _TMULTIPLIER_BLOCK_HPP_

class IPort;
#include"TBlock.hpp"
#include"TPortOut.hpp"
#include"TPortIn.hpp"

/*!
 * \brief Блок, у которого есть два входа A и B, а на выходе он выдаёт A * B
 */
class TMultiplierBlock: public TBlock{
private:
	IData			*result;
	std::string		in1;
	std::string		in2;
	std::string		out;
public:
	//calculates all output ports for time t
	virtual IData&		simulate(int32_t t, int32_t ttl, TPortOut * port, std::string where){
					if (port==this->get_out_port(out)){
						int i,j;
						TPortIn *port_b = this->get_in_port(in1);
						TPortIn *port_c = this->get_in_port(in2);
						TPortOut *port_a = this->get_out_port(out);
						IData& b_data = port_b->get_value(t, ttl,  this->get_name()+" "+where);
						IData& c_data = port_c->get_value(t,ttl, this->get_name()+" "+where);
						{
							int dim_cnt = result->get_dim_count();
							int indexes[dim_cnt] = {0};
							do{
								data_type data_a = b_data.get_item(indexes, dim_cnt) *
										   c_data.get_item(indexes, dim_cnt);
								result->set_item(indexes, dim_cnt, data_a);
							}while(result->inc_index(indexes, dim_cnt));
						}
						return *result;
					} else {
						throw Tcnexception(where+": non-existing port requested");
					}
				};
				TMultiplierBlock(std::string sname,
				                 std::string in1="B",
				                 std::string in2="C",
				                 std::string out="A",
				                 IData& default_value=TPortOut::std_default_value):
				TBlock(sname),
				in1(in1),
				in2(in2),
				out(out),
				result(default_value.clone()){
					this->add_out_port(out, *result);
					this->add_in_port(in1, *result, 0);
					this->add_in_port(in2, *result, 0);
				};
	virtual			~TMultiplierBlock(){
					this->remove_port(out);
					this->remove_port(in1);
					this->remove_port(in2);
					delete result;
				};
};

#endif
