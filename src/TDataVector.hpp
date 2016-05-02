#ifndef _TDATAVECTOR_HPP_
#define _TDATAVECTOR_HPP_
#include "config.hpp"
#include "IData.hpp"

class TDataVector: public IData{
private:
	int dim;
	data_type		*values;
public:
	virtual	int		get_dim_count() {return 1;};
	virtual	int		get_dim(int dim_idx) {if (dim_idx == 0) return dim; else return 0;};
	virtual const data_type*get_ptr_to_dump() {return values;};
	//virtual int		inc_index(int indexes_array[], int array_cnt); inherited from IData
	virtual const data_type	get_item(int indexes_array[], int array_cnt){
					if (array_cnt == 1){
						if (dim > indexes_array[0]){
							return values[indexes_array[0]];
						} else {
							throw Tcnexception("requested item with non-existing index");
						}
					} else {
						throw Tcnexception("provided wrong indexes array (amount of dimensions > 1 for TDataVector)");
					}
					throw Tcnexception("Shouldn't come to this point");
				};
	virtual void		set_item(int indexes_array[], int array_cnt, data_type data){
					if (array_cnt == 1){
						if (dim > indexes_array[0]){
							values[indexes_array[0]] = data;
							return;
						} else {
							throw Tcnexception("requested item with non-existing index");
						}
					} else {
						throw Tcnexception("provided wrong indexes array (amount of dimensions > 1 for TDataVector)");
					}
					throw Tcnexception("Shouldn't come to this point");
				};
	virtual 		~TDataVector(){
					delete[] values;
				};
				TDataVector(int size):
					dim(size){
					values=new data_type[size];
				};
				TDataVector(data_type *data, int array_cnt):
					dim(array_cnt){
					values=new data_type[array_cnt];
					int i;
					for (i = 0; i < array_cnt; i++){
						values[i] = data[i];
					}
				}
	virtual IData*		clone(){
					TDataVector * tmp = new TDataVector(values, dim);
					return tmp;
				}
};
#endif
