#include <iostream>
#include <vector>
////Լ�� ������ pascalʽ
//// ���Ա �շ�ʽ+ǰ׺��׺
//// ������ �շ�ʽ+ǰ׺��׺
//// ���� �� ȫ��д���»�������,null����
//// ������ pascalʽ
//// ö��ֵpascalʽ����pascal+�»���
////template<typename T>
#include <algorithm>
using std::cout;
using std::endl;
#include "time/ztl_timer.h"
//#include "test/test_string.h"
#include "algorithm/set_algorithm.h"
#include "algorithm/ztl_sort.h"


	


int main()
{
	ztl::vector<int> v = {1, 0, 8, 0, 3, 5, 6,9, 9, 2};
	/* for(int i = 0; i < 9; ++i)
	 {
		 v.push_back(i);
	 }*/
	/*auto middle= partition(v.begin(), v.end(), [](int element)
	 {
		 return (element < 5 || element == 5);
	 });*/
	 ztl::quick_sort(v.begin(), v.end());
	
	ztl::for_each(v.begin(), v.end(), [](int i)
	{
		cout << i;
	});

	return 0;
}
////namespace ztl
////{
////	namespace wrapper
////	{
////		namespace tuples
////		{
////			tuple���
////			pair triple���
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
////			�ڴ������
////		}
////		namespace management
////		{
////			����ָ��
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
////			���ȶ���
////			stack
////		}
////	}
////	namespace  algorithm
////	{
////		LINQ
////		����
////		�仯
////	}
////	namespace functional
////	{
////		bind ���
////		function ���
////	}
////
////}