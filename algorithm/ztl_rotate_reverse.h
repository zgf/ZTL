#pragma once
namespace ztl
{
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
			if(first == middle || middle == last)
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
			return rotate_dispath(first, middle, last, iterator_traits<BidirectionalIterator>::iterator_category());
	}
}