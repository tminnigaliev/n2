#include <boost/test/minimal.hpp>
#include "IBasicBlock.hpp"

using namespace std;

/*!
 * Создаёт сеть для того, чтоб дальше можно было применять API библиотеки
 */
IBasicBlock& CreateNet()
{

}

/*!
 * Проверяет, что int-ы сохраняются корректно в TExClass1
 */
int CheckSavingInt(TExClass1 *c1_1, TExClass1 *c1_2)
{
#if 0
	//только для проверки, что тест работает корректно
	c1_1->test_field = 0;
#endif
	BOOST_CHECK( c1_1->test_field == c1_2->test_field );
	return 1;
}

/*!
 * Проверяет, что int-ы сохраняются корректно в TExClass2
 */
int CheckSavingInt(TExClass2 *c2_1, TExClass2 *c2_2)
{
#if 0
	//только для проверки, что тест работает корректно
	c1_2->another_test_field = 0;
#endif
	BOOST_CHECK( c2_1->another_test_field == c2_2->another_test_field );
	return 1;
}

/*!
 * Проверяет, что array-и сохраняются корректно в TExClass2
 */
int CheckSavingArray(TExClass2 *c2_1, TExClass2 *c2_2)
{
	BOOST_CHECK( c2_1->char_array[0] == c2_2->char_array[0] );
	BOOST_CHECK( c2_1->char_array[1] == c2_2->char_array[1] );
	BOOST_CHECK( c2_1->char_array[2] == c2_2->char_array[2] );
	return 1;
}

/*!
 * Проверяет, что struct-ы сохраняются корректно в TExClass2
 */
int CheckSavingStruct(TExClass2 *c2_1, TExClass2 *c2_2)
{
	BOOST_CHECK( c2_1->ex_struct.bit_field1 == c2_2->ex_struct.bit_field1 );
	BOOST_CHECK( c2_1->ex_struct.bit_field2 == c2_2->ex_struct.bit_field2 );
	BOOST_CHECK( c2_1->ex_struct.bit_field3 == c2_2->ex_struct.bit_field3 );
	string str1(c2_1->ex_struct.name);
	string str2(c2_2->ex_struct.name);
	BOOST_CHECK( str1 == str2 );
	BOOST_CHECK( c2_1->ex_struct.double_val == c2_2->ex_struct.double_val );
	return 1;
}

/*!
 * Проверяет, что сохраняется корректно int*
 */
int CheckSavingIntPtr(TExClass2 *c2_1, TExClass2 *c2_2)
{
	BOOST_CHECK( *(c2_1->int_ptr) == *(c2_2->int_ptr));
	return 1;
}

/*!
 * Проверяет корректность сохранения висячих указателей на int
 */
int CheckSavingIntPtrHang(TExClass2 *c2_1, TExClass2 *c2_2)
{
    BOOST_CHECK( *(c2_1->int_ptr_hang) == *(c2_2->int_ptr_hang));
    return 1;
}

/*!
 * Проверяет корректность сохранения нулевых указателей на int
 */
int CheckSavingIntPtrNull(TExClass2 *c2_1, TExClass2 *c2_2)
{
    BOOST_CHECK( c2_1->int_ptr_null == NULL);
    BOOST_CHECK( c2_2->int_ptr_null == NULL);
    return 1;
}

/*!
 * Проверяет корректность сохранения фиксированных указателей на int
 */
int CheckSavingIntPtrFixed(TExClass2 *c2_1, TExClass2 *c2_2)
{
    BOOST_CHECK( c2_1->int_ptr_fixed == c2_2->int_ptr_fixed);
    return 1;
}

/*!
 * Проверяет корректность сохранения указателей на другие классы
 */
int CheckSavingPtrToClass(TExClass1 *c1_1, TExClass1 *c1_2)
{
    BOOST_CHECK(
    	c1_1->ex_class3_ptr->ex_class1_ptr->test_field ==
    	c1_2->ex_class3_ptr->ex_class1_ptr->test_field);
    return 1;
}

/*!
 * Проверяет корректность сохранения ссылок на другие классы
 */
int CheckSavingRefToClass(TExClass1 *c1_1, TExClass1 *c1_2)
{
    BOOST_CHECK(
    	c1_1->ex_class3_ptr->ex_class2_ref.another_test_field ==
    	c1_2->ex_class3_ptr->ex_class2_ref.another_test_field);
    return 1;
}

