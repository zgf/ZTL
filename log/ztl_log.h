#pragma once
#include <iostream>
namespace ztl
{
	/*log_helper*/
	inline void  ztl_assert(bool express, const char* message)
	{
		if (!express)
		{
			std::cerr << message << std::endl;
			abort();
		}
		
	}
}