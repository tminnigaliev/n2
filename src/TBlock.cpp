#include "TPortOut.hpp"
#include "TPortIn.hpp"
#include "TBlock.hpp"
#include <map>
#include <iostream>
#include "Tcnexception.hpp"

/*!
 * Добавляет входной порт к блоку
 * \returns		указатель на экземпляр входного порта
 * \param[in] name	имя вновь создаваемого порта
 * \param[in] delay	задержка по времени, которую будет вносить вновь создаваемый порт
 */
TPortIn *TBlock::add_in_port(std::string name, IData& sample_value, int delay)
{
	TPortIn *port = new TPortIn(this, delay, name, sample_value);
	this->in_ports->push_back(port);
	this->str_to_port->insert(make_pair(name, port));
	//this->port_to_str->insert(make_pair(port, name));
	return port;
}

/*!
 * Добавляет выходной порт к блоку
 * \returns		указатель на экземпляр выходного порта
 * \param[in] name	имя вновь создаваемого порта
 */
TPortOut *TBlock::add_out_port(std::string name, IData& default_value)
{
	TPortOut *port = new TPortOut(this, name, default_value);
	this->out_ports->push_back(port);
	this->str_to_port->insert(make_pair(name, port));
	//this->port_to_str->insert(make_pair(port, name));
	return port;
}

/*!
 * Удаляет порт с заданным именем
 * \param[in] name	имя порта, который будем удалять
 */
void TBlock::remove_port(std::string name)
{
	IPort * port = (*this->str_to_port)[name];
	TPortIn * p_in = dynamic_cast<TPortIn*>(port);
	TPortOut * p_out = dynamic_cast<TPortOut*>(port);
	if (p_in) this->in_ports->remove(p_in);
	if (p_out) this->out_ports->remove(p_out);
	this->str_to_port->erase(name);
	//this->port_to_str->erase(port);
	delete port;
/*
	for (in_port_list_t::iterator list_iter = in_ports->begin();
	     list_iter != in_ports->end(); list_iter++)
	{
		//std::cout<<"type_id = "<<(*list_iter)->type_id<<endl;
		IPort *ptr1 = dynamic_cast<IPort*>(*list_iter);
		if (ptr1 == port){

		}
	}
*/
}

/*!
 * Добавляет дочерний суб-блок
 * \param[in] subblock	указатель на блок, который добавляем в качестве дочернего
 * \param[in] name	имя под которым добавляем этот подблок
 */
void TBlock::add_subblock(IBlock *subblock, std::string name)
{
	this->subblocks->push_back(subblock);
	this->str_to_block->insert(make_pair(name, subblock));
	//subblock->get_in_ports_list()
	//this->free_in_subports->merge(subblock->get_in_ports_list());
	//this->free_out_subports->merge(subblock->get_out_ports_list());
}

static bool datas_are_equal(IData& data1, IData& data2)
{
	if (data1.get_dim_count() == data2.get_dim_count()){
		int i;
		for (i = 0; i < data1.get_dim_count(); i++) {
			if (data1.get_dim(i) != data2.get_dim(i)){
				return false;
			}
		}
		return true;
	}
	return false;
}

void TBlock::connect_subblock_inputs_assign(std::string name)
{
	IBlock * isubblock = (*this->str_to_block)[name];
	TBlock * subblock = dynamic_cast<TBlock*>(isubblock);
	for (in_port_list_t::iterator list_iter = this->in_ports->begin();
	     list_iter != this->in_ports->end(); list_iter++)
	{
		TPortIn *my_port_in = dynamic_cast<TPortIn*>(*list_iter);
		if (!my_port_in->inner_assigned())
		{
			for (in_port_list_t::iterator list_iter_subblock = subblock->in_ports->begin();
			     list_iter_subblock != subblock->in_ports->end(); list_iter_subblock++)
			{
				TPortIn *subblock_port_in = dynamic_cast<TPortIn*>(*list_iter_subblock);
				if (!subblock_port_in->outer_assigned()){
					if ((subblock_port_in->get_name() == my_port_in->get_name()) &&
					    (datas_are_equal(my_port_in->get_sample_value(), subblock_port_in->get_sample_value()))) {
						my_port_in->assign_inner(subblock_port_in);
					}
				}
			}
		}
	}
}

