#include <iostream>
#include <string>
#include "ztl_concept.h"
#include "ztl_type_traits.h"
#include "ztl_iterator_adapter.h"
#include "ztl_allocator.h"
#include "ztl_wrapper.h"
#include "ztl_functional.h"
#include <tuple>
using std::cout;
using std::endl;
//约定 类型名 pascal式
// 类成员 驼峰式+前缀后缀
// 变量名 驼峰式+前缀后缀
// 常量 宏 全大写加下划线区分,null例外
// 函数名 pascal式
// 枚举值pascal式或者pascal+下划线
//template<typename T>
//void test(const T a)
//{
//	std::cout << "a";
//}
//template<typename T>
//class Test : public ztl::concept::concept_base::IEquality<Test<T>>
//{
//	T Data;
//	friend bool operator== (const Test& left, const Test& right)
//	{
//		return left.Data == right.Data;
//	}
//
//};
//namespace ztl
//{
//	namespace wrapper
//	{
//		namespace tuples
//		{
//			tuple完成
//			pair triple完成
//		}
//	}
//	namespace concept
//	{
//		namespace concept_base
//		{
//
//		}
//		namespace concept_iterator
//		{
//
//		}
//		namespace concept_container
//		{
//
//		}
//		namespace concept_checker
//		{
//
//		}
//	}
//	namespace traits
//	{
//		namespace iterator_traits
//		{
//
//		}
//		namespace type_traits
//		{
//
//		}
//		namespace help_traits
//		{
//
//		}
//	}
//	namespace memory
//	{
//		namespace allocater
//		{
//			内存分配器
//		}
//		namespace management
//		{
//			智能指针
//			C++ GC
//		}
//	}
//	namespace container
//	{
//
//	}
//	namespace iterator
//	{
//
//	}
//	namespace adapter
//	{
//		namespace adapter_iterator
//		{
//			finish
//		}
//		namespace adapter_container
//		{
//			slist
//			优先队列
//			stack
//		}
//	}
//	namespace  algorithm
//	{
//		LINQ
//		不变
//		变化
//	}
//	namespace functional
//	{
//		bind 完成
//		function 完成
//	}
//	
//}
#include <functional>
class A
{
public:
	int func(int a)
	{
		return 1;
	}
};
struct Foo
{
	Foo(int num) : num_(num)
	{
	}
	int   print_add(int i) 
	{
		std::cout << num_ + i << '\n';
		return 0;
	}
	int   print_n(int i,double b,float c,int d)
	{
		std::cout << num_ + i << '\n';
		return 0;
	}
	bool operator()(int a, int b)
	{
		return a == b;
	}
	int num_;
};

void print_num(int i)
{
	std::cout << i << '\n';
}
int test(double a)
{
	std::cout << a << std::endl;
	return 0;
}
void testb()
{
	std::cout << "succ";
}
bool testType(int& a, const std::string& b, double c, void* d)
{
	cout << a<<endl;
	cout << b << endl;
	cout << c << endl;
	cout << d << endl;
	return true;
}
double my_divide(double x, double y)
{
	return x / y;
}

struct MyPair
{
	double a, b;
	double multiply()
	{
		return a*b;
	}
};

