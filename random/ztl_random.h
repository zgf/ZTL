#pragma once
#include "../container/ztl_vector.h"
#include <random>
namespace ztl
{
	//不重复的随机序列生成,End是超尾
	ztl::vector<int> nonrepeat_random_sequence(int start, int end)
	{
		auto count = end - start;
		ztl::vector<int>result(count);
		std::random_device rd;
		std::mt19937 gen(rd());
		ztl::vector<int> seque_array(count);
		for(int i = 0; i < seque_array.size(); i++)
		{
			seque_array[i] = start + i;
		}
		for(int i = end - 1, j = 0; i >= 0; i--, j++)
		{
			std::uniform_int_distribution<> dis(start, i);
			result[j] = seque_array[dis(gen)];
			ztl::swap(seque_array[dis(gen)], seque_array.back());
		}
	}
}