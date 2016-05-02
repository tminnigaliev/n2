#ifndef _TPORTOUT_HPP_
#define _TPORTOUT_HPP_
#include "IPort.hpp"
#include "config.hpp"
#include "TData.hpp"
#include <cstddef>

class TPortIn;

/*!
 * \brief Класс выходного порта
 */
class TPortOut: public IPort
{
private:
	IBlock			* block;
	TPortOut		* assigned_outer_port;
	TPortOut		* assigned_inner_port;
	IData&			default_value;
public:
				TPortOut(IBlock * my_block, std::string sname, IData& default_value = std_default_value):
					IPort(sname),
					block(my_block),
					assigned_outer_port(NULL),
					assigned_inner_port(NULL),
					default_value(default_value){};

	virtual IData&		get_value(const int32_t time, const int32_t ttl, std::string where);
	virtual IData&		get_sample_value() {return default_value;};
	virtual IPort		* get_other_port() {return NULL;};
	virtual PortType	get_port_type() {return PortType::PT_OUT;};
	virtual IBlock		* get_block(){return block;};
	virtual void		pipeline(TPortIn * other_port);
	virtual void		assign_inner(TPortOut * other_port);
	virtual void		unassign_inner();
	virtual bool		inner_assigned();
	virtual void		assign_outer(TPortOut * other_port);
	virtual void		unassign_outer();
	virtual bool		outer_assigned();
	static TData		std_default_value;
};

#endif
