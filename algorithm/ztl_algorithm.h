#pragma once
#include "../type/ztl_type_traits.h"
#include "../iterator/iterator_traits.h"
#include "ztl_math.h"
namespace ztl
{
//#undef max
//#undef min
//	template<typename Type>inline
//	Type max(Type&& left, Type&& right)
//	{
//		return left > right ? left : right;
//	}
//	template<typename Type>inline
//	Type min(Type&& left, Type&& right)
//	{
//		return left < right ? left : right;
//	}
	template<typename Type>inline
		Type abs(Type&& left, Type&& right)
	{
			return left < right ? right - left : left - right;
	}
	template<class InputIterator1, class InputIterator2>inline
		bool equal(
		InputIterator1 left_first,
		InputIterator1 left_end,
		InputIterator2 right_first
		)
	{
			for(; left_first != left_end; left_first++, right_first++)
			{
				if(!(*left_first == *right_first))
				{
					return false;
				}
			}
			return true;
	}
	template<class InputIterator1, class InputIterator2, class BinaryPredicate>inline
		bool equal(
		InputIterator1 left_first,
		InputIterator1 left_end,
		InputIterator2 right_first,
		BinaryPredicate pred
		)
	{
			for(; left_first != left_end; left_first++, right_first++)
			{
				if(!pred(*left_first, *right_first))
				{
					return false;
				}
			}
			return true;
	}
	template<class InputIterator, class UnaryPredicate>inline
		bool all_of(InputIterator first, InputIterator last, UnaryPredicate pred)
	{
			while(first != last)
			{
				if(!pred(*first)) return false;
				++first;
			}
			return true;
	}
	
	
}