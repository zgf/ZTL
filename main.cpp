#include <list>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include "ztl_concept.h"
#include "ztl_type_traits.h"
#include "ztl_iterator_adapter.h"
#include "ztl_allocator.h"
#include <ctime>
#include <string>
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
//
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
//		bind
//		function
//	}
//	
//}
using std::cout;
using std::endl;
 typedef char(&rn3)[3];
 rn3& func()
{
	char a[3];
	
	rn3 r= a;
	cout << "r" << &r << endl;
	
	return r;
}
using namespace ztl::traits::type_traits;
union Class
{
	int a;
	int b;
};
int main()
{
	Class test;
	test.b = 10;
	using UnionPtr = int Class::*;
	UnionPtr p = &Class::b;
	cout<<test.*p;
	//std::string i[10];
	//fun(i);
	//rn3& i = func();
	//cout << &i << endl;
	//std::cout << ztl::traits::type_traits::is_base_of<BaseB, BaseA>::value;
	//using functype = int(BaseB::*);
	//decay
	/*auto i =&BaseB::fooFunc;
	std::cout << ztl::traits::type_traits::is_member_function_pointer<decltype(i)>::value << std::endl;
	std::cout << std::is_enum<Tag>::value<< std::endl;
	std::cout << ztl::traits::type_traits::is_class<Tag>::value << std::endl;*/
	
	//std::cout << bool_predicate<is_convertible<BaseB, BaseA>::value>::value<<std::endl;
	//std::cout << is_base_of<BaseB, BaseA>::value << std::endl;
	//std::cout<<ztl::traits::type_traits::is_convertible<long, int>::value;
	auto start1 = clock();
	//for(auto i = 40000; i < 5000000; i++)
	//{
	//	ztl::memory::allocator::allocator<int>::raw_allocate(i  / 40000 );
	//	//operator new(i / 10000/ 8 + 1);
	//}
	//auto end1 = clock();
	////std::cout << (double)end1 - start1 << std::endl;
	//auto start2 = clock();
	//for(auto i = 40000; i < 5000000; i++)
	//{
	//	//ztl::memory::allocator::allocator<int>::raw_allocate(i / 10000 / 8 + 1);
	//	alloc_r(i);
	//}
	//auto end2 = clock();
	////std::cout << (double)end2 - start2 << std::endl;
	//std::cout << ((double)end1 - start1)/((double)end2 - start2) << std::endl;

	//std::cout << (int)a1<<std::endl;
	//std::cout << (int)a2<<std::endl;
	//std::cout << (int)a2 - (int)a1;
	//std::string a;
	//ztl::Nullable<int*> test(new int(3));
	//ztl::Nullable<std::vector<int>> testb(ztl::null);
	//int* a = test;
	//test == ztl::null;
	/*try
	{
		std::vector<int> a = testb;
	}
	catch (ztl::Exception& e)
	{
		std::cout<<e.What();
	}
	*/

	//Vist<LexTag::Unkown> Visit3();
	//Visit3();
	//func<LexTag::CharSet>();
	//func<LexTag::Unkown>();
	ztl::wrapper::tuples::pair<int, int> ATest(4, 5);
	ztl::wrapper::tuples::pair<int, int> BTest(4, 5);
	/*if (ATest == BTest)
	{
		ATest.second = 6;
		if(ATest != BTest)
		{
		}
		BTest = ATest;
	}*/
	return 0;
}