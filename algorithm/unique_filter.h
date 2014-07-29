#pragma once
#include "../functional/ztl_operator_functor.h"
namespace ztl
{
//	unique(ForwardIterator first, ForwardIterator last, BinaryPredicate pred)：重载版本，用二元谓词pred替换operator == 算符。
	template<typename ForwardIterator,typename BinaryPredicate=ztl::equal_to<void>>
	ForwardIterator unique(ForwardIterator first, ForwardIterator last, BinaryPredicate pred=ztl::equal_to<void>())
	{
		auto first_prev = first++;
		ztl::vector<ForwardIterator> mark_list;
		while(first != last)
		{
			if(pred(*first,*first_prev))
			{
				mark_list.push_back(first);
			}
			++first;
			++first_prev;
		}
		if(!mark_list.empty())
		{
			mark_list.push_back(last);
			auto des = mark_list[0];
			for(auto i = 1; i < mark_list.size(); i++)
			{
				des = ztl::copy(ztl::next(mark_list[i - 1]), mark_list[i], des);
			}
			return des;
		}
		return last;
	}
}