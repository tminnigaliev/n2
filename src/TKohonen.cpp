#include "TBlock.hpp"
#include "TKohonen.hpp"
#include "Tcnexception.hpp"

/*!
 * Производит симуляцию: если блок сложный, то симуляция по входящим в него блокам происходит автоматически
 * Если простой, то этот метод переопределён. Если метод не переопределён, то получим исключение.
 * \param[in] t		модельное время, для которого расчитываем результат симуляции
 * \param[in] ttl	параметр, позволяющий избежать зацикливания, убавляется при каждой новой рекурсии
 * \param[in] port	выходной порт для которого надо произвести симуляцию
 * \param[in] where	строка, которая позволяет локализовать ошибку во время симуляции
 * \param[in] index	индекс на случай, если выходной порт векторный
 */
IData& TKohonen::simulate(int32_t t, int32_t ttl, TPortOut * port, std::string where)
{
	where = "?->" + where;
	//std::cout << "?->" << where << std::endl;
	throw Tcnexception(where);
	//return 0;
}

void TKohonen::save_to_ptree(boost::property_tree::ptree &pt, std::string where)
{
	for (subblock_list_t::iterator list_iter = this->subblocks->begin();
	     list_iter != this->subblocks->end(); list_iter++)
	{
		TBlock *subblock_current = dynamic_cast<TBlock*>(*list_iter);
		subblock_current->save_to_ptree(pt, where+":"+this->get_name());
	}
}

void TKohonen::load_from_ptree(boost::property_tree::ptree &pt, std::string where)
{
	for (subblock_list_t::iterator list_iter = this->subblocks->begin();
	     list_iter != this->subblocks->end(); list_iter++)
	{
		TBlock *subblock_current = dynamic_cast<TBlock*>(*list_iter);
		subblock_current->load_from_ptree(pt, where+":"+this->get_name());
	}
}

TKohonen::TKohonen(std::string sname, int inp_cnt, std::string inp_name, int neuron_cnt, std::string out_name):
	TBlock(sname){
	int i, j;
	int indexes[2];
	weights = new TDataMatrix(neuron_cnt, inp_cnt);
	outputs = new TDataVector(neuron_cnt);
	for (i = 0; i < neuron_cnt; i++){
		for (j = 0; j < inp_cnt; j++){
			indexes[0] = i;
			indexes[1] = j;
			weights->set_item(indexes, 2, (data_type)(sqrt(((double)(dt_one))/((double)(inp_cnt)))));
		}
	}
};

TKohonen::~TKohonen(){

};

