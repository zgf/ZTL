#pragma once
#include "../type/ztl_type_traits.h"
#include "../functional/ztl_operator_functor.h"
#include "../ztl_iterator.h"
#include "ztl_relative.h"

namespace ztl
{
	// 下沉操作,当前节点不满足堆的性质,其后节点都满足
	template<typename RandomAccessIterator, typename CompareFunctor = ztl::less<void>>inline
		void heapify_sink(RandomAccessIterator first,RandomAccessIterator middle, RandomAccessIterator last, CompareFunctor&& comp = ztl::less<void>())
	{
			auto start = first;
			auto target = distance(first,middle);
			auto end = distance(first, last);
			while(target < end)
			{
				auto offset = target;
				auto right = (target + 1) * 2;
				auto left = (target + 1) * 2 - 1;
				if(left < end && comp(*(first + left), *(first + target)))
				{
					offset = left;
				}
				if(right < end && comp(*(first + right), *(first + offset)))
				{
					offset = right;
				}
				if(target != offset)
				{
					ztl::swap(*(first + offset), *(first+target));
					target = offset;
				}
				else
				{
					break;
				}
			}
	}

	//上浮操作.当前节点不满足堆的性质,之前节点都满足
	template< typename RandomAccessIterator, typename CompareFunctor = ztl::less<void>>inline
		void heapify_float(RandomAccessIterator current, RandomAccessIterator first,
		CompareFunctor&& comp = ztl::less<void>())
	{
			auto offset = distance(first, current);
			while(offset > 0 && !comp(*(first + (offset -1)/ 2), *(first + offset)))
			{
				ztl::swap(*(first + (offset-1) / 2), *(first + offset));
				offset = offset / 2;
			}
	}

	template< typename RandomAccessIterator, typename CompareFunctor = ztl::less<void>>inline
		void make_heap(RandomAccessIterator first, RandomAccessIterator last,
		CompareFunctor&& comp = ztl::less<void>())
	{
			auto middle = ztl::next(first, distance(first, last) / 2);
			while(!(middle < first))
			{
				heapify_sink(first,middle, last, ztl::forward<CompareFunctor>(comp));
				cout << endl;
				--middle;
			}
	}

	//这个pushheap 不是真的push了,而是自己先后向push好,然后调用这个函数维护堆的性质
	template< typename RandomAccessIterator, typename CompareFunctor = ztl::less<void>>inline
		void push_heap(RandomAccessIterator first, RandomAccessIterator last,
		CompareFunctor&& comp = ztl::less<void>())
	{
		heapify_float(--last, first, comp);
	}
	//这个pop不是真的pop了.而是把top元素移到了尾部,要你自己去pop_back
	template< typename RandomAccessIterator, typename CompareFunctor = ztl::less<void>>inline
		void pop_heap(RandomAccessIterator first, RandomAccessIterator last,
		CompareFunctor&& comp = ztl::less<void>())
	{
			ztl::swap(*first, *(--last));
			heapify_sink(first,first, last, forward<CompareFunctor>(comp));
	}
	template< typename RandomAccessIterator, typename CompareFunctor = ztl::less<void>>inline
		void heap_sort(RandomAccessIterator first, RandomAccessIterator last,
		CompareFunctor&& comp = ztl::less<void>())
	{
			make_heap(first, last, binary_negate<CompareFunctor>(comp));
			while(last !=first)
			{
				pop_heap(first, last, binary_negate<CompareFunctor>(comp));
				--last;
			}
			
	}

}