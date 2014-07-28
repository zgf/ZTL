#pragma once
#include "../type/ztl_type_traits.h";
namespace ztl
{
	/*
	all_of (InputIterator first, InputIterator last, UnaryPredicate pred)：C11算法。如果序列所有元素均满足谓词pred，则返回true
	any_of (InputIterator first, InputIterator last, UnaryPredicate pred)：C11算法。如果序列存在元素满足谓词pred，则返回true
	none_of (InputIterator first, InputIterator last, UnaryPredicate pred)：C11版。如果序列中所有元素不满足为此pred，则返回true
	*/
	template<typename InputIterator, typename UnaryPredicate>inline
		bool test_all_none(InputIterator first, InputIterator last, UnaryPredicate pred,bool chose)
	{
			while(first != last)
			{
				if(pred(*first) != chose)
				{
					return false;
				}
				++first;
			}
			return true;
	}
	template<typename InputIterator, typename UnaryPredicate>inline
		bool all_of(InputIterator&& first, InputIterator&& last, UnaryPredicate&& pred)
	{
			ztl_assert_true(first == last, "Container is empty!");
			return test_all_none(forward<InputIterator>(first), forward<InputIterator>(last), forward<UnaryPredicate>(pred), true);
	}
	template<typename InputIterator, typename UnaryPredicate>inline
		bool none_of(InputIterator first, InputIterator last, UnaryPredicate pred)
	{
			ztl_assert_true(first == last, "Container is empty!");
			return test_all_none(forward<InputIterator>(first), forward<InputIterator>(last), forward<UnaryPredicate>(pred), false);
	}
	template<typename InputIterator, typename UnaryPredicate>inline
		bool any_of(InputIterator first, InputIterator last, UnaryPredicate pred)
	{
			bool chose = true;
			while(first != last)
			{
				if(pred(*first) == chose)
				{
					return true;
				}
				++first;
			}
			return false;
	}
}