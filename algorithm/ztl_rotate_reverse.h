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
	/*Ч�ʲ���*/
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
	//Ч�ʲ��������
	/*template<typename BidirectionalIterator> inline
	BidirectionalIterator rotate_dispath(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last, ztl::bidirectional_iterator)
	{
	reverse(first, middle);
	reverse(middle, last);
	reverse(first, last);
	return first;
	}*/


	//�ַ�������kλ
	//[��ʼλ��,�м�λ��,����λ��)
	//����1[��ʼλ��,�м�λ��),����2[�м�λ��,����λ��)
	template<typename BidirectionalIterator> inline
		BidirectionalIterator rotate(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last)
	{
			return rotate_dispath(first, middle, last, iterator_traits<BidirectionalIterator>::iterator_category());
	}
}