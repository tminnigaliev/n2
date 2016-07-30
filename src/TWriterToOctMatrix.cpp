#include "TWriterToOctMatrix.hpp"
#include <limits>

void TWriterToOctMatrix::write_header(uint32_t rows, uint32_t cols)
{
	std::ofstream header_file;
	std::string extension(".hdr");
	std::string header_file_name = filename+extension;
	header_file.open(header_file_name.c_str(), std::ofstream::out | std::ofstream::trunc);
	std::string s1("# Created by Octave 4.0.0, Sun Mar 06 16:39:31 2016 PST <TMINNIGALIEV-W7+tminnigaliev@tminnigaliev-w7>");
	std::string s2("# name: "+get_name());
	std::string s3("# type: matrix");
	std::string s4("# rows: ");
	std::string s5("# columns: ");
	header_file << s1 << std::endl;
	header_file << s2 << std::endl;
	header_file << s3 << std::endl;
	header_file << s4 << rows << std::endl;
	header_file << s5 << cols << std::endl;
	header_file.close();
}

void TWriterToOctMatrix::clean_rows()
{
	std::ofstream rows_file;
	std::string rows_file_name = filename+".dat";
	rows_file.open(rows_file_name.c_str(), std::ofstream::out | std::ofstream::trunc);
	rows_file.close();
}

void TWriterToOctMatrix::write_row(int32_t t, IData *vector)
{
	std::ofstream data_file;
	std::string data_file_name = filename+".dat";
	int i;
	data_file.open(data_file_name.c_str(), std::ofstream::out | std::ofstream::app);
	data_file << t << " ";
	for (i = 0; i < this->cols-1; i++) {
		data_type val = vector->get_item(&i, 1);
		data_file << val << " ";
	}
	data_file << std::endl;
	data_file.close();
}

IData&	TWriterToOctMatrix::simulate(int32_t t, int32_t ttl, TPortOut * port, std::string where){

	TPortIn *port_a = this->get_in_port(in);
	IData& a_data = port_a->get_value(t, ttl,  this->get_name()+" "+where);
	//int dim_cnt = a_data.get_dim_count();
	rows++;
	write_header(rows, cols);
	write_row(t, &a_data);
	return a_data;
};
