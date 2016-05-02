#ifndef TDATA_HPP
#define TDATA_HPP
#include "config.hpp"
#include "IData.hpp"


class TData: public IData{
private:
	data_type		value;
public:
	virtual	int		get_dim_count() {return 1;};
	virtual	int		get_dim(int dim_idx) {if (dim_idx == 0) return 1; else return 0;};
	virtual const data_type*get_ptr_to_dump() {return &value;};
	//virtual int		inc_index(int indexes_array[], int array_cnt); inherited from IData
	virtual const data_type	get_item(int indexes_array[], int array_cnt){
					return value;
				};
	virtual void		set_item(int indexes_array[], int array_cnt, data_type data){
					value = data;
				};
	virtual 		~TData(){};
				TData(data_type value):value(value){};
	virtual IData*		clone(){
					TData * tmp = new TData(value);
					return tmp;
				}
};

#endif
