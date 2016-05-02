#ifndef _TPORTIN_HPP_
#define _TPORTIN_HPP_

#include "IPort.hpp"
#include "TPortOut.hpp"
#include "config.hpp"
#include "IData.hpp"
#include <cstddef>

class TPortIn;
class TPortOut;
class IBlock;

/*!
 * \brief Класс входного порта
 */
class TPortIn: public IPort
{
protected:
/*!
 *   +-Block A-------------------------------------------------------------
 *   |               +-Block B---------------------------------------------
 *   |               |                 +-Block C (simple)------------------
 *   |               |                 |
 *  ->-in_port_K----->-in_port_L------->-in_port_M      does
 *   |aip=in_port_L  | aip=in_port_M   | aip=NULL       simulation
 *   |               | aop=in_port_L   | aop=in_port_L  puts result
 *   |               |                 |                to output
 *   |               |                 |                port
 *
 *   aip = assigned_inner_port
 *   aop = assigned_outer_port
 */
	TPortIn			* assigned_inner_port;
	TPortIn 		* assigned_outer_port;
/*!
 *   ---Block A----+     +-Block B------
 *                 |     |
 *     out_port_K-->----->-in_port_L
 *     aip=NULL    |     | aip=NULL
 *     aop=NULL    |     | aop=NULL
 *     op=in_port_L|     | op=in_port_K
 *   --------------+     +--------------
 *   aip = assigned_inner_port
 *   aop = assigned_outer_port
 *   op = other_port
 */
	TPortOut 		* other_port;
/*!
 * задержка по времени, которую вносит входной порт при расчётах
 */
	int32_t			delay;
/*!
 * Указатель на родительский блок
 */
	IBlock			* block;
private:
	IData&			sample_data;
public:
				TPortIn(IBlock * my_block, int32_t delay, std::string name, IData& sample_data = TPortOut::std_default_value):
					IPort(name),
					assigned_inner_port(NULL),
					assigned_outer_port(NULL),
					other_port(NULL),
					delay(delay+1),		//PortIn always delays by at least 1 tick
					block(my_block),
					sample_data(sample_data){};

	virtual			~TPortIn(){};
	virtual PortType	get_port_type() {return PortType::PT_IN;};
	virtual IPort		* get_other_port();
	virtual IData&		get_value(const int32_t time, const int32_t ttl, std::string where);
	virtual IData&		get_sample_value() {return sample_data;};
	virtual IBlock		* get_block(){return block;};
	virtual void		pipeline(TPortOut * other_port);
	virtual void		unpipeline() {this->other_port = NULL;};
	virtual bool		pipelined();
	virtual void		assign_inner(TPortIn * other_port);
	virtual void		unassign_inner();
	virtual bool		inner_assigned();
	virtual void		assign_outer(TPortIn * other_port);
	virtual void		unassign_outer();
	virtual bool		outer_assigned();
};

#endif

