#include "../type/ztl_type_traits.h"
#include "set_algorithm.h"
#include "ztl_relative.h"
#include "ztl_heap.h"
#include "ztl_rotate_reverse.h"
namespace ztl
{
	template<typename ForwardIterator, typename BinaryPredicate = ztl::less<void>>inline
	void merge_sort(ForwardIterator first, ForwardIterator last, BinaryPredicate&& pred)
	{
		if(last - first > 1)
		{
			ForwardIterator middle = first + (last - first) / 2;
			merge_sort(first, middle);
			merge_sort(middle, last);
			ztl::inplace_merge(first, middle, last, forward<BinaryPredicate>(pred));
		}
	}
	//对范围内元素进行切分,谓词返回true的放前面,返回false的放后面,返回BidirectionalIterator迭代器p,[first,p)使谓词为true,[p,last)使谓词false
	template<typename BidirectionalIterator, typename UnaryPredicate>inline
		BidirectionalIterator partition(BidirectionalIterator first, BidirectionalIterator last, UnaryPredicate pred)
	{
			ztl::ztl_assert_true(first == last, "error! Container is empty!");
			while(first < last)
			{
				while(first != last &&pred(*first))
				{
					++first;
				}
				do
				{
					--last;
				}
				while(first < last &&!pred(*last));

				if(!(first < last))
				{
					break;
				}
				ztl::swap(*first++, *last--);
			}
			return first;
	}


	template<typename BidirectionalIterator, typename BinaryPredicate = ztl::less_equal<void>>inline
		void quick_sort(BidirectionalIterator first, BidirectionalIterator last, BinaryPredicate&& pred = ztl::less_equal<void>())
	{
			if(ztl::distance(first, last) > 1)
			{
				auto middle = chose_pivot(first, last, ztl::forward<BinaryPredicate>(pred));
				quick_sort(first, middle);
				quick_sort(ztl::next(middle), last);
			}
	}
	//插入排序
	template<typename BidirectionalIterator, typename BinaryPredicate = ztl::less_equal<void>>inline
		void insert_sort(BidirectionalIterator first, BidirectionalIterator last, BinaryPredicate&& pred = ztl::less_equal<void>())
	{
			for(auto i = ztl::next(first); i < last; i++)
			{
				auto j = i;
				while(j > first && !pred(*(ztl::prev(j)),*j))
				{
					ztl::swap(*(ztl::prev(j)), *j);
					--j;
				}
			}
	}
	
