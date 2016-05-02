//#include "TBasicBlock.hpp"
//#include "TSimplePortIn.hpp"
#include "TConstBlock.hpp"
#include "TBlock.hpp"
#include "TMultiplierBlock.hpp"
#include "TPortOut.hpp"
#include <iostream>
#include "Tcnexception.hpp"
#include "TData.hpp"
#include "TDataVector.hpp"

using namespace std;

void mainloop()
{
	TBlock cplx_b("container");
	const int vector_size = 3;
	data_type vector_4[vector_size] = {4,5,6};
	data_type vector_10[vector_size] = {10,11,12};
	TDataVector Vector4(vector_4, vector_size);
	TDataVector Vector10(vector_10, vector_size);
	TConstBlock const1_b("vector_4", Vector4);
	TConstBlock const2_b("vector_10", Vector10);
	TMultiplierBlock mult_b("mult", "A", "B", "C", Vector4);

	TPortOut * cplx_b_port_a = cplx_b.add_out_port("A", Vector4);
	TPortIn * cplx_b_port_b = cplx_b.add_in_port("B", Vector4);
	TPortIn * cplx_b_port_c = cplx_b.add_in_port("C", Vector4);

	cplx_b.add_subblock(&mult_b, "multiplier");

	TPortIn * mult_b_port_b = mult_b.get_in_port("B");
	TPortIn * mult_b_port_c = mult_b.get_in_port("C");
	TPortOut * mult_b_port_a = mult_b.get_out_port("A");

	cplx_b_port_a->assign_inner(mult_b_port_a);
	cplx_b_port_b->assign_inner(mult_b_port_b);
	cplx_b_port_c->assign_inner(mult_b_port_c);

	//cplx_b.add_block(&const1_b, "const1 generator");
	TPortOut * const1_b_port_a = const1_b.get_out_port("A");
	TPortOut * const2_b_port_a = const2_b.get_out_port("A");

	const1_b_port_a->pipeline(cplx_b_port_b);
	const2_b_port_a->pipeline(cplx_b_port_c);

	//cplx_b_port_b->assign_inner(const_b_port_a);

	//TSimplePortOut * port = dynamic_cast<TSimplePortOut*>(cplx_b.get_port_by_name("B"));
	int t;
	for (t = 0; t < 10; t++)
	{
		//int value = port->get_value(t, 4);
		//int index = 0;
		int values[vector_size];
		int i;
		for (i = 0; i < vector_size; i++){
			values[i] = cplx_b_port_a->get_value(t, 20, "main loop").get_item(&i,1);
		}
		cout << "t=" << t << ":" << values[0] << ", " << values[1] << ", " << values[2] << endl;
	}
}

int main(int argc, char * argv[])
{
	try {
		mainloop();
	}
	catch (Tcnexception &e)
	{
		cout << e.what();
	}
	return 0;
}
