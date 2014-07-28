#pragma once
#include "../type/ztl_type_traits.h"
namespace ztl
{
	//��һ�������ƶ�������һ������,��Ԫ�ص���move
	template<typename InputIterator,typename OutputIterator>inline
	OutputIterator move(InputIterator first, InputIterator last, OutputIterator result)
	{
		while(first!=last)
		{
			result = ztl::move(first);
			++result;
			++first;
		}
		return ztl::move(result);
	}
	template<typename BidirectionalIterator,typename OutputIterator>
	OutputIterator move_backward(BidirectionalIterator first, BidirectionalIterator last, OutputIterator result)
	{
		--last;
		--first;
		while(last!=first)
		{
			result = ztl::move(last);
			++result;
			--last;
		}
		return ztl::move(result);
	}
}