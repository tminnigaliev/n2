#include "TBlock.hpp"
#include "TKohonen.hpp"
#include "Tcnexception.hpp"
#include <cmath>
#include <boost/foreach.hpp>
#include <iostream>

#define MIN(A, B) ((A) < (B) ? (A) : (B))

IData& TKohonen::simulate(int32_t t, int32_t ttl, TPortOut * port, std::string where)
{
	if (port==this->get_out_port(out)){
		int i,j;
		TPortIn *port_in = this->get_in_port(in);
		TPortOut *port_out = this->get_out_port(out);
		IData& in_data = port_in->get_value(t, ttl,  this->get_name()+" "+where);
		{
			int out_cnt = outputs->get_dim(0);
			int max_out_number = -1;
			int in_cnt = weights->get_dim(1);
			data_type alpha_in[in_cnt];
			for (j = 0; j < in_cnt; j++) {
				alpha_in[j] = ((port_in->get_value(t, ttl, where)).get_item(&j, 1) * alpha + (dt_one - alpha)/sqrt(in_cnt));

			}
			weights->dump(&std::cout);
			data_type max_out_value = 0;
			data_type outs[out_cnt] = {0};
			for (i = 0; i < out_cnt; i++) {
				for (j = 0; j < in_cnt; j++) {
					int indexes[2] = {i, j};
					outs[i] += weights->get_item(indexes, 2) * alpha_in[j];
				}
			}
			for (i = 0; i < out_cnt; i++) {
				if (outs[i] > max_out_value){
					max_out_number = i;
					max_out_value = outs[i];
				}
			}
			for (i = 0; i < out_cnt; i++) {
				outputs->set_item(&i, 1, 0);
			}
			outputs->set_item(&max_out_number, 1, 1);
			for (j = 0; j < in_cnt; j++) {
				int indexes[2] = {max_out_number, j};
				data_type old_weight = weights->get_item(indexes, 2);
				data_type new_weight = old_weight * (1.0 - flearning_rate) + alpha_in[j] * flearning_rate;// * ((float)max_out_number+1.0)/((float)(out_cnt));
				weights->set_item(indexes, 2, new_weight);
			}
			//alpha = 1.0 - sqrtf(1.0 - alpha - falpha_step);
			alpha = MIN(1.0, alpha+falpha_step);
		}
		return *outputs;
	} else {
		throw Tcnexception(where+": non-existing port requested");
	}
}

void TKohonen::save_to_ptree(boost::property_tree::ptree &pt, std::string where)
{
	std::string this_name = where+"."+this->get_name();
	std::string weight_name = this_name+"."+"weights";
	int neuron_cnt = outputs->get_dim(0);
	int weight_cnt = weights->get_dim(1);
	int byte_len = neuron_cnt * weight_cnt * sizeof(*weights->get_ptr_to_dump());
	const char *current_char = reinterpret_cast<const char*>(weights->get_ptr_to_dump());
	int i,j;
	char buffer[10];
        pt.put(this_name+".neuron_cnt", neuron_cnt);
	pt.put(this_name+".weight_cnt", weight_cnt);
	pt.put(this_name+".alpha", alpha);
        for (i = 0; i < neuron_cnt; i++)
        {
        	for (j = 0; j < weight_cnt; j++) {
        		buffer[0] = 0;
        		sprintf(buffer, "%d_%d", i, j);
        		int indexes[] = {i,j};
        	        pt.put(this_name+".elem_"+buffer, (weights->get_item(indexes, 2)));
        	}
        }
}

void TKohonen::load_from_ptree(boost::property_tree::ptree &pt, std::string where)
{
	std::string this_name = where+"."+get_name();
	std::string weight_name = this_name+"."+"weights";
	int neuron_cnt = outputs->get_dim(0);
	int weight_cnt = weights->get_dim(1);
	int i, j;
	char buffer[10];
	pt.get(this_name+".alpha", alpha);
        for (i = 0; i < neuron_cnt; i++)
        {
        	for (j = 0; j < weight_cnt; j++) {
        		buffer[0] = 0;
        		sprintf(buffer, "%d_%d", i, j);
        		int indexes[] = {i,j};
        		data_type weight;
        	        weight = pt.get<data_type>(this_name+".elem_"+buffer);
        	        weights->set_item(indexes, 2, weight);
        	        //double pi = pt.get<double>("pi");
        	}
        }
}

TKohonen::TKohonen(std::string sname, std::string inp_name, std::string out_name,
                   IData& default_inp_value, IData& default_out_value, double alpha_step, double learning_rate):
	TBlock(sname),
	in(inp_name),
	out(out_name),
	default_in(default_inp_value.clone()),
	default_out(default_out_value.clone()),
	falpha_step(alpha_step),
	flearning_rate(learning_rate){
	int i, j;
	int neuron_cnt = default_out_value.get_dim(0);
	int inp_cnt = default_inp_value.get_dim(0);
	int indexes[2];
	weights = new TDataMatrix(neuron_cnt, inp_cnt);
	outputs = new TDataVector(neuron_cnt);
	alpha = 0.0;
	for (i = 0; i < neuron_cnt; i++){
		for (j = 0; j < inp_cnt; j++){
			indexes[0] = i;
			indexes[1] = j;
			weights->set_item(indexes, 2, (data_type)(sqrt(((double)(dt_one))/((double)(inp_cnt)))));
		}
	}
	this->add_out_port(out, *default_out);
	this->add_in_port(in, *default_in, 0);
};

TKohonen::~TKohonen(){
	delete weights;
	delete outputs;
};

