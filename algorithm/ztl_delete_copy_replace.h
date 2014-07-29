#pragma once
#include "../ztl_iterator.h"
namespace ztl
{
	//copy
	template<typename InputIterator, typename ForwardIterator>inline
		ForwardIterator copy_dispath(InputIterator start, InputIterator end, ForwardIterator des, ztl::false_type)
	{
			while(start != end)
			{
				construct(&*des, *start);
				des++;
			}
			return des;
	}
	/*内存初始化*/
	template<typename InputIterator, typename ForwardIterator>inline
		ForwardIterator copy_dispath(InputIterator start, InputIterator end, ForwardIterator des, ztl::true_type)
	{
			using value_type = typename iterator_traits<InputIterator>::value_type;
			using different_type = typename iterator_traits<InputIterator>::different_type;

			const different_type count = ztl::distance(start, end);
			memcpy_s(&*des, count*sizeof(value_type), &*start, count*sizeof(value_type));
			des += count;
			return des;
	}

	template<typename InputIterator, typename ForwardIterator>inline
		ForwardIterator copy(InputIterator start, InputIterator end, ForwardIterator des)
	{
			return ztl::move(copy_dispath(start, end, des, ztl::integral_constant<bool, ztl::is_pod<typename ztl::iterator_traits<InputIterator>::value_type>::value>()));
	}
	inline wchar_t* copy(wchar_t* start, wchar_t* end, wchar_t* des)
	{
		memmove(des, start, (end - start)*sizeof(wchar_t));
		return ztl::move(des + (end - start));
	}
	inline unsigned char* copy(unsigned char* start, unsigned char* end, unsigned char* des)
	{
		memmove(des, start, (end - start)*sizeof(unsigned char));
		return ztl::move(des + (end - start));
	}
	inline char* copy(char* start, char* end, char* des)
	{
		memmove(des, start, (end - start)*sizeof(char));
		return ztl::move(des + (end - start));
	}
	//result是新区间的超尾
	template<typename BidirectionalIterator1,typename BidirectionalIterator2>inline
	BidirectionalIterator2 copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result)
	{
		return ztl::copy(reverse_iterator<BidirectionalIterator1>(last), reverse_iterator<BidirectionalIterator1>(first), reverse_iterator<BidirectionalIterator2>(ztl::prev(result)));
	}
	template<typename InputIterator,typename OutputIterator,typename UnaryPredicate>inline
	OutputIterator copy_if(InputIterator first, InputIterator last, OutputIterator result, UnaryPredicate&& pred)
	{
		for(; first != last;++first)
		{
			if (pred(first))
			{
				*result = *first;
				++result;
			}
		}
		return ztl::move(result);
	}
	template<typename InputIterator,typename SizeType,typename OutputIterator>inline
	OutputIterator copy_n(InputIterator&& first, SizeType&& n, OutputIterator&& result)
	{
		return ztl::copy(forward<InputIterator>(first), ztl::next(forward<InputIterator>(first), n), forward<OutputIterator>(result));
	}
	//remove (ForwardIterator first, ForwardIterator last, const T& val)：删除序列中等于给定值的所有元素，保留的元素存放在容器的前部且相对次序不变。容器的size不变。
	template<typename ForwardIterator, typename UnaryPredicate>inline
	ForwardIterator remove_if(ForwardIterator first, ForwardIterator last, UnaryPredicate pred)
	{
		ztl::vector<ForwardIterator> mark_list;
		while(first != last)
		{
			if(pred(*first))
			{
				mark_list.push_back(first);
			}
			++first;
		}
		if (!mark_list.empty())
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

	//remove (ForwardIterator first, ForwardIterator last, const T& val)：删除序列中等于给定值的所有元素，保留的元素存放在容器的前部且相对次序不变。容器的size不变。
	template<typename ForwardIterator, typename T>inline
	ForwardIterator remove(ForwardIterator&& first, ForwardIterator&& last, const T& value)
	{
		return remove_if(ztl::forward<ForwardIterator>(first), ztl::forward<ForwardIterator>(last), [&value](const iterator_traits<ForwardIterator>::value_type& target)
		{
			return target == value;
		});
	}
	//replace_if (ForwardIterator first, ForwardIterator last, UnaryPredicate pred, const T& new_value )：把序列中满足给定谓词pred的元素替换为新值
	template<typename ForwardIterator, typename UnaryPredicate, typename T>inline
		void replace_if(ForwardIterator first, ForwardIterator last, const T& new_value, UnaryPredicate pred)
	{
		while(first != last)
		{
			if (pred(*first))
			{
				*first = new_value;
			}
			++first;
		}
	}
	template<typename ForwardIterator, typename T1, typename T2>inline
		void replace(ForwardIterator&& first, ForwardIterator&& last, cont T1& old_value, const T2& new_value)
	{
			return replace_if(forward<ForwardIterator>(first), forward<ForwardIterator>(last), new_value,[&old_value](const ztl::iterator_traits<ForwardIterator>::value_type& target)
			{
				return target == old_value;
			});
	}

}