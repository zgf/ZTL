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
#include "ztl_wrapper.h"
#include "ztl_functional.h"
//Լ�� ������ pascalʽ
// ���Ա �շ�ʽ+ǰ׺��׺
// ������ �շ�ʽ+ǰ׺��׺
// ���� �� ȫ��д���»�������,null����
// ������ pascalʽ
// ö��ֵpascalʽ����pascal+�»���
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
//			tuple���
//			pair triple���
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
//			�ڴ������
//		}
//		namespace management
//		{
//			����ָ��
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
//			���ȶ���
//			stack
//		}
//	}
//	namespace  algorithm
//	{
//		LINQ
//		����
//		�仯
//	}
//	namespace functional
//	{
//		bind
//		function
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
	int  print_add(int i) 
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


using namespace ztl::traits::type_traits;



int main()
{
	/*auto i = [](int a, int b)->int
	{
		return a;
	};
	ztl::functional::function<int(int, int)>a=i;
	auto k = a(1, 3);
	Foo c(1);
	ztl::functional::function<bool(int, int)>b = &c;
	b(1, 3);*/
	
					 
	//ztl::functional::function<void(int)>b= &print_num;
	//b(3);
	//ztl::functional::function<int(double)>c=&test;
	//c(4);
	//ztl::functional::function<int()>d=&testb;
	//d();
	//ztl::functional::function<int(Foo*,int)>e=&Foo::print_add;
	//Foo ef(1);
	//e(&ef, 1);
	//ztl::functional::function<decltype(&test)>g=&test;
	//g(4);
	//ztl::functional::function<bool(Foo*,int,int)>h=&Foo::operator();
	//Foo hf(1);
	//h(&hf, 3,1);
	//ztl::functional::function<decltype(&Foo::print_add)>er;
	//er = &Foo::print_add;
	//Foo eg(1);
	//er(&eg, 1);

	//ztl::functional::function<void(int)> f_display = &print_num;
	//f_display(-9);

	//// ���� lambda ���ʽ
	//ztl::functional::function<void()> f_display_42 = []()
	//{
	//	print_num(42);
	//};
	//f_display_42();

	//
	// //�����Ա����
	//ztl::functional::function<void(const Foo*, int)> f_add_display = &Foo::print_add;
	//Foo foo(314159);
	//f_add_display(&foo, 1);
	return 0;
}