int CheckSavingDescendantClassInstance(TExClass4 *c4_1, TExClass4 *c4_2)
{
	BOOST_CHECK(c4_1->one_more_test_field == c4_2->one_more_test_field);
	BOOST_CHECK(c4_1->test_field == c4_2->test_field);
	return 1;
}

int CheckSavingVirtualPublicDescendantClassInstance(TExClass5 *c5_1, TExClass5 *c5_2)
{
	TExClass1 *c1_1 = dynamic_cast<TExClass1*>(c5_1);
	TExClass1 *c1_2 = dynamic_cast<TExClass1*>(c5_2);
	TExClass2 *c2_1 = dynamic_cast<TExClass2*>(c5_1);
	TExClass2 *c2_2 = dynamic_cast<TExClass2*>(c5_2);

	CheckSavingInt(c1_1, c1_2);
    CheckSavingInt(c2_1, c2_2);
	CheckSavingArray(c2_1, c2_2);
	CheckSavingStruct(c2_1, c2_2);
	CheckSavingIntPtr(c2_1, c2_2);
	CheckSavingIntPtrHang(c2_1, c2_2);
	CheckSavingIntPtrNull(c2_1, c2_2);
	CheckSavingIntPtrFixed(c2_1, c2_2);
	CheckSavingPtrToClass(c1_1, c1_2);
	CheckSavingRefToClass(c1_1, c1_2);
	return 1;
}

/*!
 * Эта функция подменяет функцию int main(int argc, char*argv[]) потому что это юнит-тест
 */
int test_main( int, char * [])
{
#if 1
	cout << endl << endl << "classes dump before saving..." << endl << endl;
	list<ISaverLoader*>& obj_list = CreateList();
	DumpList(obj_list);
	SaveList(obj_list, "test.xml");
#endif
#if 1
	cout  << endl << endl << "classes dump after loading..." << endl << endl;
	list<ISaverLoader*> new_list;
	LoadList(new_list, "test.xml");
	DumpList(new_list);
#endif
#if 1
	list<ISaverLoader*>::iterator list_iter2;
	for (list<ISaverLoader*>::iterator list_iter1 = obj_list.begin(),
			                             list_iter2 = new_list.begin();
		 list_iter1 != obj_list.end();
		 list_iter1++, list_iter2++)
	{
		TExClass1* tmp1 = dynamic_cast<TExClass1*>(*list_iter1);
		TExClass1* tmp2 = dynamic_cast<TExClass1*>(*list_iter2);
		if (tmp1 != NULL)
		{
			CheckSavingInt(tmp1, tmp2);
			CheckSavingPtrToClass(tmp1, tmp2);
			CheckSavingRefToClass(tmp1, tmp2);
		}
		TExClass2* tmp3 = dynamic_cast<TExClass2*>(*list_iter1);
		TExClass2* tmp4 = dynamic_cast<TExClass2*>(*list_iter2);
		if (tmp3 != NULL)
		{
			CheckSavingInt(tmp3, tmp4);
			CheckSavingArray(tmp3, tmp4);
			CheckSavingStruct(tmp3, tmp4);
			CheckSavingIntPtr(tmp3, tmp4);
			CheckSavingIntPtrHang(tmp3, tmp4);
			CheckSavingIntPtrNull(tmp3, tmp4);
			CheckSavingIntPtrFixed(tmp3, tmp4);
		}
		TExClass4* tmp5 = dynamic_cast<TExClass4*>(*list_iter1);
		TExClass4* tmp6 = dynamic_cast<TExClass4*>(*list_iter2);
		if (tmp5 != NULL)
		{
			CheckSavingDescendantClassInstance(tmp5, tmp6);
		}
	}
#endif
    return 0;
}

#if 0
int main()
{
    try
    {
#if 0
    	list<ISaverLoader*>& obj_list = CreateList();
    	DumpList(obj_list);
    	SaveList(obj_list, "test.xml");

    	list<ISaverLoader*> new_list;
    	LoadList(new_list, "test.xml");
    	DumpList(new_list);
#endif
        cout << "Success\n";
    }
    catch (TSaverLoaderException &e)
    {
        cout << "Error: " << e.what() << "\n";
    }
    return 0;
}
#endif

