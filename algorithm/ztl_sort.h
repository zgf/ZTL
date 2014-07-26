#include "../type/ztl_type_traits.h"
#include "set_algorithm.h"
#include "ztl_relative.h"
#include "ztl_heap.h"
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
		BidirectionalIterator partition(BidirectionalIterator first, BidirectionalIterator last, UnaryPredicate Pred)
	{
			ztl::ztl_assert_true(first == last, "error! Container is empty!");
			while(first < last)
			{
				while(first != last &&Pred(*first))
				{
					++first;
				}
				do
				{
					--last;
				}
				while(first < last &&!Pred(*last));

				if(!(first < last))
				{
					break;
				}
				ztl::swap(*first++, *last--);
			}
			return first;
	}


	template<typename BidirectionalIterator,
		typename BinaryPredicate = ztl::less_equal<void>>inline
	BidirectionalIterator chose_pivot(BidirectionalIterator first, BidirectionalIterator last, BinaryPredicate&& Pred = ztl::less_equal<void>())
	{
		ztl::ztl_assert_true(first == last, "error! Container is empty!");
		auto pivot_iter = middle_element(first, ztl::next(first, distance(first, last) / 2), ztl::prev(last));
		ztl::swap(*first, *pivot_iter);

		////选第一个元素为枢元素
		pivot_iter = first;
		const auto pivot = *pivot_iter;

		//这里需要的应该是使谓词为true的最后一个元素为枢元素,而不是true区间的超尾
		auto middle = ztl::prev(ztl::partition(ztl::next(first), last, [&pivot, &Pred](iterator_traits<BidirectionalIterator>::value_type& element)
		{
			return Pred(element, pivot);
		}));
		//交换后middle就是枢元素位置了
		ztl::swap(*middle, *pivot_iter);

		return ztl::move(middle);
	}
	template<typename BidirectionalIterator, typename BinaryPredicate = ztl::less_equal<void>>inline
		void quick_sort(BidirectionalIterator first, BidirectionalIterator last, BinaryPredicate&& Pred = ztl::less_equal<void>())
	{
			if(ztl::distance(first, last) > 1)
			{
				auto middle = chose_pivot(first, last, ztl::forward<BinaryPredicate>(Pred));
				quick_sort(first, middle);
				quick_sort(ztl::next(middle), last);
			}
	}
	//插入排序
	template<typename BidirectionalIterator, typename BinaryPredicate = ztl::less_equal<void>>inline
		void insert_sort(BidirectionalIterator first, BidirectionalIterator last, BinaryPredicate&& Pred = ztl::less_equal<void>())
	{
			for(auto i = ztl::next(first); i < last; i++)
			{
				auto j = i;
				while(j > first && !Pred(*(ztl::prev(j)),*j))
				{
					ztl::swap(*(ztl::prev(j)), *j);
					--j;
				}
			}
	}
	
	//会自己切换快排,堆排,插入排序
	template<typename BidirectionalIterator, typename BinaryPredicate = ztl::less_equal<void>>inline
		void intro_sort(BidirectionalIterator first, BidirectionalIterator last, size_t limit_depth, BinaryPredicate&& Pred = ztl::less_equal<void>())
	{
			if(ztl::distance(first, last) < 16)
			{
				insert_sort(first, last, forward<BinaryPredicate>(Pred));
			}
			else if(limit_depth == 0)
			{
				heap_sort(first, last, ztl::forward<BinaryPredicate>(Pred));
			}
			else
			{
				auto middle = chose_pivot(first, last, ztl::forward<BinaryPredicate>(Pred));
				intro_sort(first, middle, limit_depth - 1, ztl::forward<BinaryPredicate>(Pred));
				intro_sort(ztl::next(middle), last, limit_depth - 1, ztl::forward<BinaryPredicate>(Pred));
			}
	}
	template<typename BidirectionalIterator, typename BinaryPredicate = ztl::less_equal<void>>inline
		void sort(BidirectionalIterator&& first, BidirectionalIterator&& last, BinaryPredicate&& Pred = ztl::less_equal<void>())
	{
			ztl::ztl_assert_true(first == last, "error! Container is empty!");
			intro_sort(ztl::forward<BidirectionalIterator>(first),
				ztl::forward<BidirectionalIterator>(last),
				ztl::log2(ztl::distance(first, last)) * 2, 
				ztl::forward<BinaryPredicate>(Pred));
	}
}