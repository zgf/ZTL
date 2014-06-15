#pragma once
#include "../type/ztl_type_traits.h"
#include "../iterator/iterator_traits.h"
namespace ztl
{
#undef max
#undef min
	template<typename Type>inline
	Type max(Type&& left, Type&& right)
	{
		return left > right ? left : right;
	}
	template<typename Type>inline
	Type min(Type&& left, Type&& right)
	{
		return left < right ? left : right;
	}
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
	template<typename BidirectionalIterator>inline
	void reverse_dispath(BidirectionalIterator first, BidirectionalIterator last, bidirectional_iterator)
	{
		while(true)
		{
			if(first == last || first == --last)
			{
				return;
			}
			else
			{
				ztl::swap(*first, *last);
				++first;
			}
		}
	}
	template<typename BidirectionalIterator>inline
	void reverse_dispath(BidirectionalIterator first, BidirectionalIterator last, random_acess_iterator)
	{
		while(first < last)
		{
			--last;
			ztl::swap(*first, *last);
			++first;
		}
	}
	template<typename BidirectionalIterator>inline
	void reverse(BidirectionalIterator first, BidirectionalIterator last)
	{
		reverse_dispath(first, last, iterator_traits<BidirectionalIterator>::iterator_category());
	}
	/*效率不行*/
	template<typename ForwordIter> inline
		ForwordIter rotate_dispath(ForwordIter first, ForwordIter middle, ForwordIter last, ztl::forward_iterator)
	{
		if (first == middle||middle==last)
		{
			return first;
		}
		auto old_first = first;
		for(auto i = middle;;)
		{
			ztl::swap(ztl::move(*i), ztl::move(*first));
			++first;
			++i;
			if(first == middle)
			{
				if(i == last)
				{
					return old_first;
				}
				middle = i;
			}
			else if(i == last)
			{
				i = middle;
			}
			
		}
		
		return old_first;
	}
	//效率不如上面的
	/*template<typename BidirectionalIterator> inline
		BidirectionalIterator rotate_dispath(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last, ztl::bidirectional_iterator)
	{
		reverse(first, middle);
		reverse(middle, last);
		reverse(first, last);
		return first;
	}*/
	
	
	//字符串左旋k位
	//[开始位置,中间位置,结束位置)
	//区间1[开始位置,中间位置),区间2[中间位置,结束位置)
	template<typename BidirectionalIterator> inline
		BidirectionalIterator rotate(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last)
	{
		return rotate_dispath(first, middle, last,  iterator_traits<BidirectionalIterator>::iterator_category());
	}
	template<typename IntegralType>
	IntegralType gcd(IntegralType a, IntegralType b)
	{
		if(a == 0) return b;
		if(b == 0) return a;
		if(a % 2 == 0 && b % 2 == 0) return 2 * gcd(a >> 1, b >> 1);
		else if(a % 2 == 0)  return gcd(a >> 1, b);
		else if(b % 2 == 0) return gcd(a, b >> 1);
		else return gcd(abs(a,b), min(a, b));
	}
	size_t log2(size_t x)
	{
		static const unsigned char log_2[256] = {
			0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
			6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
			7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
			7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
			8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
			8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
			8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
			8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8
		};
		std::ptrdiff_t l = -1;
		while(x >= 256)
		{
			l += 8; x >>= 8;
		}
		return l + log_2[x];
	}
	
}