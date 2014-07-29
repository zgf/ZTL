#include "../type/ztl_type_traits.h"
#include "../functional/ztl_operator_functor.h"
namespace ztl
{
	template<typename InputIterator, typename UnaryPredicate>inline
		InputIterator find_if(InputIterator first, InputIterator last, UnaryPredicate&& pred)
	{
			while(first != last)
			{
				if(pred(first))
				{
					return first;
				}
				++first;
			}
			return ztl::move(first);
	}
	template<typename InputIterator, typename T>inline
		InputIterator find(InputIterator first, InputIterator last, const T& val)
	{
			while(first != last)
			{
				if(*first == val)
				{
					return first;
				}
				++first;
			}
			return ztl::move(first);
	}

	template<typename InputIterator, typename T>inline
		InputIterator find_end(InputIterator first, InputIterator last, const T& val)
	{
			auto end_pos = last;
			--last;
			--first;
			while(first != last)
			{
				if(*last == val)
				{
					return ztl::move(last);
				}
				--last;
			}
			return ztl::move(end_pos);
	}

	//在序列1里面找序列2第一次出现位置
	template<typename InputIterator, typename BinaryPredicate = ztl::equal_to<void>>inline
		InputIterator find_begin(InputIterator first1, InputIterator last1, InputIterator first2, InputIterator last2, BinaryPredicate pred = ztl::equal_to<void>())
	{
			for(; first1 != last1; ++first1)
			{
				auto first1 = find(first1, last1, *first2);
				if(first1 == last1)
				{
					break;
				}
				else
				{
					auto& i = ++first2;
					auto j = first1;
					++j;
					for(; i != last2; ++i)
					{
						if(!pred(*i, *j))
						{
							break;
						}
					}
					if(i == last2)
					{
						return ztl::move(first1);
					}
				}
			}
			return ztl::move(last1);
	}
	//在序列1里面找序列2最后一次出现位置
	template<typename InputIterator, typename BinaryPredicate = ztl::equal_to<void>>inline
		InputIterator find_end(InputIterator first1, InputIterator last1, InputIterator first2, InputIterator last2, BinaryPredicate&& pred = ztl::equal_to<void>())
	{
			return find_begin(reverse_iterator<InputIterator>(first1), reverse_iterator<InputIterator>(last1), reverse_iterator<InputIterator>(first2), reverse_iterator<InputIterator>(last2), forward<BinaryPredicate>(pred));
	}

	template<typename ForwardIterator, typename BinaryPredicate == ztl::equal_to<void>>inline
		ForwardIterator binary_search(ForwardIterator first, ForwardIterator last, const T& val)
	{
			auto middle = ztl::next(first, distance(first, last));
			if(*middle == val)
			{
				return ztl::move(middle);
			}
			else if(*middle < val)
			{
				return binary_search(middle, last, val);
			}
			else
			{
				return binary_search(first, middle, val);
			}
	}
	//在序列1中找到集合2任意元素第一次出现的位置
	template<typename InputIterator, typename BinaryPredicate = ztl::equal_to<void>>inline
		InputIterator find_first_of(InputIterator first1, InputIterator last1, InputIterator first2, InputIterator last2, BinaryPredicate&& pred = ztl::equal_to<void>())
	{
			auto& functor = [&pred, &first1](const iterator_traits<InputIterator>::value_type& target)->bool
			{
				return pred(target, *first1);
			};
			while(first1 != last1)
			{
				auto& find_iter = find_if(first2, last2, functor);
				if(find_iter != last2)
				{
					return find_iter;
				}
				++first1;
			}
			return last1;
	}
	//在序列1中找到集合2任意元素最后第一次出现的位置
	template<typename InputIterator, typename BinaryPredicate = ztl::equal_to<void>>inline
		InputIterator find_last_of(InputIterator first1, InputIterator last1, InputIterator first2, InputIterator last2, BinaryPredicate&& pred = ztl::equal_to<void>())
	{
			return find_first_of(reverse_iterator<InputIterator>(first1), reverse_iterator<InputIterator>(last1), reverse_iterator<InputIterator>(first2), reverse_iterator<InputIterator>(last2), forward<BinaryPredicate>(pred));
	}

	//!find_if
	template<typename InputIterator, typename UnaryPredicate>inline
		InputIterator find_if_not(InputIterator&& first, InputIterator&& last, UnaryPredicate&& pred)
	{
			return find_if(forward<InputIterator>(first),
				forward<InputIterator>(last), ztl::unary_negate<
				typename remove_reference<UnaryPredicate>::type>
				(forward<UnaryPredicate>(pred)));
	}
	//在序列[first1,last1)中，找到序列[first2,last2)首次出现的位置
	template<typename InputIterator, typename BinaryPredicate = ztl::equal_to<void>>inline
		InputIterator search(InputIterator&& first1, InputIterator&& last1, InputIterator&& first2, InputIterator&& last2, BinaryPredicate&& pred = ztl::equal_to<void>())
	{
			return find_begin(forward<InputIterator>(first1),
				forward<InputIterator>(last1),
				forward<InputIterator>(first2),
				forward<InputIterator>(last2),
				forward<BinaryPredicate>(pred));
	}
	//给定序列中，搜索给定值val连续出现n次的位置
	template<typename ForwardIterator, typename Size, typename T, typename BinaryPredicate>inline
		ForwardIterator search_n(ForwardIterator first, ForwardIterator last, Size count, const T& val, BinaryPredicate pred)
	{
			auto first = find(first, last, val);
			if(first == last)
			{
				break;
			}
			else
			{
				auto i = first;
				++i;
				auto j = 1;
				for(; j < count; ++j)
				{
					if(!pred(*i, val))
					{
						break;
					}
				}
				if(j == count)
				{
					return ztl::move(first);
				}
			}

			return ztl::move(last);
	}
}