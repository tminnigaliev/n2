#include "TReaderFromOctMatrix.hpp"
#include <limits>

void TReaderFromOctMatrix::pre_read()
{
	std::string s;
	std::getline(data_file, s);		//# Created by Octave 4.0.0, Sun Mar 06 16:39:31 2016 PST <TMINNIGALIEV-W7+tminnigaliev@tminnigaliev-w7>
	//std::cout << ".1:" << s << std::endl;
	std::getline(data_file, s);		//# name: a
	//std::cout << ".2:" << s << std::endl;
	std::string tmp;
	std::string type;
	data_file >> tmp >> tmp >> type;	//# type: matrix
	//std::cout << ".3:" << type << std::endl;
	if (type == "matrix") {
		data_file >> tmp >> tmp >> this->rows; //# rows: 4
		//std::cout << ".4:" << this->rows << std::endl;
		data_file >> tmp >> tmp >> this->cols;  //# columns: 4
		//std::cout << ".5:" << this->cols << std::endl;
	}
}

IData& TReaderFromOctMatrix::read_row(int32_t t, std::string where)
{
	int file_position = 0;
	int i;
	if (this->rows < 0) pre_read();
	data_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	if (this->pos_cache->find(t) != this->pos_cache->end()) {
		file_position = (*this->pos_cache)[t];
		data_file.seekg(file_position);
	} else {
		std::string s;
		char line[256];
		if (this->biggest_known_row_number < 0) {
			for (i = 0; i <= t; i++) {
				if (!data_file.eof()) {
					int position =  data_file.tellg();
					(*pos_cache)[i] = position;
					this->biggest_known_row_number = i;
				} else {
					throw Tcnexception(this->get_name()+": requested row doesn't exist" + where);
				}
				if (i == t) break;
				//data_file.getline(line, 256);
				data_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		} else {
			data_file.seekg((*pos_cache)[this->biggest_known_row_number]);
			for (i = this->biggest_known_row_number; i <= t; i++) {
				if (!data_file.eof()) {
					int position =  data_file.tellg();
					(*pos_cache)[i] = position;
					this->biggest_known_row_number = i;
				} else {
					throw Tcnexception(this->get_name()+": requested row doesn't exist" + where);
				}
				if (i == t) break;
				//data_file.getline(line, 256);
				data_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		}
	}
	for (i = 0; i < this->cols; i++) {
		data_type val;
		data_file >> val;
		value->set_item(&i, 1, val);
	}
	return *value;
}
