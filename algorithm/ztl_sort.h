#include "../type/ztl_type_traits.h"
#include "set_algorithm.h"
namespace ztl
{
	template<typename ForwardIterator>
	void merge_sort(ForwardIterator first, ForwardIterator last)
	{
		if(last - first > 1)
		{
			ForwardIterator middle = first + (last - first) / 2;
			merge_sort(first, middle);
			merge_sort(middle, last);
			ztl::inplace_merge(first, middle, last);
		}
	}
	//对范围内元素进行切分,谓词返回true的放前面,返回false的放后面,返回BidirectionalIterator迭代器p,[first,p)使谓词为true,[p,last)使谓词false
	template<typename BidirectionalIterator, typename UnaryPredicate>inline
		BidirectionalIterator partition(BidirectionalIterator first, BidirectionalIterator last, UnaryPredicate Pred)
	{
			ztl::ztl_assert_true(first == last,"error! Container is empty!");
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
				
				
				if (!(first < last))
				{
					break;
				}
				ztl::swap(*first++, *last--);
			}
			return first;
	}
	template<typename BidirectionalIterator, typename BinaryPredicate = ztl::less_equal<void>>
	BidirectionalIterator chose_pivot(BidirectionalIterator first, BidirectionalIterator last, BinaryPredicate&& Pred = ztl::less_equal<void>())
	{

	}
	template<typename BidirectionalIterator,typename BinaryPredicate=ztl::less_equal<void>>inline
		void quick_sort(BidirectionalIterator first, BidirectionalIterator last,BinaryPredicate&& Pred=ztl::less_equal<void>())
	{
			if (ztl::distance(first,last) > 1)
			{
				//auto dis = ztl::distance(first, last);
				auto pivot_iter = first;//ztl::next(first, dis / 2);
				auto pivot = *pivot_iter;
				//这里需要的应该是使谓词为true的最后一个元素为枢元素,而不是true区间的超尾
				auto middle = --partition(++first, last, [&pivot,&Pred](iterator_traits<BidirectionalIterator>::value_type& element)
				{
					return Pred(element,pivot);
				});
				--first;
				//交换后middle就是枢元素位置了
				ztl::swap(*middle, *pivot_iter);

				quick_sort(first, middle);
				quick_sort(++middle, last);
			}
			
	}
}