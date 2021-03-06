#include <iostream>
#include <vector>
////约定 类型名 pascal式
//// 类成员 驼峰式+前缀后缀
//// 变量名 驼峰式+前缀后缀
//// 常量 宏 全大写加下划线区分,null例外
//// 函数名 pascal式
//// 枚举值pascal式或者pascal+下划线
////template<typename T>
#include <algorithm>
using std::cout;
using std::endl;
#include "time/ztl_timer.h"
#include "algorithm/ztl_algorithm.h"
#include "test/help_test.h"
void test_heapify()
{
	ztl::vector<int> w{3, 1, 4, 2, 5, 9,8,13};
	nth_element(w.begin(), w.begin() + 3, w.end());
	/*heap_sort(w.begin(), w.end());*/
	for_each(w.begin(), w.end(), [](int i)
	{
		cout << i<<" ";
	});
}
void test_make_heap()
{
	ztl::vector<int> w{3, 1, 4, 2, 5, 9};
	heap_sort(w.begin(), w.end());
	//make_heap(w.begin(), w.end(),ztl::greater_equal<void>());
	/*for_each(w.begin(), w.end(), [](int i)
	{
		cout << i;
	});*/
}
void test_remove()
{
	ztl::vector<int> w = {0,1,2,2,3,4,2,5,2,6};
	ztl::remove(w.begin(), w.end(), 2);
	for_each(w.begin(), w.end(), [](int i)
	{
		cout << i << " ";
	});
}
int main()
{
	test_remove();
	//test_heapify();
	/*ztl::vector<int> v = {1, 0, 8, 0, 3, 5, 6,9, 9, 2};
	ztl::vector<int> w{3, 1, 4, 1, 5, 9};
	ztl::make_heap(w.begin(), w.end());

	ztl::for_each(w.begin(), w.end(), [](int i)
	{
		cout << i;
	});*/
	/* for(int i = 0; i < 9; ++i)
	 {
		 v.push_back(i);
	 }*/
	/*auto middle= partition(v.begin(), v.end(), [](int element)
	 {
		 return (element < 5 || element == 5);
	 });*/
	// ztl::quick_sort(v.begin(), v.end());
	
	/*ztl::for_each(v.begin(), v.end(), [](int i)
	{
		cout << i;
	});*/
	
	return 0;
}
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