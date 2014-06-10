#pragma once
#include "../type/ztl_type_traits.h"
namespace ztl
{
	static const char digit_pairs[201] = {
		"00010203040506070809"
		"10111213141516171819"
		"20212223242526272829"
		"30313233343536373839"
		"40414243444546474849"
		"50515253545556575859"
		"60616263646566676869"
		"70717273747576777879"
		"80818283848586878889"
		"90919293949596979899"
	};

	char* enum_to_string_impl(char* target)
	{
		return target;
	}
	template<typename IntegralType>
	void integal_to_string(IntegralType n, char* c)
	{
		if(n == 0)
		{
			*c = '0';
			*(c + 1) = 0;
			return;
		}

		int sign = -(n < 0);
		typename ztl::if_else<sizeof(IntegralType) == 2, unsigned short, typename ztl::if_else<sizeof(IntegralType) == 4, unsigned int, unsigned __int64>::type>::type val = (n^sign) - sign;
		int size = (val >= 10000000000) ?
			(val >= 1000000000000000) ? 16 :
			(val >= 100000000000000) ? 15 :
			(val >= 10000000000000) ? 14 :
			(val >= 1000000000000) ? 13 :
			(val >= 100000000000) ? 12 : 11
			:
			(val >= 100000) ?
			(val >= 1000000000) ? 10 :
			(val >= 100000000) ? 9 :
			(val >= 10000000) ? 8 :
			(val >= 1000000) ? 7 : 6
			:
			(val >= 10000) ? 5 :
			(val >= 1000) ? 4 :
			(val >= 100) ? 3 :
			(val >= 10) ? 2 : 1;

		c += size - 1;
		while(val >= 100)
		{
			int pos = val % 100;
			val /= 100;
			*(short*)(c - 1) = *(short*)(digit_pairs + 2 * pos);
			c -= 2;
		}
		while(val > 0)
		{
			*c-- = '0' + (val % 10);
			val /= 10;
		}
		c[size + 1] = '\0';
	}
	void bool_to_string(bool n, char* c)
	{
		if(n == false)
		{
			*c = '0';
			*(c + 1) = 0;
		}
		else
		{
			*c = '1';
			*(c + 1) = 0;
		}
		return;
	}

	/*int_num整数位数*/
	/*float_num 小数位数*/
	void double_to_string(double n, int int_num, int float_num, char* c)
	{
		if(float_num == 0)
		{
			integal_to_string(long(n), c);
			return;
		}
		else
		{
			auto  expand = (float_num >= 11) ?
				(float_num >= 15) ? 1000000000000000 :
				(float_num >= 14) ? 100000000000000 :
				(float_num >= 13) ? 10000000000000 :
				(float_num >= 12) ? 1000000000000 : 100000000000
				:
				(float_num >= 6) ?
				(float_num >= 10) ? 10000000000 :
				(float_num >= 9) ? 1000000000 :
				(float_num >= 8) ? 100000000 :
				(float_num >= 7) ? 10000000 : 1000000
				:
				(float_num >= 5) ? 100000 :
				(float_num >= 4) ? 10000 :
				(float_num >= 3) ? 1000 :
				(float_num >= 2) ? 100 :
				(float_num >= 1) ? 10 : 1
				;
			auto target = long long(n*expand);
			integal_to_string(target, c);
			for(auto i = float_num + int_num; i >= int_num; i--)
			{
				c[i + 1] = c[i];
			}
			c[int_num] = '.';
			return;
		}
	}
	void float_to_string(float n, int int_num, int float_num, char* c)
	{
		if(float_num == 0)
		{
			integal_to_string(long(n), c);
			return;
		}
		else
		{
			auto  expand =
				(float_num >= 6) ?
				(float_num >= 9) ? 1000000000 :
				(float_num >= 8) ? 100000000 :
				(float_num >= 7) ? 10000000 : 1000000
				:
				(float_num >= 5) ? 100000 :
				(float_num >= 4) ? 10000 :
				(float_num >= 3) ? 1000 :
				(float_num >= 2) ? 100 :
				(float_num >= 1) ? 10 : 1
				;
			auto target = int(n*expand);
			integal_to_string(target, c);
			for(auto i = float_num + int_num; i >= int_num; i--)
			{
				c[i + 1] = c[i];
			}
			c[int_num] = '.';
			return;
		}
	}
#define enum_to_string(enum_name)\
	enum_to_string_impl(#enum_name);
}
