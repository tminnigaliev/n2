#ifndef _TWRITER_TO_OCT_MATRIX_HPP_
#define _TWRITER_TO_OCT_MATRIX_HPP_

class IPort;
#include"TBlock.hpp"
#include"TPortIn.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include "Tcnexception.hpp"

/*!
 * \brief записывает данные в матрицу в формате Октава.
 * Кол-во столбцов у матрицы всегда на 1 больше чем количество столбцов у данных
 * Первым столбцом всегда идёт время.
 * Упорядоченности по времени нет. Т.е. возможна такая матрица:
 * 3	10	11	15
 * 4	12	13	17
 * 1	6	7	11   (как пример, где 3,4 и 1 - это время)
 */
class TWriterToOctMatrix: public TBlock{
public:
	//calculates all output ports for time t
	virtual IData&		simulate(int32_t t, int32_t ttl, TPortOut * port, std::string where);
				TWriterToOctMatrix(std::string sname, std::string fname, IData& samp_value, std::string in="A"):
				TBlock(sname),
				in(in),
				filename(fname),
				rows(0),
				cols(samp_value.get_dim(0)+1), //value.get_dim(0)+1
				value(samp_value.clone()){
					this->add_in_port(in, samp_value, 0);
					write_header(0,0);
					clean_rows();
				};
	virtual			~TWriterToOctMatrix(){
					this->remove_port(in);
					delete value;
				};
private:
	IData			*value;
	std::string		in;
	std::string		filename;
	int			rows;
	int			cols;
	virtual void		write_header(uint32_t rows, uint32_t cols);
	virtual void		clean_rows();
	virtual void		write_row(int32_t, IData *vector);
};

#endif
