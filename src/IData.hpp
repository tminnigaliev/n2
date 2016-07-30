#ifndef IDATA_HPP
#define IDATA_HPP
#include "config.hpp"
#include "TCnexception.hpp"
#include <string>
#include <iostream>

class IData{
public:
	virtual	int		get_dim_count() = 0;
	virtual	int		get_dim(int dim_idx) = 0;
	virtual const data_type*get_ptr_to_dump() = 0;
	virtual void		dump(std::ostream *outstream = &std::cout) = 0;
	virtual int		inc_index(int indexes_array[], int array_cnt){
					int i;
					int dim_cnt = get_dim_count();
					if (dim_cnt > array_cnt) throw Tcnexception("dimension of indexes array is less than actual");
					for (i = 0; i < dim_cnt; i++){
						indexes_array[i]++;
						if (indexes_array[i] < get_dim(i)) return 1;
						if (i < dim_cnt-1){
							indexes_array[i] = 0;
							continue;
						} else {
							//end of enumerating indexes
							return 0;
						}
					}
					throw Tcnexception("shouldn't come to this point");
				}
	virtual const data_type	get_item(int indexes_array[], int array_cnt) = 0;
	virtual void		set_item(int indexes_array[], int array_cnt, data_type data) = 0;
	virtual bool		is_compatible(IData& other_data){
					int i;
					int dim_cnt = get_dim_count();
					if (dim_cnt != other_data.get_dim_count()) return false;
					for (i = 0; i < dim_cnt; i++){
						if (get_dim(i) != other_data.get_dim(i)) return false;
					}
					return true;
				};
	virtual IData*		clone() = 0;
	virtual 		~IData(){};
				IData(){};
};

#endif