	//会自己切换快排,堆排,插入排序
	template<typename BidirectionalIterator, typename BinaryPredicate = ztl::less_equal<void>>inline
		void intro_sort(BidirectionalIterator first, BidirectionalIterator last, size_t limit_depth, BinaryPredicate&& pred = ztl::less_equal<void>())
	{
			if(ztl::distance(first, last) < 16)
			{
				insert_sort(first, last, forward<BinaryPredicate>(pred));
			}
			else if(limit_depth == 0)
			{
				heap_sort(first, last, ztl::forward<BinaryPredicate>(pred));
			}
			else
			{
				auto& middle = chose_pivot(first, last, ztl::forward<BinaryPredicate>(pred));
				intro_sort(first, middle, limit_depth - 1, ztl::forward<BinaryPredicate>(pred));
				intro_sort(ztl::next(middle), last, limit_depth - 1, ztl::forward<BinaryPredicate>(pred));
			}
	}
	template<typename BidirectionalIterator, typename BinaryPredicate = ztl::less_equal<void>>inline
		void sort(BidirectionalIterator&& first, BidirectionalIterator&& last, BinaryPredicate&& pred = ztl::less_equal<void>())
	{
			ztl::ztl_assert_true(first == last, "error! Container is empty!");
			intro_sort(ztl::forward<BidirectionalIterator>(first),
				ztl::forward<BidirectionalIterator>(last),
				ztl::log2(ztl::distance(first, last)) * 2, 
				ztl::forward<BinaryPredicate>(pred));
	}
	template<typename BidirectionalIterator, typename BinaryPredicate = ztl::less_equal<void>>inline
		void partial_sort(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last, BinaryPredicate&& pred = ztl::less_equal<void>())
	{
			auto& rbegin = reverse_iterator<BidirectionalIterator>(last);
			auto& rend = reverse_iterator<BidirectionalIterator>(first);
			make_heap(rbegin, rend, pred);
			for(auto i = first; i < middle;++i)
			{
				pop_heap(rbegin,rend,pred);
				rend--;
			}
	}
	template<typename ForwardIterator, typename BinaryPredicate = ztl::less<void>>inline
	bool is_sort(ForwardIterator  first, ForwardIterator  last, BinaryPredicate&& pred = ztl::less<void>())
	{
		ztl_assert_true(first == last, "Container is empty!");
			while(ztl::next(first) != last)
			{
				if (!pred(first,ztl::next(first)))
				{
					return false;
				}
				++first;
			}
			return true;
	}
	template<typename BidirectionalIterator,
		typename BinaryPredicate = ztl::less_equal<void>>inline
	BidirectionalIterator chose_pivot(BidirectionalIterator first, BidirectionalIterator last, BinaryPredicate&& pred = ztl::less_equal<void>())
	{
		ztl::ztl_assert_true(first == last, "error! Container is empty!");
		if (distance(first,last) == 1)
		{
			return ztl::move(first);
		}
		auto& pivot_iter = middle_element(first, ztl::next(first, distance(first, last) / 2), ztl::prev(last));
		ztl::swap(*first, *pivot_iter);

		////选第一个元素为枢元素
		pivot_iter = first;
		const auto pivot = *pivot_iter;

		//这里需要的应该是使谓词为true的最后一个元素为枢元素,而不是true区间的超尾
		auto middle = ztl::prev(ztl::partition(ztl::next(first), last, [&pivot, &pred](iterator_traits<BidirectionalIterator>::value_type& element)
		{
			return pred(element, pivot);
		}));
		//交换后middle就是枢元素位置了
		ztl::swap(*middle, *pivot_iter);

		return ztl::move(middle);
	}
	template<typename ForwardIterator, typename BinaryPredicate = ztl::less<void>>inline
	ForwardIterator is_sort_until(ForwardIterator  first, ForwardIterator  last, BinaryPredicate&& pred = ztl::less<void>())
	{
		ztl_assert_true(first == last, "Container is empty!");
		while(ztl::next(first) != last&&pred(first, ztl::next(first)))
		{
			++first;
		}
		return ztl::move(first);
	}
	template<typename BidirectionalIterator, typename BinaryPredicate = ztl::less_equal<void>>inline
		void stable_sort(BidirectionalIterator&& first, BidirectionalIterator&& last, BinaryPredicate&& pred = ztl::less_equal<void>())
	{
			merge_sort(first, last, forward<BinaryPredicate>(pred));
	}
	template<typename BidirectionalIterator, typename BinaryPredicate = ztl::less_equal<void>>inline
		void nth_element(BidirectionalIterator first,BidirectionalIterator nth, BidirectionalIterator last, BinaryPredicate&& pred = ztl::less_equal<void>())
	{
			if (distance(first,last)>0)
			{
				auto& middle = chose_pivot(first, last, forward<BinaryPredicate>(pred));
				auto nth_index = distance(first, nth);
				auto middle_index = distance(first, middle);
				if(middle_index == nth_index)
				{
				}
				else if(nth_index < middle_index)
				{
					nth_element(first, nth, middle, forward<BinaryPredicate>(pred));
				}
				else
				{
					nth_element(ztl::next(middle), nth, last, forward<BinaryPredicate>(pred));
				}
			}
			
	}
	//判断序列满足pred true的元素是不是在序列前方
	template<typename InputIterator, typename UnaryPredicate>
	bool is_partitioned(InputIterator first, InputIterator last, UnaryPredicate pred)
	{
		ztl::ztl_assert_true(first == last, "Continer is empty!");
		bool chose = true;
		int sum = 0;
		while(first != last)
		{
			if (pred(*first) != chose)
			{
				++sum;
				chose = !chose;
				if(sum == 2)
				{
					return false;
				}
			}
		}
		return true;
	}
	template<typename BidirectionalIterator, typename UnaryPredicate>inline
		BidirectionalIterator stable_partition(BidirectionalIterator first, BidirectionalIterator last, UnaryPredicate&& pred)
	{
		auto middle = partition(first, last, forward<UnaryPredicate>(pred));
		ztl::reverse(middle, last);
		return ztl::move(middle);
	}
}