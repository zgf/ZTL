#pragma once
#include "ztl_functional.h"
#include <iostream>
using std::cout;
using std::endl;
void test__function()
{
	cout << "null arg null return"<<endl;
}
double test_function(int a,double b,const char* str)
{
	cout << "int,double,const char* and double renturn";
	return a+b+str[0];
}