void TBlock::connect_subblock_outputs_assign(std::string name)
{
	IBlock * isubblock = (*this->str_to_block)[name];
	TBlock * subblock = dynamic_cast<TBlock*>(isubblock);

	//TBlock * subblock = dynamic_cast<TBlock*>(*this->str_to_block[name]);//[name];
	for (out_port_list_t::iterator list_iter = this->out_ports->begin();
	     list_iter != this->out_ports->end(); list_iter++)
	{
		TPortOut *my_port_out = dynamic_cast<TPortOut*>(*list_iter);
		if (!my_port_out->inner_assigned())
		{
			for (out_port_list_t::iterator list_iter_subblock = subblock->out_ports->begin();
			     list_iter_subblock != subblock->out_ports->end(); list_iter_subblock++)
			{
				TPortOut *subblock_port_out = dynamic_cast<TPortOut*>(*list_iter_subblock);
				if (!subblock_port_out->outer_assigned()){
					if ((subblock_port_out->get_name() == my_port_out->get_name()) &&
					    (datas_are_equal(my_port_out->get_sample_value(), subblock_port_out->get_sample_value()))) {
						my_port_out->assign_inner(subblock_port_out);
					}
				}
			}
		}
	}
}

void TBlock::connect_subblock_pipeline(std::string name, bool connect_first, bool match_names)
{
	IBlock * isubblock_main = (*this->str_to_block)[name];
	TBlock * subblock_main = dynamic_cast<TBlock*>(isubblock_main);

	//TBlock * subblock = dynamic_cast<TBlock*>(*this->str_to_block[name]);//[name];
	for (subblock_list_t::iterator list_iter = this->subblocks->begin();
	     list_iter != this->subblocks->end(); list_iter++)
	{
		TBlock *subblock_current = dynamic_cast<TBlock*>(*list_iter);
		if (!(subblock_current == subblock_main))
		{
			/*!
			 * Соединяем все выходные порты под-блока с именем name с соответствующими входными портами других под-блоков
			 * того же уровня.
			 */
			for (out_port_list_t::iterator list_iter_out_main = subblock_main->out_ports->begin();
			     list_iter_out_main != subblock_main->out_ports->end(); list_iter_out_main++){
				TPortOut * main_port_out = dynamic_cast<TPortOut*>(*list_iter_out_main);
				for (in_port_list_t::iterator list_iter_in_current = subblock_current->in_ports->begin();
				     list_iter_in_current != subblock_current->in_ports->end(); list_iter_in_current++){
					TPortIn * current_port_in = dynamic_cast<TPortIn*>(*list_iter_in_current);
					if (!current_port_in->pipelined()){
						if ((!match_names) || (main_port_out->get_name() == current_port_in->get_name())) {
							if (datas_are_equal(main_port_out->get_sample_value(), current_port_in->get_sample_value())) {
								main_port_out->pipeline(current_port_in);
								if (connect_first) break;
							}
						}
					}
				}
				//сюда мы попадаем в случае, если connect_first == true и переходим к следующему выходному порту
				//подблока main.
			}

			/*!
			 * Соединяем все входные порты под-блока с именем name с соответствующими выходными портами других под-блоков
			 * того же уровня.
			 */
			for (in_port_list_t::iterator list_iter_in_main = subblock_main->in_ports->begin();
			     list_iter_in_main != subblock_main->in_ports->end(); list_iter_in_main++){
				TPortIn * main_port_in = dynamic_cast<TPortIn*>(*list_iter_in_main);
				if (!main_port_in->pipelined()){
					for (out_port_list_t::iterator list_iter_out_current = subblock_current->out_ports->begin();
					     list_iter_out_current != subblock_current->out_ports->end(); list_iter_out_current++){
						TPortOut * current_port_out = dynamic_cast<TPortOut*>(*list_iter_out_current);

						if ((!match_names) || (main_port_in->get_name() == current_port_out->get_name())) {
							if (datas_are_equal(main_port_in->get_sample_value(), current_port_out->get_sample_value())) {
								main_port_in->pipeline(current_port_out);
								break;
								//выходим в любом случае, т.к. к этому входному порту блока
								//main мы уже ничего подсоединить не сможем.
							}
						}
					}
				}
			}

		}
	}

	/*!
	 * Соединяем все входные порты под-блока с именем name с соответствующими выходными портами под-блока с именем name.
	 * (это сработает, если блок является рекурсивным, т.е. его входные и выходные порты имеют одинаковую сигнатуру данных
	 * и имена.
	 */

	for (in_port_list_t::iterator list_iter_in_main = subblock_main->in_ports->begin();
	     list_iter_in_main != subblock_main->in_ports->end(); list_iter_in_main++){
		TPortIn * main_port_in = dynamic_cast<TPortIn*>(*list_iter_in_main);
		if (!main_port_in->pipelined()){
			for (out_port_list_t::iterator list_iter_out_main = subblock_main->out_ports->begin();
			     list_iter_out_main != subblock_main->out_ports->end(); list_iter_out_main++){
				TPortOut * main_port_out = dynamic_cast<TPortOut*>(*list_iter_out_main);
				if ((!match_names) || (main_port_out->get_name() == main_port_in->get_name())) {
					if (datas_are_equal(main_port_out->get_sample_value(), main_port_in->get_sample_value())) {
						main_port_out->pipeline(main_port_in);
						break;
					}
				}
			}
		}
	}
}

