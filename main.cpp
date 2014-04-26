
#include <vector>
#include <iostream>
#include <memory>
#include "Z_Concept.h"
//约定 类型名 pascal式
// 类成员 驼峰式+前缀后缀
// 变量名 驼峰式+前缀后缀
// 常量 宏 全大写加下划线区分,null例外
// 函数名 pascal式
// 枚举值pascal式或者pascal+下划线


int main()
{
	ztl::concept::base_concept::Test<int> TestA;
	ztl::concept::base_concept::Test<int> TestB;

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
	return 0;
}