#pragma once
#include "../functional/ztl_operator_functor.h"
#include "../algorithm/ztl_delete_copy_replace.h"
#include "../container/ztl_vector.h"
//�����set��ָ��������
namespace ztl
{
	//�ϲ���������������
	template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename BinaryOperator = ztl::less<void>>inline
	OutputIterator merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, BinaryOperator&& binary_op = ztl::less<void>())
	{
		for(; first1 != last1; ++result)
		{
			if (first2 == last2)
			{
				return ztl::copy(first1, last1, result);
			}
			else if(binary_op(*first1,*first2))
			{
				*result = *first1;
				++first1;
			}
			else
			{
				*result = *first2;
				++first2;
			}
		}
		return ztl::copy(first2, last2, result);
	}
	//TODO ����Ӧ��Ҳ������1��Ԫ�ص��ڴ�ռ����������㶨
	template<typename InputIterator, typename BinaryOperator = ztl::less<void>>inline
	void inplace_merge(InputIterator first, InputIterator middle, InputIterator last,
		BinaryOperator&& binary_op = ztl::less<void>())
	{
		if (first == middle || middle == last||first == last)
		{
			return;
		}
		ztl::vector<iterator_traits<InputIterator>::value_type> buff(first, middle);
		ztl::merge(buff.begin(), buff.end(), middle, last, first, ztl::forward<BinaryOperator>(binary_op));
	}

	//������1�м�ȥ2��Ԫ��,�ظ�Ԫ��ͬ��
	template< typename InputIterator1, typename InputIterator2,
		typename OutputIterator, typename BinaryOperator = ztl::less<void>>inline
	OutputIterator set_difference(
	InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2,
	OutputIterator result, BinaryOperator&& comp = ztl::less<void>())
	{
		while(first1 != last1)
		{
			if (first2 == last2)
			{
				return ztl::copy(first1, last1, result);
			}
			if (comp(*first1 , *first2))//a1 < a2
			{
				*result = *first1;
				++result;
				++first1;
			}
			else if(comp(*first2,*first1))//a2 <a1
			{
				++first2;
			}
			else// a1==a2
			{
				++first1;
				++first2;
			}
		}
		return result;
	}
//set�Ľ������㣬�ظ�Ԫ���� ����min��n1,n2����
		template<typename InputIterator1, typename InputIterator2,typename OutputIterator,
			typename BinaryCompareFunctor = ztl::less<void >> inline
		OutputIterator set_intersection(
		InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result, BinaryCompareFunctor&& comp=ztl::less<void>())
	{
		while (first1 != last1&&first2!=last2)
		{
			if (comp(*first1,*first2))
			{
				++first1;
			}
			else if (comp(*first2,*first1))
			{
				++first2;
			}
			else
			{
				*result = *first1;
				++first1;
				++first2;
				++result;
			}
		}
		return result;
	}
		//�ϲ�������������
		template<typename InputIterator1, typename InputIterator2,
			typename OutputIterator, typename BinaryOperator = ztl::less<void >> inline
		typename ztl::decay<OutputIterator>::non_ref_type 
			set_union(InputIterator1&& first1, InputIterator1&& last1,
			InputIterator2&& first2, InputIterator2&& last2, 
			OutputIterator&& result, BinaryOperator&& binary_op = ztl::less<void>())
		{
			return merge(ztl::forward<InputIterator1>(first1),ztl::forward<InputIterator1>(last1),
						 ztl::forward<InputIterator2>(first2),ztl::forward<InputIterator2>(last2),
						 ztl::forward<OutputIterator>(result), ztl::forward<BinaryOperator>(binary_op));
		}

		//���ϵĶԳƲ
		template< typename InputIterator1, typename InputIterator2,
			typename OutputIterator, typename BinaryCompareFunctor = ztl::less<void >> inline
		OutputIterator set_symmetric_difference(
		InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result, BinaryCompareFunctor&& comp=ztl::less<void>())
		{
			while(first1 != last1)
			{
				if(first2 == last2)
				{
					return ztl::copy(first1, last1, result);
				}
				if(comp(*first1 , *first2))//a1 < a2
				{
					*result = *first1;
					++result;
					++first1;
				}
				else if(comp(*first2, *first1))//a2 <a1
				{
					*result = *first2;
					++result;
					++first2;
				}
				else// a1==a2
				{
					++first1;
					++first2;
				}
			}
			return ztl::copy(first2, last2, result);
		}

}