void TBlock::connect_subblock(std::string name, TAutoConnectingType type)
{
	switch(type){
		case ACT_INPUTS_ASSIGN:
		connect_subblock_inputs_assign(name);
		break;
		case ACT_OUTPUTS_ASSIGN:
		connect_subblock_outputs_assign(name);
		break;
		case ACT_ONE_ONE_PIPELINE:
		connect_subblock_pipeline(name, true, true);
		break;
		case ACT_SCATTER_DIMS_PIPELINE:
		connect_subblock_pipeline(name, false, false);
		break;
		case ACT_SCATTER_PIPELINE:
		connect_subblock_pipeline(name, false, true);
		break;
	}
}

/*!
 * Удаляет подблок
 * \param[in]	subblock	указатель на блок, который мы хотим удалить.
 */
void TBlock::remove_block(IBlock * subblock)
{
	std::string name = subblock->get_name();
	this->subblocks->remove(subblock);
	//this->block_to_str->erase(subblock);
	this->str_to_block->erase(name);
#if 0
	for (in_port_list_t::iterator list_iter = subblock->get_in_ports_list().begin();
	     list_iter != subblock->get_in_ports_list().end(); list_iter++)
	{
		TPortIn *ptr1 = dynamic_cast<TPortIn*>(*list_iter);
		ptr1->unpipeline();
		ptr1->unassign_outer();
	}
#endif
}

/*!
 * Возвращает указатель на входной порт
 * \param[in] name	имя порта. Если входного порта с таким именем нет - вернёт NULL.
 */
TPortIn* TBlock::get_in_port(std::string name)
{
	return dynamic_cast<TPortIn*>((*(this->str_to_port))[name]);
}

/*!
 * Возвращает указатель на выходной порт
 * \param[in] name	имя порта. Если входного порта с таким именем нет - вернёт NULL.
 */
TPortOut* TBlock::get_out_port(std::string name)
{
	return dynamic_cast<TPortOut*>((*(this->str_to_port))[name]);
}

/*!
 * Производит симуляцию: если блок сложный, то симуляция по входящим в него блокам происходит автоматически
 * Если простой, то этот метод переопределён. Если метод не переопределён, то получим исключение.
 * \param[in] t		модельное время, для которого расчитываем результат симуляции
 * \param[in] ttl	параметр, позволяющий избежать зацикливания, убавляется при каждой новой рекурсии
 * \param[in] port	выходной порт для которого надо произвести симуляцию
 * \param[in] where	строка, которая позволяет локализовать ошибку во время симуляции
 * \param[in] index	индекс на случай, если выходной порт векторный
 */
IData& TBlock::simulate(int32_t t, int32_t ttl, TPortOut * port, std::string where)
{
	where = "?->" + where;
	//std::cout << "?->" << where << std::endl;
	throw Tcnexception(where);
	//return 0;
}

IData& TBlock::simulate_port_name(int32_t t, int32_t ttl, std::string port_name, std::string where)
{
	TPortOut *out = get_out_port(port_name);
	return simulate(t, ttl, out, where);
}

void TBlock::save_to_ptree(boost::property_tree::ptree &pt, std::string where)
{
	for (subblock_list_t::iterator list_iter = this->subblocks->begin();
	     list_iter != this->subblocks->end(); list_iter++)
	{
		TBlock *subblock_current = dynamic_cast<TBlock*>(*list_iter);
		subblock_current->save_to_ptree(pt, where+":"+this->get_name());
	}
}

void TBlock::load_from_ptree(boost::property_tree::ptree &pt, std::string where)
{
	for (subblock_list_t::iterator list_iter = this->subblocks->begin();
	     list_iter != this->subblocks->end(); list_iter++)
	{
		TBlock *subblock_current = dynamic_cast<TBlock*>(*list_iter);
		subblock_current->load_from_ptree(pt, where+":"+this->get_name());
	}
}

TBlock::TBlock(std::string sname):
	IBlock(sname){
	subblocks = new subblock_list_t();
	str_to_block = new name_to_block_map_t();
	in_ports = new in_port_list_t();
	free_in_subports = new in_port_list_t();
	out_ports = new out_port_list_t();
	free_out_subports = new out_port_list_t();
	str_to_port = new name_to_port_map_t();
};

TBlock::~TBlock(){
	for (in_port_list_t::iterator list_iter = in_ports->begin();
		list_iter != in_ports->end(); list_iter++)
	{
		IPort *ptr1 = dynamic_cast<IPort*>(*list_iter);
		delete ptr1;
	}
	for (out_port_list_t::iterator list_iter = out_ports->begin();
		list_iter != out_ports->end(); list_iter++)
	{
		IPort *ptr1 = dynamic_cast<IPort*>(*list_iter);
		delete ptr1;
	}
	delete(in_ports);
	delete(free_in_subports);
	delete(out_ports);
	delete(free_out_subports);
	delete(str_to_port);
};

