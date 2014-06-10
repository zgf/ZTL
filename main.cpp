#include <iostream>
#include <vector>
#include <numeric>
#include <string>
//#include <assert.h>
#include "container/ztl_string.h"
#include <algorithm>
#include <list>
#include <algorithm>
#include "container/ztl_deque.h"
#include "test/test_deque.h"
#include "boost/any.hpp"
#include "tuple/ztl_any.h"
//#include <windows.h>
//#include <sstream>
//#include <stdlib.h>
//#include "time/ztl_timer.h"
//#include <string>

//#include <functional>
//using std::bind;
//using std::function;
//#include "test/test_function.h"
//#include <tuple>
//#include "type/ztl_type_list.h"
//using std::cout;
//using std::endl;
////约定 类型名 pascal式
//// 类成员 驼峰式+前缀后缀
//// 变量名 驼峰式+前缀后缀
//// 常量 宏 全大写加下划线区分,null例外
//// 函数名 pascal式
//// 枚举值pascal式或者pascal+下划线
////template<typename T>
////void test(const T a)
////{
////	std::cout << "a";
////}
////template<typename T>
////class Test : public ztl::concept::concept_base::IEquality<Test<T>>
////{
////	T Data;
////	friend bool operator== (const Test& left, const Test& right)
////	{
////		return left.Data == right.Data;
////	}
////
////};
////namespace ztl
////{
////	namespace wrapper
////	{
////		namespace tuples
////		{
////			tuple完成
////			pair triple完成
////		}
////	}
////	namespace concept
////	{
////		namespace concept_base
////		{
////			INocopyable
////			IEquality
////			IOrdering
////			IAssignable
////		}
////		namespace concept_iterator
////		{
////
////		}
////		namespace concept_container
////		{
////
////		}
////		namespace concept_checker
////		{
////
////		}
////	}
////	namespace traits
////	{
////		namespace iterator_traits
////		{
////
////		}
////		namespace type_traits
////		{
////
////		}
////		namespace help_traits
////		{
////
////		}
////	}
////	namespace memory
////	{
////		namespace allocater
////		{
////			内存分配器
////		}
////		namespace management
////		{
////			智能指针
////			C++ GC
////		}
////	}
////	namespace container
////	{
////
////	}
////	namespace iterator
////	{
////
////	}
////	namespace adapter
////	{
////		namespace adapter_iterator
////		{
////			finish
////		}
////		namespace adapter_container
////		{
////			slist
////			优先队列
////			stack
////		}
////	}
////	namespace  algorithm
////	{
////		LINQ
////		不变
////		变化
////	}
////	namespace functional
////	{
////		bind 完成
////		function 完成
////	}
////
////}
//
//
//using namespace ztl::traits::type_traits;
//using namespace ztl::functional;
//using namespace ztl::wrapper::tuples;
using std::cout;
using std::endl;
//int main()
//{
//	return 0;
//
//}
//
//#include <boost/assert.hpp>
//
//#define BOOST_CHECK BOOST_ASSERT
//
//static int g_Aconstruct = 0;
//static int g_Acopy = 0;
//static int g_Amove = 0;
//static int g_MoveAmove = 0;
//static int g_MoveAcopy = 0;
//
//void reset_counter()
//{
//	g_Aconstruct = g_Acopy = g_Amove = g_MoveAmove = g_MoveAcopy = 0;
//}
//
//struct A
//{
//	explicit A(int i) : m_i(i)
//	{
//		++g_Aconstruct; /*Dump("A::construct " << m_i);*/
//	};
//	A(const A& other) : m_i(other.m_i)
//	{
//		++g_Acopy; /*Dump("A::copy " << m_i);*/
//	}
//	A(A&& other) : m_i(other.m_i)
//	{
//		other.m_i = 0; ++g_Amove; /*Dump("A::move " << m_i);*/
//	}
//	~A() = default;
//
//	mutable int m_i{0};
//	int f() const
//	{
//		//DumpX(m_i);
//		return (m_i *= 10);
//	}
//
//	int mem_func(int a, int b)
//	{
//		return a + b + m_i;
//	}
//
//	int mem_func_const(int a, int b) const
//	{
//		return a + b + m_i;
//	}
//
//	int mem_func_volatile(int a, int b) volatile
//	{
//		return a + b + m_i;
//	}
//};
//
//
//int func(A construct, A copy, A & lref, A const& clref, A && rref)
//{
//	construct.f();
//	copy.f();
//	lref.f();
//	clref.f();
//	rref.f();
//	A temp(std::move(rref));
//	return 0;
//}
//
//struct object_func
//{
//	inline void operator()(int& a, int b) const
//	{
//		a += b;
//	}
//};
//
//struct MoveA
//{
//	MoveA() = default;
//	MoveA(MoveA&&)
//	{
//		++g_MoveAmove; /*Dump("MoveA::move");*/
//	}
//	~MoveA() = default;
//
//	/// TODO: 支持vs2013 ctp
//	MoveA(MoveA const&)
//	{
//		++g_MoveAcopy;
//	}
//	//MoveA& operator=(MoveA const&) = default;
//	//MoveA& operator=(MoveA&&) = default;
//};
//
//int move_func(MoveA const& a)
//{
//	return 0;
//}
//
//void void_move_func(MoveA const& a)
//{
//}
//
//void test()
//{
//	{
//		/// 自由函数转发测试 (形参: 值传递, 左值引用, 常量左值引用, 右值引用)
//		reset_counter();
//		A a(1), b(2), c(3), d(4);
//		auto f = bind(&func, _1, _2, _3, /*_5*/std::ref(c), _4);
//		f(A(9), a, b, /*c,*/ move(d));
//		BOOST_CHECK(a.m_i == 1);
//		BOOST_CHECK(b.m_i == 20);
//		BOOST_CHECK(c.m_i == 30);
//		BOOST_CHECK(d.m_i == 0);
//
//		BOOST_CHECK(g_Aconstruct == 5);
//	//	BOOST_CHECK(g_Acopy == 1);
//	//	BOOST_CHECK(g_Amove == 2);
//	}
//
//	//////////////////////////////////////////////////////////////////////////
//	// @{ 非静态成员函数 (cv)
//	{
//		reset_counter();
//		A a(1);
//		auto f = bind(&A::mem_func, _1, 1, _2);
//		BOOST_CHECK(f(&a, 2) == 4);
//		BOOST_CHECK(g_Aconstruct == 1);
//		BOOST_CHECK(g_Acopy == 0);
//		BOOST_CHECK(g_Amove == 0);
//	}
//	{
//		reset_counter();
//		const A a(1);
//		auto f = bind(&A::mem_func_const, _1, 1, _2);
//		BOOST_CHECK(f(&a, 2) == 4);
//		BOOST_CHECK(g_Aconstruct == 1);
//		BOOST_CHECK(g_Acopy == 0);
//		BOOST_CHECK(g_Amove == 0);
//	}
//	{
//		reset_counter();
//		volatile A a(1);
//		auto f = bind(&A::mem_func_volatile, _1, 1, _2);
//		BOOST_CHECK(f(&a, 2) == 4);
//		BOOST_CHECK(g_Aconstruct == 1);
//		BOOST_CHECK(g_Acopy == 0);
//		BOOST_CHECK(g_Amove == 0);
//	}
//	{
//		reset_counter();
//		A a(2);
//		auto f = bind(&A::f, &std::ref(a));
//		f();
//	}
//	///// @}
//	////////////////////////////////////////////////////////////////////////////
//
//	////////////////////////////////////////////////////////////////////////////
//	///// @{ 仿函数
//	{
//		object_func obj;
//		auto f = bind(&obj, _1, 3);
//		int r = 1;
//		f(r);
//		BOOST_CHECK(r == 4);
//	}
//	///// @}
//	////////////////////////////////////////////////////////////////////////////
//
//	////////////////////////////////////////////////////////////////////////////
//	///// @{ 绑定非静态成员变量
//	{
//		/*reset_counter();
//		A a(2);
//		auto f = bind(&A::m_i, _1);
//		int r = f(&a);
//		BOOST_CHECK(r == 2);
//		BOOST_CHECK(g_Aconstruct == 1);
//		BOOST_CHECK(g_Acopy == 0);
//		BOOST_CHECK(g_Amove == 0);*/
//	}
//	///// @}
//	////////////////////////////////////////////////////////////////////////////
//
//	////////////////////////////////////////////////////////////////////////////
//	///// @{ 移动语义绑定参数
//	{
//		reset_counter();
//		MoveA a;
//		auto f = bind(&move_func, std::move(a));
//		f();
//		BOOST_CHECK(g_MoveAmove == 1);
//	}
//	{
//		reset_counter();
//		MoveA a;
//		auto f = bind(&void_move_func, std::move(a));
//		f();
//		BOOST_CHECK(g_MoveAmove == 1);
//	}
//	/// @}
//	//////////////////////////////////////////////////////////////////////////
//}
//
#include "time/ztl_timer.h"
#include "conversion/ztl_type_to_string.h"
//enum dadada
//{
//	huhuhu,
//};
//std::ofstream operator<<(std::ofstream os, ztl::string target)
//{
//	os << target.data();
//	return os;
//}
#include <memory>
#include "test/test_string.h"
#include <unordered_map>
#include "boost/smart_ptr/enable_shared_from_this.hpp"
class testA //:public boost::enable_shared_from_this<testA>
{
public:
	testA()
	{
		a = 0;
	}
	int get()
	{
		//auto p = shared_from_this();
		//boost::shared_ptr<testA> pp(this);
	
		return 0;
	}
	void operator()(const testA* )
	{

	}
	int a;
};
int main()
{
	//vector::emplace()
	/*int n = -1;
	ztl::vector<char> temp1(4,5);
	temp1.push_back(4);
	for(auto i = 0; i < 5;i++)
	{
		cout << temp1[i];
	}*/
	//TestFunction();
	//using container = ztl::type_list<int, double, int, float>;
	//cout << enum_to_string(dadada::huhuhu);
//	
//	ztl::Timer timer;
//	timer.Start();
////	//ztl::vector<char> StringBuff;
//	ztl::string StringBuff;
//	char* temp = new char[32];
//	ztl::string tempString;
//	//tempString.reserve(9999999 * 11);
//	for(auto i = 1; i < 9999999;i++)
//	{
//		
//		ztl::integal_to_string(i, temp);
		/*temp[0] = '9';
		temp[1] = '9';
		temp[2] = '9';
		temp[3] = '9';
		temp[4] = '9';
		temp[5] = '9';
		temp[6] = '9';
		temp[6] = '9';
		temp[6] = '\0';
*/
		//timer.Start();
		
		//StringBuff += temp;
		//cout << StringBuff;
//	//StringBuff.insert(StringBuff.end(), temp, temp + strlen(temp));
	//}
//	
//
//	
////	ztl::string String(std::move(StringBuff.data()));
	//timer.Stop();
//	
//	
//////	char* temp = new char[5];
////	//float a = 1.324f;
////	//ztl::type_to_string::to_string(a, 1, 3, temp);
	//输出运行时间（单位：s）
//	
	//cout << "ztl::string版本运行时间（单位：s）：" << timer.GetElapsedTime() << endl;


	// replace signatures used in the same order as described above:
	// Using positions:                 0123456789*123456789*12345
	test_string();

	char a1[100] = {1,0,3};
	const char* b1 = a1;
	ztl::string a2(b1, 0, 3);
	cout<<a2;
	/*testA b;
	{
	boost::shared_ptr<void> p(new testA,b);
	auto c = p;
	}*/
	return 0;
}
