#ifndef _TDATAMATRIX_HPP_
#define _TDATAMATRIX_HPP_
#include "config.hpp"
#include "IData.hpp"
#include <iostream>

class TDataMatrix: public IData{
private:
	int rows;
	int cols;
	data_type		*values;
public:
	virtual	int		get_dim_count() {return 2;};
	virtual	int		get_dim(int dim_idx) {if (dim_idx == 0) return rows; else if (dim_idx==1) return cols; else return 0;};
	virtual const data_type*get_ptr_to_dump() {return values;};
	virtual void 		dump(std::ostream *outstream = &std::cout) {
					int i;
					int j;
					for (i = 0; i < rows; i++) {
						for (j = 0; j < cols; j++) {
							int indexes[] = {i,j};
							*outstream << get_item(indexes, 2) << "  ";
						}
						*outstream << std::endl;
					}
					*outstream << std::endl;
				}
	//virtual int		inc_index(int indexes_array[], int array_cnt); inherited from IData
	virtual const data_type	get_item(int indexes_array[], int array_cnt){
					if (array_cnt == 1){
						if (rows*cols > indexes_array[0]){
							return values[indexes_array[0]];
						} else {
							throw Tcnexception("requested item with non-existing index");
						}
					} else if (array_cnt == 2){
						if (rows > indexes_array[0] && cols > indexes_array[1]){
							return values[indexes_array[0]*cols+indexes_array[1]];
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
						if (rows*cols > indexes_array[0]){
							values[indexes_array[0]] = data;
							return;
						} else {
							throw Tcnexception("requested item with non-existing index");
						}
					} else if (array_cnt == 2){
						if (rows > indexes_array[0] && cols > indexes_array[1]){
							values[indexes_array[0]*cols + indexes_array[1]] = data;
							return;
						} else {
							throw Tcnexception("requested item with non-existing index");
						}
					} else {
						throw Tcnexception("provided wrong indexes array (amount of dimensions > 1 for TDataVector)");
					}
					throw Tcnexception("Shouldn't come to this point");
				};
	virtual 		~TDataMatrix(){
					delete[] values;
				};
				TDataMatrix(int row_cnt, int col_cnt):
					rows(row_cnt),
					cols(col_cnt){
					values=new data_type[row_cnt * col_cnt];
				};
				TDataMatrix(data_type *data, int row_cnt, int col_cnt):
					rows(row_cnt),
					cols(col_cnt){
					values=new data_type[row_cnt*col_cnt];
					int i;
					for (i = 0; i < row_cnt*col_cnt; i++){
						values[i] = data[i];
					}
				}
	virtual IData*		clone(){
					TDataMatrix * tmp = new TDataMatrix(values, rows, cols);
					return tmp;
				}
};
#endif
