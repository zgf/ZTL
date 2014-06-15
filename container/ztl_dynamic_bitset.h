#pragma once
#include "ztl_vector.h"
#include "boost/dynamic_bitset/dynamic_bitset.hpp"
namespace ztl
{
	class dynamic_bitset
	{
	public:
		typedef ztl::vector<int>		buff_type;
		typedef size_t					size_type;
	public:
		dynamic_bitset()
		{

		}
		dynamic_bitset(const size_type bit_size)
		{

		}
	public:
		buff_type bitbuff;
	};
}