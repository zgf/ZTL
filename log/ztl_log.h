#pragma once
#include <iostream>
namespace ztl
{
	/*log_helper*/
	
	inline void  ztl_assert_true(bool express, const char* message)
	{
		if(express)
		{
			std::cerr << message << std::endl;
			abort();
		}
	}
	template<typename ExceptionType>
	inline void ztl_assert_true(bool express, ExceptionType exceptor)
	{
		if(express)
		{
			throw exceptor;
		}
	}
	inline void  ztl_assert_false(bool express, const char* message)
	{
		ztl_assert_true(!express, message);
	}
	template<typename ExceptionType>
	inline void ztl_assert_false(bool express, ExceptionType exceptor)
	{
		ztl_assert_true(!express, exceptor);
	}
	inline void  ztl_assert(bool express, const char* message)
	{
		return ztl_assert_false(express, message);
	}
	template<typename ExceptionType>
	inline void  ztl_assert(bool express, ExceptionType exceptor)
	{
		return ztl_assert_false(express, exceptor);
	}
	
}