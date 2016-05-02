#include "IBlock.hpp"
#include "TPortIn.hpp"
#include "TPortOut.hpp"
#include <iostream>
#include "Tcnexception.hpp"
#include <cstddef>

void TPortIn::assign_inner(TPortIn * other_port)
{
	if (this->assigned_inner_port != other_port) {
		if (!(this->get_sample_value().is_compatible(other_port->get_sample_value()))){
			throw Tcnexception("Error when trying to assign inner port ("+
			                   other_port->get_block()->get_name()+":"+
			                   other_port->get_name()+") to port "+
			                   this->get_block()->get_name()+":"+get_name());
		}
		this->assigned_inner_port = other_port;
		other_port->assign_outer(this);
	}
}

void TPortIn::unassign_inner()
{
	if (this->inner_assigned()) {
		TPortIn * other_port = this->assigned_inner_port;
		this->assigned_inner_port = NULL;
		other_port->unassign_outer();
	}
}

bool TPortIn::inner_assigned()
{
	return (this->assigned_inner_port != NULL);
}

void TPortIn::assign_outer(TPortIn * other_port)
{
	if (this->assigned_outer_port != other_port) {
		if (!(this->get_sample_value().is_compatible(other_port->get_sample_value()))){
			throw Tcnexception("Error when trying to assign outer port ("+
					   other_port->get_block()->get_name()+":"+
					   other_port->get_name()+") to port "+
					   this->get_block()->get_name()+":"+get_name());
		}
		unassign_outer();
		this->assigned_outer_port = other_port;
		other_port->assign_inner(this);
	}
}

void TPortIn::unassign_outer()
{
	if (this->outer_assigned()) {
		TPortIn * other_port = this->assigned_outer_port;
		this->assigned_outer_port = NULL;
		other_port->unassign_inner();
	}
}

bool TPortIn::outer_assigned()
{
	return (this->assigned_outer_port != NULL);
}

void TPortIn::pipeline(TPortOut * other_port){
	if (!(this->get_sample_value().is_compatible(other_port->get_sample_value()))){
		throw Tcnexception("Error when trying to "+
		                   this->get_block()->get_name()+":"+get_name()+".pipeline("+
		                   other_port->get_block()->get_name()+":"+
		                   other_port->get_name()+")");
	}
	this->other_port = other_port;
}

bool TPortIn::pipelined() {
	return (this->other_port != NULL);
}

IData& TPortIn::get_value(const int32_t time, const int32_t ttl, std::string where)
{
	where = this->get_name() + "(" + this->get_block()->get_name() +"->"+where;
	if (this->assigned_outer_port != NULL) {
		if (ttl > 0) {
			return this->assigned_outer_port->get_value(time, ttl-1, where);
		}
		else {
			//return 0;
			throw Tcnexception("ttl = 0 "+where);
		}
	} else {
		if (this->other_port) {
			if (ttl > 0) {
				return this->other_port->get_value(time-delay, ttl-1, where);
			} else {
				throw Tcnexception("ttl = 0 "+where);
			}
		} else {
			//std::cout << "?->" << where << std::endl;
			//return 0;
			throw Tcnexception("?->"+where);
		}
	}
}

IPort * TPortIn::get_other_port()
{
	return other_port;
}