using namespace ztl::traits::type_traits;
using namespace ztl::functional;
using namespace ztl::wrapper::tuples;
using std::cout;
using std::endl;
int main()
{
	
	/*Foo c(1);
	ztl::functional::function<bool(int, int)>b = &c;
	b(1, 3);
	auto i = [](int a, int b)->int
	{
		return a;
	};
	ztl::functional::function<int(int, int)>a = i;
	auto k = a(1, 3);
	


	ztl::functional::function<void(int)>b= &print_num;
	b(3);*/
	////ztl::functional::function<int(double)>c=&test;
	////c(4);
	////ztl::functional::function<int()>d=&testb;
	////d();
	ztl::functional::function<int(Foo*, int)>e = &Foo::print_add;
	Foo ef(1);
	e(&ef, 1);
	ztl::functional::function<decltype(&test)>g = &test;
	g(4);
	ztl::functional::function<bool(Foo*, int, int)>h = &Foo::operator();
	Foo hf(1);
	h(&hf, 3, 1);
	ztl::functional::function<decltype(&Foo::print_add)>er;
	er = &Foo::print_add;
	Foo eg(1);
	er(&eg, 1);

	ztl::functional::function<void(int)> f_display = &print_num;
	f_display(-9);

	//// 保存 lambda 表达式
	//ztl::functional::function<void()> f_display_42 = []()
	//{
	//	print_num(42);
	//};
	//f_display_42();

	
	 //保存成员函数
	ztl::functional::function<void(const Foo*, int)> f_add_display = &Foo::print_add;
	const Foo foo(314159);
	f_add_display(&foo, 1);


	/*typename ztl::traits::type_traits::if_else<
		is_placeholder<typename tuple_index_type<index,
		BindType>::type>::value,
		typename tuple_index_type<
		is_placeholder<typename tuple_index_type<index
		, BindType>::type>::arg_index - 1,
		CallType>::type,
		typename tuple_index_type<index, BindType>::type>::type*/
	static  int index = 2;
	
	
	Foo a(3);
	//auto bindList = make_tuple(&a, ztl::functional::_1);
	//auto callList = ztl::wrapper::tuples::make_tuple(3);

	//cout << (select_index_value<2>(bindList, callList) == 2.9) << endl;
//	select_value<2>(bindList, callList, ztl::traits::type_traits::true_type());
	//std::cout<<std::is_same<typename std::remove_reference<decltype(ituple.get<0>())>::type,const char[2]>::value;
	
	auto i = ztl::functional::bind(&Foo::print_n, &a, ztl::functional::_1, ztl::functional::_2, ztl::functional::_3, 3);
	i(3, 4, (float)5);
	//ztl::functional::bind(&testType, index, _1, _2, _3)(std::string("vvv"), 2.0, (void*)0x11111);
	testType(index, std::string("vvv"), 2.0, (void*)0x11111);
	//auto i =select_index_value<0>(bindList, callList);
	//cout << (select_index_value<0>(bindList, callList) == &a) << endl;
//	cout << (select_index_value<1>(bindList, callList) == 3) << endl;
	/*cout << (select_index_value<2>(bindList, callList) == 1) << endl;
	cout << (select_index_value<3>(bindList, callList) == 1) << endl;
	cout << (select_index_value<4>(bindList, callList) == 1) << endl;*/
	//ztl::functional::function<decltype(&Foo::print_n)> test(&Foo::print_n);
	//test(select_index_value<0>(bindList, callList), select_index_value<1>(bindList, callList)/*, select_index_value<2>(bindList, callList), select_index_value<3>(bindList, callList), select_index_value<4>(bindList, callList)*/);
	const int& tb = 10;
	tuple_value<decltype(tb)> ta(tb);

	// binding functions:
	auto fn_five = bind(&my_divide, 10, 2);               // returns 10/2
	std::cout << fn_five() << '\n';                          // 5

	auto fn_half = bind(&my_divide, _1, 2);               // returns x/2
	std::cout << fn_half(10) << '\n';                        // 5

	auto fn_invert = bind(&my_divide, _2, _1);            // returns y/x
	std::cout << fn_invert(10, 2) << '\n';                    // 0.2

	auto fn_rounding = bind(&my_divide, _1, _2);     // returns int(x/y)
	std::cout << fn_rounding(10, 3) << '\n';                  // 3
	function<decltype(&my_divide)> aaa = fn_rounding;
	aaa(10, 3);
	MyPair ten_two{10, 2};

	// binding members:
	auto bound_member_fn = bind(&MyPair::multiply, _1); // returns x.multiply()
	std::cout << bound_member_fn(&ten_two) << '\n';           // 20
	Foo f_te(10);
	bind(&f_te, 1, 3)();
	//auto bound_member_data = bind(&MyPair::a, ten_two); // returns ten_two.a
	//std::cout << bound_member_data() << '\n';                // 10
	return 0;
}
