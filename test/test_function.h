#pragma once
#include "../functional/ztl_functional.h"
#include "../tuple/ztl_tuple.h"
#include <iostream>
#include <string>
#include <functional>
using std::cout;
using std::endl;
using  ztl::function;
using  ztl::tuple;
using std::string;
/*
function语义,要实现对仿函数,对lambda,对普通函数,对成员函数指针的绑定和调用
形式:
仿函数,泛型仿函数
functor Foo;
function<functor*>(&Foo)();
普通函数
fuction<R(args...)>(&func)();
成员函数
function<R(callCalss*,args...)>(&Foo::func);
无状态lambda
function<return_type(args...)> =[](int,int)->int{return 0};
普通函数成员函数lambda的另外一种调用形式
function<fun_ptr>(&func);
make_function(fun_ptr func);
回去写测试文件

ztl::bind语义,高阶函数,传入的函数执行部分参数绑定,生成新函数对象,调用时传入剩余参数
ztl::bind(func_ptr,args...);
ztl::bind可以绑定仿函数,函数指针,
*/
class TestFunctor
{
public:
	template<typename... FuncArgs>
	bool operator()(double a, FuncArgs... Args)
	{
		//cout << a << "非const版本 泛型仿函数" << endl;
		return true;
	}
	template<typename... FuncArgs>
	bool operator()(double a, FuncArgs... Args)const
	{
		//cout << a << "const版本 泛型仿函数" << endl;
		return true;
	}
	bool operator()(string& a, const string& b)
	{
		//cout << a + b << "普通仿函数" << endl;
		return true;
	}
	bool operator()()
	{
		//cout << "普通仿函数无参数" << endl;
		return true;
	}
	int print_num(const double a, int b, size_t& c)
	{
		//cout << a << b << c << "普通成员函数" << endl;
		return a;
	}
	int print_void()
	{
		//cout << "空成员函数" << endl;
		return 0;
	}
};
int print_num_global(const double a, int b, size_t& c)
{
	//cout << a << b << c << "普通函数" << endl;
	return 0;
}
void print_void_global()
{
	//cout << "空函数" << endl;
}
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
		//std::cout << num_ + i << '\n';
		return 0;
	}
	int   print_n(int i, double b, float c, int d)
	{
		//std::cout << num_ + i << '\n';
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
	//std::cout << i << '\n';
}
int test(double a)
{
	//std::cout << a << std::endl;
	return 0;
}
void testb()
{
	//std::cout << "succ";
}
bool testType(int& a, const std::string& b, double c, void* d)
{
	//cout << a << endl;
	//cout << b << endl;
	//cout << c << endl;
	//cout << d << endl;
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
using namespace ztl;
void TestFunction()
{
	TestFunctor testClass;
	const TestFunctor const_testClass;
	double a = 2.0;
	int b = 4;
	size_t c = 6;
	function<int(const double, int, size_t&)>functor_one(&print_num_global);
	functor_one(a, b, c);
	function<decltype(&print_num_global)>functor_two(&print_num_global);
	functor_two(a, b, c);
	function<void()>functor_3(&print_void_global);
	functor_3();
	function<decltype(&print_void_global)>functor_4(&print_void_global);
	functor_4();

	function<decltype(&testClass)>functor_5(&testClass);
	functor_5();
	std::string d = "asdf";
	std::string e = "ghjk";
	functor_5(d, e);
	functor_5(a, e, d);
	function<decltype(&const_testClass)> functor_6 = &const_testClass;
	functor_6(a, e, d);
	function<bool(TestFunctor*)> functor_7;
	using mem_ptr_type = bool(TestFunctor::*)();
	mem_ptr_type mem_ptr = &TestFunctor::operator();
	functor_7 = mem_ptr;
	functor_7(&testClass);
	function<void(TestFunctor*)>functor_8(&TestFunctor::print_void);
	functor_8(&testClass);
	Foo ti(3);
	function<Foo*> tf = &ti;
	tf(2, 3);
	//funcaa(1, 2);
	/*Foo c(1);
	ztl::function<bool(int, int)>b = &c;
	b(1, 3);
	auto i = [](int a, int b)->int
	{
	return a;
	};
	ztl::function<int(int, int)>a = i;
	auto k = a(1, 3);

	ztl::function<void(int)>b= &print_num;
	b(3);*/
	////ztl::function<int(double)>c=&test;
	////c(4);
	////ztl::function<int()>d=&testb;
	////d();
	ztl::function<int(Foo*, int)>ed = &Foo::print_add;
	Foo ef(1);
	Foo* pe = &ef;
	(*pe)(2, 3);
	ed(&ef, 1);
	ztl::function<decltype(&test)>g = &test;
	g(4);

	ztl::function<bool(Foo*, int, int)>h = &Foo::operator();
	Foo hf(1);
	h(&hf, 3, 1);

	ztl::function<decltype(&Foo::print_add)>er;
	er = &Foo::print_add;
	Foo eg(1);
	er(&eg, 1);

	ztl::function<void(int)> f_display = &print_num;
	f_display(-9);

	//// 保存 lambda 表达式
	//ztl::function<void()> f_display_42 = []()
	//{
	//	print_num(42);
	//};
	//f_display_42();

	//保存成员函数
	ztl::function<void(const Foo*, int)> f_add_display = &Foo::print_add;
	const Foo foo(314159);
	f_add_display(&foo, 1);

	/*typename ztl::traits::type_traits::if_else<
	is_placeholder<typename tuple_element_type<index,
	BindType>::type>::value,
	typename tuple_element_type<
	is_placeholder<typename tuple_element_type<index
	, BindType>::type>::arg_index - 1,
	CallType>::type,
	typename tuple_element_type<index, BindType>::type>::type*/
	static  int index = 2;

	Foo aa(3);
	//	function<Foo*>kjk = &a;

	//std::ztl::bind(&Foo::operator(), a, 2, 3)();
	auto i = ztl::bind(&Foo::print_n, &aa, ztl::_1, ztl::_2, ztl::_3, 3);
	i(3, 4, (float)5);
	//auto bindList = make_tuple(&a, ztl::_1);
	//auto callList = make_tuple(3);

	//cout << (select_index_value<2>(bindList, callList) == 2.9) << endl;
	//	select_value<2>(bindList, callList, ztl::traits::type_traits::true_type());
	//std::cout<<std::is_same<typename std::remove_reference<decltype(ituple.get<0>())>::type,const char[2]>::value;

	auto jj = &ztl::bind(&Foo::print_n, &a, ztl::_1, ztl::_2, ztl::_3, 3);
	function<decltype(jj)> kk = jj;

	//ztl::ztl::bind(&testType, index, _1, _2, _3)(std::string("vvv"), 2.0, (void*)0x11111);
	testType(index, std::string("vvv"), 2.0, (void*)0x11111);
	//auto i =select_index_value<0>(bindList, callList);
	//cout << (select_index_value<0>(bindList, callList) == &a) << endl;
	//	cout << (select_index_value<1>(bindList, callList) == 3) << endl;
	/*cout << (select_index_value<2>(bindList, callList) == 1) << endl;
	cout << (select_index_value<3>(bindList, callList) == 1) << endl;
	cout << (select_index_value<4>(bindList, callList) == 1) << endl;*/
	//ztl::function<decltype(&Foo::print_n)> test(&Foo::print_n);
	//test(select_index_value<0>(bindList, callList), select_index_value<1>(bindList, callList)/*, select_index_value<2>(bindList, callList), select_index_value<3>(bindList, callList), select_index_value<4>(bindList, callList)*/);
	const int& tb = 10;
	ztl::tuple_value<decltype(tb)> ta(tb);

	// binding functions:
	auto fn_five = ztl::bind(&my_divide, 10, 2);               // returns 10/2
	std::cout << fn_five() << '\n';                          // 5

	auto fn_half = ztl::bind(&my_divide, _1, 2);               // returns x/2
	std::cout << fn_half(10) << '\n';                        // 5

	auto fn_invert = ztl::bind(&my_divide, _2, _1);            // returns y/x
	std::cout << fn_invert(10, 2) << '\n';                    // 0.2

	auto fn_rounding = ztl::bind(&my_divide, _1, _2);     // returns int(x/y)
	std::cout << fn_rounding(10, 3) << '\n';                  // 3
	function<decltype(&fn_rounding)> aaa = &fn_rounding;
	aaa(10, 3);
	MyPair ten_two{10, 2};

	// binding members:
	auto bound_member_fn = ztl::bind(&MyPair::multiply, _1); // returns x.multiply()
	std::cout << bound_member_fn(&ten_two) << '\n';           // 20
	Foo f_te(10);
	auto iv = ztl::bind(&f_te, 1, 3);
	function<decltype(&iv)> ivf = &iv;
	iv();

	auto bij = ztl::bind(&f_te, 3, _1);
	ztl::bind(&bij, 3)();
	auto jjk = &bij;
	function<decltype(jjk)> kjka(&bij);
	kjka(3);
	make_function(&bij)(3);
}