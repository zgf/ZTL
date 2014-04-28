
#include <list>
#include <iostream>
#include <algorithm>
#include <vector>
#include "ztl_concept.h"
#include "ztl_type_traits.h"
#include "ztl_iterator_adapter.h"
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
//
//		}
//		namespace management
//		{
//
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
//
//		}
//		namespace adapter_container
//		{
//
//		}
//	}
//	namespace  algorithm
//	{
//
//	}
//	namespace functional
//	{
//
//	}
//	
//}

int main()
{
	int a = 0;
	std::reverse_iterator
	//std::front_inserter
	//std::string a;
	//ztl::Nullable<int*> test(new int(3));
	//ztl::Nullable<std::vector<int>> testb(ztl::null);
	//int* a = test;
	//test == ztl::null;
	///*try
	//{
	//	std::vector<int> a = testb;
	//}
	//catch (ztl::Exception& e)
	//{
	//	std::cout<<e.What();
	//}
	//*/

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