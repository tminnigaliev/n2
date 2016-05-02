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
#include "TReaderFromOctMatrix.hpp"
#include "TWriterToOctMatrix.hpp"
#include <cstdlib>
using namespace std;

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define WARN(cond)	do{ if (!(cond)) {std::cout<<ANSI_COLOR_YELLOW"condition "#cond" is not satisfied! at file: "<< __FILE__ << "; function: " << __func__ << "; line: " << __LINE__ << ANSI_COLOR_RESET << endl;} }while(0)
#define CHECK(cond)	do{ if (!(cond)) {std::cout<<ANSI_COLOR_RED"condition "#cond" failed! at file: "<< __FILE__ << "; function: " << __func__ << "; line: " << __LINE__ << ANSI_COLOR_RESET << endl;} }while(0)
#define REQUIRE(cond)	do{ if (!(cond)) {std::cout<<ANSI_COLOR_MAGENTA"fatal error: condition "#cond" failed! at file: "<< __FILE__ << "; function: " << __func__ << "; line: " << __LINE__ << ANSI_COLOR_RESET << endl; std::exit(-1);} }while(0)
#define PASSED		do{ std::cout << ANSI_COLOR_GREEN << __func__ << " passed" << ANSI_COLOR_RESET << endl;} while(0)
#define FAILED		do{ std::cout << ANSI_COLOR_RED << __func__ << " FAILED" << ANSI_COLOR_RESET << endl;} while(0)

/*!
 * простой тест для константного блока
 *
 *   +-const+
 *   |      |
 *   | 4    A-->
 *   |      |
 *   +------+
 */
void test_001()
{
	TData test_data(4);
        TConstBlock cb("const", test_data, "A");
        TPortOut * port = dynamic_cast<TPortOut*>(cb.get_out_port("A"));
        int t;
        int i = 0;
        for (t = 0; t < 10; t++)
        {
                int value = port->get_value(t, 4, __func__).get_item(&i, 1);
                REQUIRE(value == 4);
        }
        PASSED;
}

/*!
 * константный блок внутри сложного блока
 *
 *   +-complex block------+
 *   |  +-const-----+     |
 *   |  |           |     |
 *   |  | 4         A---->B--->
 *   |  |           |     |
 *   |  +-----------+     |
 *   +--------------------+
 */
void test_002()
{
	TData test_data(4);
	TBlock cplx_b("complex block");
	TConstBlock const_b("const", test_data, "A");
	TPortOut * cplx_b_port_b = cplx_b.add_out_port("B");
	cplx_b.add_subblock(&const_b, "const generator");
	TPortOut * const_b_port_a = dynamic_cast<TPortOut*>(const_b.get_out_port("A"));
	cplx_b_port_b->assign_inner(const_b_port_a);

	int t;
	int i = 0;
	for (t = 0; t < 10; t++)
	{
		int value = cplx_b_port_b->get_value(t, 20, __func__).get_item(&i, 1);
		REQUIRE(value == 4);
	}
	PASSED;
}

/*!
 *   +-const 4-+    +-complex block--------+
 *   |         |    |                      |
 *   | 4       A--->B-+   +-multiplier-+   |
 *   |         |    | |   |            |   |
 *   +---------+    | +-->B            |   |
 *                  |     |            A-->A-->
 *   +-const 6-+    | +-->C            |   |
 *   |         |    | |   |            |   |
 *   | 6       A--->C-+   +------------+   |
 *   |         |    |                      |
 *   +---------+    +----------------------+
 */
void test_003()
{
	TData test_data0(0);
	TData test_data4(4);
	TData test_data6(6);
	TBlock cplx_b("complex block");
	TConstBlock const1_b("const 4", test_data4, "A");
	TConstBlock const2_b("const 6", test_data6, "A");
	TMultiplierBlock mult_b("mult", "B", "C", "A", test_data0);

	TPortOut * cplx_b_port_a = cplx_b.add_out_port("A", test_data0);
	TPortIn * cplx_b_port_b = cplx_b.add_in_port("B", test_data0, 0);
	TPortIn * cplx_b_port_c = cplx_b.add_in_port("C", test_data0, 0);

	cplx_b.add_subblock(&mult_b, "multiplier");

	TPortIn * mult_b_port_b = mult_b.get_in_port("B");
	TPortIn * mult_b_port_c = mult_b.get_in_port("C");
	TPortOut * mult_b_port_a = mult_b.get_out_port("A");

	cplx_b_port_a->assign_inner(mult_b_port_a);
	cplx_b_port_b->assign_inner(mult_b_port_b);
	cplx_b_port_c->assign_inner(mult_b_port_c);

	TPortOut * const1_b_port_a = const1_b.get_out_port("A");
	TPortOut * const2_b_port_a = const2_b.get_out_port("A");

	const1_b_port_a->pipeline(cplx_b_port_b);
	const2_b_port_a->pipeline(cplx_b_port_c);

	int t;
	int i = 0;
	for (t = 0; t < 10; t++)
	{
		int value = cplx_b_port_a->get_value(t, 20, __func__).get_item(&i,1);
		REQUIRE(value == 24); //because 6 * 4 == 24
	}
	PASSED;
}

/*!
 *   +-const 4-+    +-complex block--------+
 *   |         | 3  |                      |
 *   | 4,5,6   A-/->B-+   +-multiplier-+   |
 *   |         |    | |   |            |   |
 *   +---------+    | +-->B            |   | 3
 *                  |     |            A-->A-/-> 40, 55, 72
 *   +-const 6-+    | +-->C            |   |
 *   |         | 3  | |   |            |   |
 *   |10,11,12 A-/->C-+   +------------+   |
 *   |         |    |                      |
 *   +---------+    +----------------------+
 */void test_004()
{
	TBlock cplx_b("container");
	const int vector_size = 3;
	data_type vector_4[vector_size] = {4,5,6};
	data_type vector_10[vector_size] = {10,11,12};
	TDataVector Vector4(vector_4, vector_size);
	TDataVector Vector10(vector_10, vector_size);
	TConstBlock const1_b("vector_4", Vector4, "A");
	TConstBlock const2_b("vector_10", Vector10, "A");
	TMultiplierBlock mult_b("mult", "B", "C", "A", Vector4);

	TPortOut * cplx_b_port_a = cplx_b.add_out_port("A", Vector4);
	TPortIn * cplx_b_port_b = cplx_b.add_in_port("B", Vector4, 0);
	TPortIn * cplx_b_port_c = cplx_b.add_in_port("C", Vector4, 0);

	cplx_b.add_subblock(&mult_b, "multiplier");

	TPortIn * mult_b_port_b = mult_b.get_in_port("B");
	TPortIn * mult_b_port_c = mult_b.get_in_port("C");
	TPortOut * mult_b_port_a = mult_b.get_out_port("A");

	cplx_b_port_a->assign_inner(mult_b_port_a);
	cplx_b_port_b->assign_inner(mult_b_port_b);
	cplx_b_port_c->assign_inner(mult_b_port_c);

	TPortOut * const1_b_port_a = const1_b.get_out_port("A");
	TPortOut * const2_b_port_a = const2_b.get_out_port("A");

	const1_b_port_a->pipeline(cplx_b_port_b);
	const2_b_port_a->pipeline(cplx_b_port_c);

	int t;
	for (t = 0; t < 10; t++)
	{
		int values[vector_size];
		int i;
		for (i = 0; i < vector_size; i++){
			values[i] = cplx_b_port_a->get_value(t, 20, __func__).get_item(&i,1);
		}
		REQUIRE(values[0] == 40); //4 * 10
		REQUIRE(values[1] == 55); //5 * 11
		REQUIRE(values[2] == 72); //6 * 12
	}
	PASSED;
}

/*!
 * Тест на автосоединение:
 *
 *  +-container-------------------------+
 *  |                                   |
 *  |  +const1--+      +mult----+       |
 *  |  |        |  3   |        |       |
 *  |  | 4,5,6  B--/-->B        |       |
 *  |  |        |      |        |       |
 *  |  +--------+      |        |   3   |   3
 *  |                  |        A---/-->A---/---> 40,55,72
 *  |  +const2--+      |        |       |
 *  |  |10,11,12|  3   |        |       |
 *  |  |        C--/-->C        |       |
 *  |  |        |      |        |       |
 *  |  +--------+      +--------+       |
 *  +-----------------------------------+
 */
void test_005()
{
	TBlock cplx_b("container");
	const int vector_size = 3;
	data_type vector_4[vector_size] = {4,5,6};
	data_type vector_10[vector_size] = {10,11,12};
	TDataVector Vector4(vector_4, vector_size);
	TDataVector Vector10(vector_10, vector_size);
	TConstBlock const1_b("vector_4", Vector4, "B");
	TConstBlock const2_b("vector_10", Vector10, "C");
	TMultiplierBlock mult_b("mult", "B", "C", "A", Vector4);
	TPortOut * cplx_b_port_a = cplx_b.add_out_port("A", Vector4);

	cplx_b.add_subblock(&mult_b, "multiplier");
	cplx_b.add_subblock(&const1_b, "const1");
	cplx_b.add_subblock(&const2_b, "const2");

	cplx_b.connect_subblock("multiplier", ACT_OUTPUTS_ASSIGN);
	cplx_b.connect_subblock("multiplier", ACT_ONE_ONE_PIPELINE);


	int t;
	for (t = 0; t < 10; t++)
	{
		int values[vector_size];
		int i;
		for (i = 0; i < vector_size; i++){
			values[i] = cplx_b_port_a->get_value(t, 20, __func__).get_item(&i,1);
		}
		REQUIRE(values[0] == 40); //4 * 10
		REQUIRE(values[1] == 55); //5 * 11
		REQUIRE(values[2] == 72); //6 * 12
	}
	PASSED;
}

void test_006()
{
	int t;
	int vector_size = 4;
	data_type vector_4[vector_size] = {0};
	TDataVector Vector4(vector_4, vector_size);
	TReaderFromOctMatrix matr_reader("reader", "data/a4x4real", 0, Vector4, "A");
	for (t = 0; t < 4; t++)
	{
		int values[vector_size];
		int i;
		for (i = 0; i < vector_size; i++){
			values[i] = matr_reader.simulate_port_name(t,10,"A",__func__).get_item(&i,1);
		}
		//std::cout << values[0] << " "<< values[1] << " " << values[2] << " " << values[3] << endl;
		REQUIRE(values[0] == t*4); //4 * 10
		REQUIRE(values[1] == t*4+1); //5 * 11
		REQUIRE(values[2] == t*4+2); //6 * 12
		REQUIRE(values[3] == t*4+3); //6 * 12
	}
	PASSED;
}

void test_007()
{
	int t;
	int vector_size = 3;
	data_type vector_4[vector_size] = {0};
	TDataVector Vector4(vector_4, vector_size);
	try{
		TReaderFromOctMatrix matr_reader("reader", "data/a4x4real", 0, Vector4, "A");
	} catch(Tcnexception &e) {
		PASSED;
		return;
	}
	FAILED;
}

void test_008()
{
	int t;
	TBlock cplx_b("container");
	const int vector_size = 3;
	data_type vector_4[vector_size] = {4,5,6};
	TDataVector Vector4(vector_4, vector_size);
	TConstBlock const1_b("vector_4", Vector4, "A");
	TWriterToOctMatrix matr_writer("writer", "data/write_test", Vector4, "A");
	cplx_b.add_subblock(&const1_b, "const 4");
	cplx_b.add_subblock(&matr_writer, "writer");
	cplx_b.connect_subblock("const 4", ACT_ONE_ONE_PIPELINE);

	for (t = 0; t < 4; t++)
	{
		matr_writer.simulate(t, 10, NULL, __func__);
	}
	PASSED;
#if 0
	int t;
	int vector_size = 3;
	data_type vector_3[vector_size] = {0};
	TDataVector Vector3(vector_3, vector_size);
	TWriterToOctMatrix matr_writer("writer", "data/write_test", Vector3, "A");
	TConstBlock const1_a("const 4", vector_3, "A");
	TBlock cplx_b("container");

	cplx_b.add_subblock(&const1_a, "const 4");
	cplx_b.add_subblock(&matr_writer, "writer");
	cplx_b.connect_subblock("const 4", ACT_ONE_ONE_PIPELINE);


	for (t = 0; t < 4; t++)
	{
		matr_writer.simulate(t, 10, NULL, __func__);
	}
	PASSED;
#endif
}

void mainloop()
{
	test_001();
	test_002();
	test_003();
	test_004();
	test_005();
	test_006();
	test_007();
	test_008();
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
