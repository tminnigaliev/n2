#include "TPortIn.hpp"
#include "TPortOut.hpp"
#include "IBlock.hpp"
#include "iostream"
#include "Tcnexception.hpp"

void TPortOut::assign_inner(TPortOut * other_port)
{
	if (assigned_inner_port != other_port) {
		if (!(this->get_sample_value().is_compatible(other_port->get_sample_value()))){
			throw Tcnexception("Error when trying to assign inner port ("+
					   other_port->get_block()->get_name()+":"+
					   other_port->get_name()+") to port "+
					   this->get_block()->get_name()+":"+get_name());
		}
		assigned_inner_port = other_port;
		other_port->assign_outer(this);
	}
};

void TPortOut::unassign_inner()
{
	if (assigned_inner_port != NULL) {
		TPortOut *other_port = assigned_inner_port;
		assigned_inner_port = NULL;
		other_port->unassign_outer();
	}
}

bool TPortOut::inner_assigned()
{
	return (this->assigned_inner_port != NULL);
}

void TPortOut::assign_outer(TPortOut * other_port)
{
	if (!(this->get_sample_value().is_compatible(other_port->get_sample_value()))){
		throw Tcnexception("Error when trying to assign outer port ("+
				   other_port->get_block()->get_name()+":"+
				   other_port->get_name()+") to port "+
				   this->get_block()->get_name()+":"+get_name());
	}
	if (this->assigned_outer_port != other_port) {
		unassign_outer();
		this->assigned_outer_port = other_port;
		other_port->assign_inner(this);
	}
}

void TPortOut::unassign_outer()
{
	if (this->assigned_outer_port != NULL) {
		TPortOut * other_port = this->assigned_outer_port;
		this->assigned_outer_port = NULL;
		other_port->unassign_inner();
	}
}

bool TPortOut::outer_assigned()
{
	return (this->assigned_outer_port != NULL);
}

IData& TPortOut::get_value(const int32_t time, const int32_t ttl, std::string where)
{
	TPortOut *port = this->assigned_inner_port;
	where = this->get_block()->get_name() +")"+ this->get_name()+ "->"+where;

	if (time < 0) {
#ifdef ERROR_ON_NEGATIVE_TIME
		throw Tcnexception("Negative time: "+where);
#else
		return this->fdefault_value;
#endif
	}

	if (port != NULL){
		if (ttl > 0) {
			return port->get_value(time, ttl-1, where);
		} else {
			throw Tcnexception("ttl = 0 " + where);
		}
	} else {
		if (ttl > 0) {
			return block->simulate(time, ttl-1, this, where);
		} else {
			throw Tcnexception("ttl = 0 " + where);
		}
	}
	throw Tcnexception("?->"+where);
}

void TPortOut::pipeline(TPortIn * other_port)
{
	if (!(this->get_sample_value().is_compatible(other_port->get_sample_value()))){
		throw Tcnexception("Error when trying to "+
		                   this->get_block()->get_name()+":"+get_name()+".pipeline("+
		                   other_port->get_block()->get_name()+":"+
		                   other_port->get_name()+")");
	}
	other_port->pipeline(this);
}

TData TPortOut::std_default_value(0);
