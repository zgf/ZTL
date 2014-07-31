#pragma once
#include "../type/ztl_type_traits.h"
#include "../iterator/iterator_traits.h"
#include "../functional/ztl_operator_functor.h"
#include "../tuple/ztl_pair.h"
#include "../container/ztl_vector.h"
namespace ztl
{
	template<typename Type>inline
		Type abs(Type&& left, Type&& right)
	{
			return left < right ? right - left : left - right;
	}
	//��ת,����
	/*
	reverse (BidirectionalIterator first, BidirectionalIterator last)���������е�Ԫ������
	reverse_copy (BidirectionalIterator first, BidirectionalIterator last, OutputIterator result)���������е�����
	rotate (ForwardIterator first, ForwardIterator middle,ForwardIterator last)����Ч��ѭ���������У�ʹ�õ�����middle��ָ��Ԫ�س�Ϊ��Ԫ�ء�
	rotate_copy (ForwardIterator first, ForwardIterator middle, ForwardIterator last, OutputIterator result)����Ч��ѭ���������в����Ƶ��µĴ洢�ռ䣬ʹ�õ�����middle��ָ��Ԫ�س�Ϊ��Ԫ�ء�
	*/

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
	template<typename ForwordIterator> inline
		ForwordIterator rotate_dispath(ForwordIterator first, ForwordIterator middle, ForwordIterator last, ztl::forward_iterator)
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
	template<typename BidirectionalIterator> inline
	BidirectionalIterator rotate_dispath(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last, ztl::bidirectional_iterator)
	{
	reverse(first, middle);
	reverse(middle, last);
	reverse(first, last);
	return first;
	}


	//�ַ�������kλ
	//[��ʼλ��,�м�λ��,����λ��)
	//����1[��ʼλ��,�м�λ��),����2[�м�λ��,����λ��)
	template<typename BidirectionalIterator> inline
		BidirectionalIterator rotate(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last)
	{
			return rotate_dispath(first, middle, last, iterator_traits<BidirectionalIterator>::iterator_category());
	}

	//����
	/*
	iter_swap (ForwardIterator1 a, ForwardIterator2 b)������������������ָ��Ԫ�ض���
	swap (T& a, T& b)������������������ʹ���ƶ�����
	swap(T (&a)[N], T (&b)[N])������������������
	swap_ranges (ForwardIterator1 first1, ForwardIterator1 last1,ForwardIterator2 first2)���������������ж�ӦԪ��
	*/
	template<typename ForwardIterator1, typename ForwardIterator2>
	void swap_range(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2)
	{
		ztl::ztl_assert_true(distance(first1, last1) != distance(first2, last2), "error! Length Range 1!= Range 2");
		for(; first1 != last1; ++first1, ++first2)
		{
			ztl::swap(*first1, *first2);
		}
	}

	//����
	/*
	ForwardIterator adjacent_find (ForwardIterator first, ForwardIterator last)���������з��ֵ�һ��������ֵ��ȵ�Ԫ��
	ForwardIterator adjacent_find (ForwardIterator first, ForwardIterator last, BinaryPredicate pred)�����ذ汾���ø���ν��pred������operator ==
	binary_search (ForwardIterator first, ForwardIterator last,const T& val)����һ�����������������������ж��������Ƿ��и���ֵval��
	binary_search (ForwardIterator first, ForwardIterator last,const T& val, Compare comp)�����ذ汾���ø���ν��pred������operator ==
	find (InputIterator first, InputIterator last, const T& val)����һ���������У��ҵ���һ�����ڸ���ֵ��Ԫ��
	ForwardIterator1 find_end (ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2)��������[first1,last1)�У��ҵ�����[first2,last2)�����ֵ�λ��
	ForwardIterator1 find_end (ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2, BinaryPredicate pred)�����ذ汾���ø���ν��pred����operator==
	find_first_of (InputIterator first1, InputIterator last1, ForwardIterator first2, ForwardIterator last2)��������[first1, last1)�У��ҵ�����[first2,last2)���κ�һ��Ԫ�صĵ�һ�γ���
	find_first_of (InputIterator first1, InputIterator last1, ForwardIterator first2, ForwardIterator last2, BinaryPredicate pred)�����ذ汾���ø���ν��pred����operator==
	find_if (InputIterator first, InputIterator last, UnaryPredicate pred)���������з�������ν��pred�ĵ�һ��Ԫ��
	find_if_not (InputIterator first, InputIterator last, UnaryPredicate pred)��C11�㷨���������з��ز�����ν��pred�ĵ�һ��Ԫ��
	search (ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2)��������[first1,last1)�У��ҵ�����[first2,last2)�״γ��ֵ�λ��
	search (ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2, BinaryPredicate pred)�����ذ汾���ø���ν��pred����operator==
	search_n (ForwardIterator first, ForwardIterator last, Size count, const T& val)�����������У���������ֵval��������n�ε�λ��
	search_n ( ForwardIterator first, ForwardIterator last, Size count, const T& val, BinaryPredicate pred )�����ذ汾���ø���ν��pred����operator==
	*/
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
		InputIterator find(InputIterator&& first, InputIterator&& last, const T& value)
	{
			return find_if(ztl::forward<InputIterator>(first), ztl::forward<InputIterator>(last),
				[&value](const iterator_traits<InputIterator>::value_type& target)->bool
			{
				return target == value;
			});
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

	//������1����������2��һ�γ���λ��
	template<typename InputIterator1, typename InputIterator2 ,typename BinaryPredicate = ztl::equal_to<void>>inline
		InputIterator1 find_begin(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, BinaryPredicate pred = ztl::equal_to<void>())
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
	//������1����������2���һ�γ���λ��
	template<typename BidirectionalIterator1, typename BidirectionalIterator2, typename BinaryPredicate = ztl::equal_to<void>>inline
		BidirectionalIterator2 find_end(BidirectionalIterator1 first1, BidirectionalIterator1 last1, BidirectionalIterator2 first2, BidirectionalIterator2 last2, BinaryPredicate&& pred = ztl::equal_to<void>())
	{
			return find_begin(reverse_iterator<BidirectionalIterator1>(last1), reverse_iterator<BidirectionalIterator1>(first1), reverse_iterator<BidirectionalIterator2>(last2), reverse_iterator<BidirectionalIterator2>(first2), forward<BinaryPredicate>(pred));
	}
	//����ҵ���,������������ֵ,���û�ҵ�,���ظ�Ԫ��Ӧ����ǰ�������λ��.
	template<typename ForwardIterator, typename T, typename BinaryPredicate = ztl::less<void>>inline
		ztl::pair<bool, ForwardIterator> __binary_search(ForwardIterator& first, ForwardIterator& last, const T& value, BinaryPredicate&& pred = ztl::less<void>())
	{
			if(distance(first, last) > 0)
			{
				auto& middle = ztl::next(first, distance(first, last) / 2);
				if(pred(*middle, value))
				{
					return __binary_search(middle, last, value, forward<BinaryPredicate>(pred));
				}
				else if(*middle == value)
				{
					return{true, ztl::move(middle)};
				}
				else
				{
					return __binary_search(first, middle, value, forward<BinaryPredicate>(pred));
				}
			}
			return{false, ztl::move(last)};
	}
	template<typename ForwardIterator, typename T, typename BinaryPredicate = ztl::less<void>>inline
		ForwardIterator stl_binary_search(ForwardIterator first, ForwardIterator last, const T& value, BinaryPredicate&& pred = ztl::less<void>())
	{
			auto result = __binary_search(first, last, value, ztl::forward<BinaryPredicate>(pred));
			if(result.first == false)
			{
				return ztl::move(last);
			}
			else
			{
				return ztl::move(result.second);
			}
	}
	//������1���ҵ�����2����Ԫ�ص�һ�γ��ֵ�λ��
	template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate = ztl::equal_to<void>>inline
		InputIterator1 find_first_of(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, BinaryPredicate&& pred = ztl::equal_to<void>())
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
	//������1���ҵ�����2����Ԫ������һ�γ��ֵ�λ��
	template<typename BidirectionalIterator1, typename BidirectionalIterator2 ,typename BinaryPredicate = ztl::equal_to<void>>inline
		BidirectionalIterator1 find_last_of(BidirectionalIterator1 first1, BidirectionalIterator1 last1, BidirectionalIterator2 first2, BidirectionalIterator2 last2, BinaryPredicate&& pred = ztl::equal_to<void>())
	{
			return find_first_of(reverse_iterator<BidirectionalIterator1>(last1), reverse_iterator<BidirectionalIterator1>(first1), reverse_iterator<BidirectionalIterator2>(last2), reverse_iterator<BidirectionalIterator2>(first2), forward<BinaryPredicate>(pred));
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
	//������[first1,last1)�У��ҵ�����[first2,last2)�״γ��ֵ�λ��
	template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate = ztl::equal_to<void>>inline
		InputIterator1 search(InputIterator1&& first1, InputIterator1&& last1, InputIterator2&& first2, InputIterator2&& last2, BinaryPredicate&& pred = ztl::equal_to<void>())
	{
			return find_begin(forward<InputIterator>(first1),
				forward<InputIterator>(last1),
				forward<InputIterator>(first2),
				forward<InputIterator>(last2),
				forward<BinaryPredicate>(pred));
	}
	//���������У���������ֵval��������n�ε�λ��
	template<typename ForwardIterator, typename Size, typename T, typename BinaryPredicate>inline
		ForwardIterator search_n(ForwardIterator first, ForwardIterator last, Size count, const T& val, BinaryPredicate&& pred)
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
	//���������еı߽����
	/*
	equal_range(forIterBegin, forIterEnd, targetVal)����������������в���Ŀ��ֵ��λ�÷�Χ�����ط�Χ���½����Ͻ硣����������������ö��ֲ��ң��������Բ��ҡ�����pair<ForwardIterator,ForwardIterator>
	equal_range(forIterBegin, forIterEnd, targetVal, binPred)�����ذ汾������binPred�Ǹ��������ϵ������
	lower_bound(forIterBegin, forIterEnd, targetVal)��������������в��ҵ�һ����С��targetVal��Ԫ�ء�ʵ�����Ƕ��ֲ��ҡ�
	lower_bound(forIterBegin, forIterEnd, targetVal, binPred)�����ذ汾������binPred�Ǹ��������ϵ������
	upper_bound(forIterBegin, forIterEnd, val)����������������в���Ŀ��ֵval���ֵ��Ͻ磨����һ������Ŀ��ֵval��Ԫ�ص�λ�ã���
	upper_bound(forIterBegin, forIterEnd, val, binPred)�����ذ汾������binPred�Ǹ��������ϵ������
	*/
	template<typename ForwardIterator, typename T, typename BinaryPredicate>inline
		ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, T&& value, BinaryPredicate&& pred)
	{
			auto result = __binary_search(first, last, ztl::forward<T>(value), ztl::forward<BinaryPredicate>(pred));
			if(result.first == false)
			{
				return ztl::move(last);
			}
			else
			{
				while(true)
				{
					result = __binary_search(first, result.second, ztl::forward<T>(value), ztl::forward<BinaryPredicate>(pred));
					if(result.first == false)
					{
						return  ztl::move(result.second);
					}
				}
			}
	}
	template<typename ForwardIterator, typename T, typename BinaryPredicate>inline
		ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T& value, BinaryPredicate&& pred)
	{
			auto result = __binary_search(first, last, ztl::forward<T>(value), ztl::forward<BinaryPredicate>(pred));
			if(result.first == false)
			{
				return ztl::move(last);
			}
			else
			{
				while(result.second != last && !pred(value, result.second))
				{
					++result.second;
				}
				return  ztl::move(result.second);
			}
	}
	template<typename ForwardIterator, typename T, typename BinaryPredicate>inline
		ztl::pair<ForwardIterator, ForwardIterator> equal_range(ForwardIterator&& first, ForwardIterator&& last, T&& value, BinaryPredicate&& pred)
	{
			using no_ref_type = remove_reference<ForwardIterator>::type;
			pair<no_ref_type, no_ref_type> result;
			result.first = lower_bound(forward<ForwardIterator>(first), forward<ForwardIterator>(last), forward<T>(value), forward<BinaryPredicate>(pred));
			result.second = upper_bound(result.first, forward<ForwardIterator>(last), forward<T>(value), forward<BinaryPredicate>(pred));
			return ztl::move(result);
	}
	//�����е�ÿ��Ԫ��ִ�к�������
	/*	for_each(inIterBegin, inIterEnd,ufunc)���ú�������ufunc����������ÿһ��Ԫ��
		transform (InputIterator first1, InputIterator last1, OutputIterator result, UnaryOperation op)����������ÿһ��Ԫ�أ�ִ��һԪ����op�����д����һ������
		transform (InputIterator1 first1, InputIterator1 last1,InputIterator2 first2, OutputIterator result,BinaryOperation binary_op)�������������ж�Ӧ��ÿһ��Ԫ�أ�ִ�ж�Ԫ����binary_op�����д����һ������*/
	template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename BinaryOperator>
	OutputIterator transfrom(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, OutputIterator result, const BinaryOperator&& binary_op)
	{
		for(; first1 != last1; ++first1, ++first2, ++result)
		{
			*result = binary_op(*first1, *first2);
		}
		return result;
	}
	template<typename InputIterator, typename OutputIterator, typename UnaryOperator>
	OutputIterator transfrom(InputIterator first, InputIterator last, OutputIterator result, const UnaryOperator&& unary_op)
	{
		for(; first != last; ++first, ++result)
		{
			*result = unary_op(*first);
		}
		return result;
	}
	template<class InputIterator, class UnaryOperator>inline
		void for_each(InputIterator first, InputIterator last, const UnaryOperator&& unary_op)
	{
			for(; first != last; ++first)
			{
				unary_op(*first);
			}
	}
	//�������е�����
	/*
	all_of (InputIterator first, InputIterator last, UnaryPredicate pred)��C11�㷨�������������Ԫ�ؾ�����ν��pred���򷵻�true
	any_of (InputIterator first, InputIterator last, UnaryPredicate pred)��C11�㷨��������д���Ԫ������ν��pred���򷵻�true
	none_of (InputIterator first, InputIterator last, UnaryPredicate pred)��C11�档�������������Ԫ�ز�����Ϊ��pred���򷵻�true
	*/
	template<typename InputIterator, typename UnaryPredicate>inline
		bool test_all_none(InputIterator first, InputIterator last, UnaryPredicate pred, bool chose)
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
	//�Ƚ�
	/*
	equal(inIter1Begin, inIter1End, inIter2Begin)���Ƚ��������еĶ�ӦԪ���Ƿ����
	equal(inIter1Begin, inIter1End, inIter2Begin, binPred)�����ذ汾������binPred�Ǹ�������ȱȽϺ�����
	lexicographical_compare(inIter1Begin, inIter1End, inIter2Begin, inIter2End)���������������ʵ�Ƚϡ��������еĶ�ӦԪ���� > ������Ƚϡ������һ�������ڴʵ�����С�ڵڶ������У�����true
	lexicographical_compare(inIter1Begin, inIter1End, inIter2Begin, inIter2End, binPred)�����ذ汾������binPred�Ǹ����ġ�С�ڡ��ȽϺ�����
	mismatch (InputIterator1 first1, InputIterator1 last1,InputIterator2 first2)���Ƚ��������еĶ�ӦԪ�أ�������std::pair��ʾ�ĵ�һ����ƥ�����������е�λ�á��Ƚ�ʱʹ��==�������
	mismatch (InputIterator1 first1, InputIterator1 last1,InputIterator2 first2, BinaryPredicate pred)�����ذ汾������binPred�Ǹ����ġ���ȡ��ȽϺ�����
	*/
	//    equal:                    ������������ڱ�־��Χ��Ԫ�ض���ȣ�����true�����ذ汾ʹ������Ĳ���������Ĭ�ϵĵ��ڲ�������
	template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate = ztl::equal_to<void>>inline
		bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate&& pred = ztl::equal_to<void>())
	{
			for(; first1 != last1; ++first1, ++first2)
			{
				if(!pred(*first1, *first2))
				{
					return false;
				}
			}
			return true;
	}
	template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate = ztl::equal_to<void>>inline
		bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, BinaryPredicate&& pred = ztl::equal_to<void>())
	{
			while(first1 != last1 && first2 != last2)
			{
				if(!(*first1,*first2))
				{
					return false;
				}
				++first1;
				++first2;
			}
			return first1 == last1 && first2 == last2;
	}
	template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate = ztl::less<void>>inline
		bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2, BinaryPredicate&& binary_op = ztl::less<void>())
	{
			for(; ( first1 != last1 ) && ( first2 != last2 ); first1++, first2++)
			{
				if(binary_op(*first1, *first2))
				{
					return true;
				}
				if(binary_op(*first2, *first1))
				{
					return false;
				}
			}
			return ( first1 == last1 ) && ( first2 != last2 );
	}
	template< typename InputIterator1, typename InputIterator2, typename BinaryPredicate = ztl::equal_to<void>>inline
		ztl::pair<InputIterator1, InputIterator2>
		mismatch(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2,
		BinaryPredicate&& pred = ztl::equal_to<void>())
	{
			for(; first1 != last1 && pred(*first1, *first2);
				++first1, ++first2)
			{
			}
			return ztl::make_pair(first1, first2);
	}
	template< typename InputIterator1, typename InputIterator2, typename BinaryPredicate = ztl::equal_to<void> >inline
		ztl::pair<InputIterator1, InputIterator2>
		mismatch(InputIterator1&& first1, InputIterator1&& last1,
		InputIterator2&& first2, InputIterator2&& last2,
		BinaryPredicate&& pred = ztl::equal_to<void>())
	{
			return ztl::move(
				mismatch(ztl::forward(first1),
				ztl::forward(last1),
				ztl::forward(first2),
				ztl::forward(pred)));
	}

	//����
	/*
	copy (InputIterator first, InputIterator last, OutputIterator result)������һ�����е���һ������
	copy_backward (BidirectionalIterator1 first,BidirectionalIterator1 last,BidirectionalIterator2 result)����һ�����и��Ƶ���һ�����У�������β��ͷ˳�����θ���Ԫ�ء�
	copy_if (InputIterator first, InputIterator last,OutputIterator result, UnaryPredicate pred)��C11�㷨��������������������ν��pred��Ԫ��
	copy_n (InputIterator first, Size n, OutputIterator result)��C11�㷨������������ǰn��Ԫ��
	*/
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
	/*�ڴ��ʼ��*/
	template<typename InputIterator, typename ForwardIterator>inline
		ForwardIterator copy_dispath(InputIterator start, InputIterator end, ForwardIterator des, ztl::true_type)
	{
			using value_type = typename iterator_traits<InputIterator>::value_type;
			using different_type = typename iterator_traits<InputIterator>::different_type;

			const different_type count = ztl::distance(start, end);
			memcpy_s(&*des, count*sizeof( value_type ), &*start, count*sizeof( value_type ));
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
		memmove(des, start, ( end - start )*sizeof( wchar_t ));
		return ztl::move(des + ( end - start ));
	}
	inline unsigned char* copy(unsigned char* start, unsigned char* end, unsigned char* des)
	{
		memmove(des, start, ( end - start )*sizeof( unsigned char ));
		return ztl::move(des + ( end - start ));
	}
	inline char* copy(char* start, char* end, char* des)
	{
		memmove(des, start, ( end - start )*sizeof( char ));
		return ztl::move(des + ( end - start ));
	}
	//ɾ��[�༭]
	/*
	remove(ForwardIterator first, ForwardIterator last, const T& val)��ɾ�������е��ڸ���ֵ������Ԫ�أ�������Ԫ�ش����������ǰ������Դ��򲻱䡣������size���䡣
		remove_if(ForwardIterator first, ForwardIterator last, UnaryPredicate pred)��ɾ���������������ν��pred��Ԫ�أ�������Ԫ�ش����������ǰ������Դ��򲻱䡣������size���䡣
		remove_copy(InputIterator first, InputIterator last, OutputIterator result, const T& val)����һ�������в����ڸ���ֵ��Ԫ�ظ��Ƶ���һ�������С�
		remove_copy_if(InputIterator first, InputIterator last, OutputIterator result, UnaryPredicate pred)����һ�������в��������ν��pred��Ԫ�ظ��Ƶ���һ�������С�
	//	�滻[�༭]
		replace(ForwardIterator first, ForwardIterator last, const T& old_value, const T& new_value)����������Ϊ����ֵ��Ԫ���滻Ϊ��ֵ
		replace_if(ForwardIterator first, ForwardIterator last, UnaryPredicate pred, const T& new_value)�����������������ν��pred��Ԫ���滻Ϊ��ֵ
		replace_copy(InputIterator first, InputIterator last, OutputIterator result, const T& old_value, const T& new_value)���������У����ڵ�����ֵ��Ԫ�ظ���ʱʹ����ֵ
		replace_copy_if(InputIterator first, InputIterator last, OutputIterator result, UnaryPredicate pred, const T& new_value)���������У������������ν��pred��Ԫ�ظ�����ֵ��
	*/
	//remove (ForwardIterator first, ForwardIterator last, const T& val)��ɾ�������е��ڸ���ֵ������Ԫ�أ�������Ԫ�ش����������ǰ������Դ��򲻱䡣������size���䡣
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
			if(!mark_list.empty())
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

	//remove (ForwardIterator first, ForwardIterator last, const T& val)��ɾ�������е��ڸ���ֵ������Ԫ�أ�������Ԫ�ش����������ǰ������Դ��򲻱䡣������size���䡣
	template<typename ForwardIterator, typename T>inline
		ForwardIterator remove(ForwardIterator&& first, ForwardIterator&& last, const T& value)
	{
			return remove_if(ztl::forward<ForwardIterator>(first), ztl::forward<ForwardIterator>(last), [&value](const iterator_traits<ForwardIterator>::value_type& target)
			{
				return target == value;
			});
	}
	//replace_if (ForwardIterator first, ForwardIterator last, UnaryPredicate pred, const T& new_value )�����������������ν��pred��Ԫ���滻Ϊ��ֵ
	template<typename ForwardIterator, typename UnaryPredicate, typename T>inline
		void replace_if(ForwardIterator first, ForwardIterator last, const T& new_value, UnaryPredicate pred)
	{
			while(first != last)
			{
				if(pred(*first))
				{
					*first = new_value;
				}
				++first;
			}
	}
	template<typename ForwardIterator, typename T1, typename T2>inline
		void replace(ForwardIterator&& first, ForwardIterator&& last, const T1& old_value, const T2& new_value)
	{
			return replace_if(forward<ForwardIterator>(first), forward<ForwardIterator>(last), new_value, [&old_value](const ztl::iterator_traits<ForwardIterator>::value_type& target)
			{
				return target == old_value;
			});
	}

	//result��������ĳ�β
	template<typename BidirectionalIterator1, typename BidirectionalIterator2>inline
		BidirectionalIterator2 copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result)
	{
			return ztl::copy(reverse_iterator<BidirectionalIterator1>(last), reverse_iterator<BidirectionalIterator1>(first), reverse_iterator<BidirectionalIterator2>(ztl::prev(result)));
	}
	template<typename InputIterator, typename OutputIterator, typename UnaryPredicate>inline
		OutputIterator copy_if(InputIterator first, InputIterator last, OutputIterator result, UnaryPredicate&& pred)
	{
			for(; first != last; ++first)
			{
				if(pred(first))
				{
					*result = *first;
					++result;
				}
			}
			return ztl::move(result);
	}
	template<typename InputIterator, typename SizeType, typename OutputIterator>inline
		OutputIterator copy_n(InputIterator&& first, SizeType&& n, OutputIterator&& result)
	{
			return ztl::copy(forward<InputIterator>(first), ztl::next(forward<InputIterator>(first), n), forward<OutputIterator>(result));
	}
	//����

	//��ֵ����
	/*
	unique (ForwardIterator first, ForwardIterator last)����������һȺ��������ȵ�Ԫ�أ���������һ��Ԫ�أ���Ⱥ����Ԫ�ر���ȺԪ��֮�������ֵ��Ԫ���滻���ú������ı���Щֵ���໥˳�򣬲��ı�������size����������ֵΪ���һ������Ԫ�ص���һ��λ�ã�past-the-end element�������麯�����غ�resize��������
	unique (ForwardIterator first, ForwardIterator last,BinaryPredicate pred)�����ذ汾���ö�Ԫν��pred�滻operator==�����
	unique_copy (InputIterator first, InputIterator last,OutputIterator result)����һ�������е�Ԫ�ؿ�������һ�����У�����һȺ��������ȵ�Ԫ�أ���������һ��Ԫ�ء�
	unique_copy (InputIterator first, InputIterator last,OutputIterator result, BinaryPredicate pred)�����ذ汾���ö�Ԫν��pred�滻operator==�����
	*/
	//	unique(ForwardIterator first, ForwardIterator last, BinaryPredicate pred)�����ذ汾���ö�Ԫν��pred�滻operator == �����
	template<typename ForwardIterator, typename BinaryPredicate = ztl::equal_to<void>>
	ForwardIterator unique(ForwardIterator first, ForwardIterator last, BinaryPredicate pred = ztl::equal_to<void>())
	{
		auto first_prev = first++;
		ztl::vector<ForwardIterator> mark_list;
		while(first != last)
		{
			if(pred(*first, *first_prev))
			{
				mark_list.push_back(first);
			}
			++first;
			++first_prev;
		}
		if(!mark_list.empty())
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
	//���
	/*
	fill (ForwardIterator first, ForwardIterator last, const T& val)���ø���ֵ���������ÿ��Ԫ��
	fill_n (OutputIterator first, Size n, const T& val)���ø���ֵ������е�n��Ԫ��
	*/
	template<typename ForwardIterator, typename ValueType>inline
		ForwardIterator fill(ForwardIterator start, ForwardIterator end, const ValueType& val)
	{
			while(start != end)
			{
				*start = val;
				start++;
			}
			return end;
	}
	inline char* fill(char* start, char* end, const char& val)
	{
		memset(start, val, end - start);
		return end;
	}
	inline wchar_t* fill(wchar_t* start, wchar_t* end, const wchar_t& val)
	{
		memset(start, val, end - start);
		return end;
	}
	inline unsigned char* fill(unsigned char* start, unsigned char* end, const unsigned char& val)
	{
		memset(start, val, end - start);
		return end;
	}
	template<typename ForwardIterator, typename DifferentType, typename ValueType>inline
		ForwardIterator fill_n(ForwardIterator start, DifferentType count, const ValueType& val)
	{
			return ztl::move(fill(start, start + count, val));
	};
	template<typename ForwardIterator, typename DifferentType >
	void fill_type(ForwardIterator start, DifferentType count, typename iterator_traits<ForwardIterator>::value_type value)
	{
		auto add_count = 1;
		auto loop_number = ztl::log2(count);
		*start = value;
		for(auto i = 0; i < loop_number; i++)
		{
			copy(start, start + add_count, start + add_count);
			add_count <<= 1;
		}
		auto rest_block = count - add_count;
		copy(start, start + rest_block, start + add_count);
	}
	//����
	/*
	generate (ForwardIterator first, ForwardIterator last, Generator gen)����������ÿ��Ԫ�أ����ε��ú���gen�ķ���ֵ��ֵ��
	generate_n (OutputIterator first, Size n, Generator gen)���������е�n��Ԫ�أ����ε���ָ�������ķ���ֵ��ֵ��
	void init_range(ForwardIterator first, ForwardIterator last, Type init, Type increase = 1)���������ʼֵ��ĳ�ֵ���״̬
	*/
	template<typename OutputIterator, typename Generator>
	void generate(OutputIterator first, OutputIterator last, const Generator&&  gen)
	{
		while(first != last)
		{
			*first++ = gen();
		}
	}
	template<typename OutputIterator, typename Generator>
	void generate_n(OutputIterator first, typename iterator_traits<OutputIterator>::different_type count, const Generator&& gen)
	{
		for(decltype( count ) i = 0; i < count; ++i)
		{
			*first++ = gen();
		}
		return first;
	}
	//���������ʼֵ��ĳ�ֵ���״̬,�ʱ��㷨
	template<typename ForwardIterator, typename Type>inline
		void init_range(ForwardIterator first, ForwardIterator last, Type init, Type increase = 1)
	{
			*first = init;
			while(++first != last)
			{
				init += increase;
				first = init;
			}
	}
	//��ѧ�㷨
	/*
	Functions
	accumulate
	Accumulate values in range (function template )
	adjacent_difference
	Compute adjacent difference of range (function template )
	inner_product
	Compute cumulative inner product of range (function template )
	partial_sum
	Compute partial sums of range (function template )
	iota
	Store increasing sequence (function template )
	*/
	template<typename InputIterator, typename Type, typename BinaryOperator = ztl::plus<void>>inline
		Type accumulate(InputIterator first, InputIterator last, Type init, const BinaryOperator&& binary_op = ztl::plus<void>())
	{
			for(; first != last; ++first)
			{
				init = binary_op(init, *first);
			}
			return init;
	}
	//����һ�����еĵ�һ��ֵ�Ѿ�֮��ÿ������ֵ��һ����Ԫ�����Ľ������׼���ǲ�ֵ��
	template<typename InputIterator, typename OutputIterator, typename BinaryOperator = ztl::minus<void>>inline
		OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, const BinaryOperator&& binary_op = ztl::minus<void>())
	{
			ztl::ztl_assert_true(first == last, "range is error!");
			*result = *first;
			typename iterator_traits<InputIterator>::value_type temp = *result;
			while(++first != last)
			{
				*++result = binary_op(*first, temp);
				temp = *first;
			}
			return ++result;
	}
	//ǰnԪ�غ��г�����
	template<typename InputIterator, typename OutputIterator, typename BinaryOperator = ztl::plus<void>>inline
		OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result, const BinaryOperator&& binary_op = ztl::plus<void>())
	{
			ztl::ztl_assert_true(first == last, "range is error!");
			*result = *first;
			typename iterator_traits<InputIterator>::value_type temp = *result;
			while(++first != last)
			{
				*++result = binary_op(*first, temp);
				temp = *result;
			}
			return ++result;
	}
	//binary_op2��Ӧ������������Ԫ�صģ�binary_op1��Ӧ�������ɵ��м����е�
	template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Type, typename BinaryOperator1 = ztl::plus, typename BinaryOperator2 = ztl::multiplies>inline
		Type inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, Type value, const BinaryOperator1&& binary_op1, const BinaryOperator2&& binary_op2)
	{
			ztl::vector<decltype( binary_op2(first1, first2) )> result(ztl::distance(first1, last1));
			auto& result_end = ztl::transfrom(first1, last1, first2, result.begin(), binary_op2);
			return ztl::accumulate(result, result_end, value, binary_op1);
	}

	template<typename ForwardIterator, typename Type = int>inline
		void iota(ForwardIterator&& first, ForwardIterator&& last, Type&& init, Type&& increase = 1)
	{
			ztl::init_range(ztl::forward(first), ztl::forward(last), ztl::forward(init), ztl::forward(increase));
	}
	template<typename IntegralType>
	IntegralType gcd(IntegralType a, IntegralType b)
	{
		if(a == 0) return b;
		if(b == 0) return a;
		if(a % 2 == 0 && b % 2 == 0) return 2 * gcd(a >> 1, b >> 1);
		else if(a % 2 == 0)  return gcd(a >> 1, b);
		else if(b % 2 == 0) return gcd(a, b >> 1);
		else return gcd(abs(a, b), min(a, b));
	}
	template<typename InteraglType>inline
		InteraglType log2(InteraglType x)
	{
			static const unsigned char log_2[256] = {
				0, 0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
				5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
				6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
				7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
				7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
				8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
				8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
				8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8
			};
			std::ptrdiff_t l = -1;
			while(x >= 256)
			{
				l += 8; x >>= 8;
			}
			return std::move(l + log_2[x]);
	}
	//�Ѳ���
	/*
	is_heap (RandomAccessIterator first, RandomAccessIterator last)��C11�棬�ж������Ƿ�Ϊ�����
	is_heap_until (RandomAccessIterator first, RandomAccessIterator last,Compare comp)��C11�棬���ذ汾
	is_heap_until (RandomAccessIterator first, RandomAccessIterator last)��C11�棬������Ч����ѵ���ĩ��Χ
	is_heap (RandomAccessIterator first, RandomAccessIterator last,Compare comp)��C11�棬���ذ汾
	make_heap (RandomAccessIterator first, RandomAccessIterator last)������һ�����У���Ԫ��������ã�ԭ��(in-place)����һ���������
	make_heap (RandomAccessIterator first, RandomAccessIterator last,Compare comp )�����ذ汾
	pop_heap (RandomAccessIterator first, RandomAccessIterator last)���ѵĸ��ڵ㱻ж�ص�last-1λ�ã��ѵ�Ԫ����Ŀ��1�����ֶ����ʡ�
	pop_heap (RandomAccessIterator first, RandomAccessIterator last,Compare comp)�����ذ汾
	push_heap (RandomAccessIterator first, RandomAccessIterator last)����һ����ΧΪ[first,last-1)�Ķѣ�����һ����Ԫ�ء���Ԫ�����������last-1λ�á�
	push_heap(r,r,bpred)�����ذ汾
	sort_heap (RandomAccessIterator first, RandomAccessIterator last)����һ���ѣ�ִ��ԭ�ض����򣬵õ�һ����������
	sort_heap (RandomAccessIterator first, RandomAccessIterator last,Compare comp)�����ذ汾
	*/
	// �³�����,��ǰ�ڵ㲻����ѵ�����,���ڵ㶼����
	template<typename RandomAccessIterator, typename CompareFunctor = ztl::less<void>>inline
		void heapify_sink(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last, CompareFunctor&& comp = ztl::less<void>())
	{
			auto start = first;
			auto target = distance(first, middle);
			auto end = distance(first, last);
			while(target < end)
			{
				auto offset = target;
				auto right = ( target + 1 ) * 2;
				auto left = ( target + 1 ) * 2 - 1;
				if(left < end && comp(*( first + left ), *( first + target )))
				{
					offset = left;
				}
				if(right < end && comp(*( first + right ), *( first + offset )))
				{
					offset = right;
				}
				if(target != offset)
				{
					ztl::swap(*( first + offset ), *( first + target ));
					target = offset;
				}
				else
				{
					break;
				}
			}
	}

	//�ϸ�����.��ǰ�ڵ㲻����ѵ�����,֮ǰ�ڵ㶼����
	template< typename RandomAccessIterator, typename CompareFunctor = ztl::less<void>>inline
		void heapify_float(RandomAccessIterator current, RandomAccessIterator first,
		CompareFunctor&& comp = ztl::less<void>())
	{
			auto offset = distance(first, current);
			while(offset > 0 && !comp(*( first + ( offset - 1 ) / 2 ), *( first + offset )))
			{
				ztl::swap(*( first + ( offset - 1 ) / 2 ), *( first + offset ));
				offset = offset / 2;
			}
	}

	template< typename RandomAccessIterator, typename CompareFunctor = ztl::less<void>>inline
		void make_heap(RandomAccessIterator first, RandomAccessIterator last,
		CompareFunctor&& comp = ztl::less<void>())
	{
			auto middle = ztl::next(first, distance(first, last) / 2);
			while(!( middle < first ))
			{
				heapify_sink(first, middle, last, ztl::forward<CompareFunctor>(comp));
				cout << endl;
				--middle;
			}
	}

	//���pushheap �������push��,�����Լ��Ⱥ���push��,Ȼ������������ά���ѵ�����
	template< typename RandomAccessIterator, typename CompareFunctor = ztl::less<void>>inline
		void push_heap(RandomAccessIterator first, RandomAccessIterator last,
		CompareFunctor&& comp = ztl::less<void>())
	{
			heapify_float(--last, first, comp);
	}
	//���pop�������pop��.���ǰ�topԪ���Ƶ���β��,Ҫ���Լ�ȥpop_back
	template< typename RandomAccessIterator, typename CompareFunctor = ztl::less<void>>inline
		void pop_heap(RandomAccessIterator first, RandomAccessIterator last,
		CompareFunctor&& comp = ztl::less<void>())
	{
			ztl::swap(*first, *( --last ));
			heapify_sink(first, first, last, forward<CompareFunctor>(comp));
	}
	//���ϲ���
	/*
	includes ( InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2 )���жϵ�2����Ϊ��������е�Ԫ���Ƿ񶼳����ڵ�1������������
	includes ( InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp )�����ذ汾���ø���ν��pred����operator<
	set_difference (InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result)��������������֮��
	set_difference (InputIterator1 first1, InputIterator1 last1,InputIterator2 first2, InputIterator2 last2,OutputIterator result, Compare comp)�����ذ汾
	set_intersection (InputIterator1 first1, InputIterator1 last1,InputIterator2 first2, InputIterator2 last2,OutputIterator result)�������������У����ϣ��Ľ�
	set_intersection (InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare comp)�����ذ汾
	set_symmetric_difference (InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result)�������������еĶԳƲ�
	set_symmetric_difference (InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare comp)�����ذ汾
	set_union (InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result)�������������еĲ�
	set_union (InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare comp)�����ذ汾
	*/
	//�ж�������������2�Ƿ�������1��������,���Բ�����
	template< typename InputIterator1, typename InputIterator2, typename CompareFunctor = ztl::less<void>>inline
		bool includes(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, CompareFunctor&& comp = ztl::less<void>())
	{
			//���������˼��...��<������>=���壬ͨ������<��>=������==������
			//��Ϊ���Ǽ����㷨������ֻҪ��Ԫ��֧��<û˵Ҫ֧��==
			for(; first2 != last2; ++first1)
			{
				//�������comp��trueҲ��˵*first2<*first1 Ҳ���ǲ���������*first2==*first1 ����false
				//��֮˵��*first2>=*first1
				if(first1 == last1 || comp(*first2, *first1))
				{
					return false;
				}
				//����true˵�� *first1 >= *first2
				//�������������������ǰ����� ==�����
				if(!comp(*first1, *first2))
				{
					++first2;
				}
			}
			return true;
	}
	

	//������1�м�ȥ2��Ԫ��,�ظ�Ԫ��ͬ��
	template < typename InputIterator1, typename InputIterator2,
		typename OutputIterator, typename BinaryOperator = ztl::less < void >> inline
		OutputIterator set_difference(
		InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result, BinaryOperator&& comp = ztl::less<void>())
	{
			while(first1 != last1)
			{
				if(first2 == last2)
				{
					return ztl::copy(first1, last1, result);
				}
				if(comp(*first1, *first2))//a1 < a2
				{
					*result = *first1;
					++result;
					++first1;
				}
				else if(comp(*first2, *first1))//a2 <a1
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
	template < typename InputIterator1, typename InputIterator2, typename OutputIterator,
		typename BinaryCompareFunctor = ztl::less < void >> inline
		OutputIterator set_intersection(
		InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result, BinaryCompareFunctor&& comp = ztl::less<void>())
	{
			while(first1 != last1&&first2 != last2)
			{
				if(comp(*first1, *first2))
				{
					++first1;
				}
				else if(comp(*first2, *first1))
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
	template < typename InputIterator1, typename InputIterator2,
		typename OutputIterator, typename BinaryOperator = ztl::less < void >> inline
		typename ztl::decay<OutputIterator>::non_ref_type
		set_union(InputIterator1&& first1, InputIterator1&& last1,
		InputIterator2&& first2, InputIterator2&& last2,
		OutputIterator&& result, BinaryOperator&& binary_op = ztl::less<void>())
	{
			return merge(ztl::forward<InputIterator1>(first1), ztl::forward<InputIterator1>(last1),
				ztl::forward<InputIterator2>(first2), ztl::forward<InputIterator2>(last2),
				ztl::forward<OutputIterator>(result), ztl::forward<BinaryOperator>(binary_op));
	}

	//���ϵĶԳƲ
	template < typename InputIterator1, typename InputIterator2,
		typename OutputIterator, typename BinaryCompareFunctor = ztl::less < void >> inline
		OutputIterator set_symmetric_difference(
		InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result, BinaryCompareFunctor&& comp = ztl::less<void>())
	{
			while(first1 != last1)
			{
				if(first2 == last2)
				{
					return ztl::copy(first1, last1, result);
				}
				if(comp(*first1, *first2))//a1 < a2
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
	//�ϲ�
	/*
	inplace_merge (BidirectionalIterator first, BidirectionalIterator middle,BidirectionalIterator last)�����������������[first,middle) ��[middle,last)��ִ��ԭ�غϲ����ϲ���������Ա������򡣿ռ临�Ӷ�O(1)��ʱ�临�Ӷ�O(N)
	inplace_merge (BidirectionalIterator first, BidirectionalIterator middle,BidirectionalIterator last, Compare comp)���ذ汾��operator< ����������comp���档
	merge (InputIterator1 first1, InputIterator1 last1,InputIterator2 first2, InputIterator2 last2,OutputIterator result)��������������кϲ���������б�������
	merge (InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare comp)�����ذ汾��operator< ����������comp���档
	*/
	//�ϲ���������������
	template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename BinaryOperator = ztl::less<void>>inline
		OutputIterator merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, BinaryOperator&& binary_op = ztl::less<void>())
	{
			for(; first1 != last1; ++result)
			{
				if(first2 == last2)
				{
					return ztl::copy(first1, last1, result);
				}
				else if(binary_op(*first1, *first2))
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
	template<typename InputIterator, typename BinaryOperator = ztl::less<void>>inline
		void inplace_merge(InputIterator first, InputIterator middle, InputIterator last,
		BinaryOperator&& binary_op = ztl::less<void>())
	{
			if(first == middle || middle == last || first == last)
			{
				return;
			}
			ztl::vector<iterator_traits<InputIterator>::value_type> buff(first, middle);
			ztl::merge(buff.begin(), buff.end(), middle, last, first, ztl::forward<BinaryOperator>(binary_op));
	}
	//��С���
	/*
	min (const T& a, const T& b)������ֵ�е���Сֵ��
	min (const T& a, const T& b, Compare comp)�����ذ汾
	max (const T& a, const T& b)������ֵ�е����ֵ��
	max (const T& a, const T& b, Compare comp)�����ذ汾
	min_element (ForwardIterator first, ForwardIterator last)��������������Сֵ�ĵ�����
	min_element (ForwardIterator first, ForwardIterator last,Compare comp) �����ذ汾
	max_element (ForwardIterator first, ForwardIterator last)���������������ֵ�ĵ�����
	max_element (ForwardIterator first, ForwardIterator last,Compare comp) �����ذ汾
	max (initializer_list<T> il, Compare comp)��C11�汾
	minmax (const T& a, const T& b)��C11�棬��������Сֵ�����ֵ���ɵ�std:pair
	minmax (const T& a, const T& b, Compare comp)�����ذ汾
	minmax (initializer_list<T> il)��C11�棬�����ɳ�ʼ���б�����СԪ�������Ԫ�ع��ɵ�std:pair
	minmax (initializer_list<T> il, Compare comp)�����ذ汾
	minmax_element (ForwardIterator first, ForwardIterator last)��C11�棬��������������СԪ�������Ԫ�ع��ɵ�std:pair
	minmax_element (ForwardIterator first, ForwardIterator last, Compare comp)�����ذ汾
	min_if(Type&& left, Type&& right, LeftFunctor&& left_functor, RightFunctor&& right_functor)
	max_if(Type&& left, Type&& right, LeftFunctor&& left_functor, RightFunctor&& right_functor) 
	*/
	template<typename ForwardIterator, typename CompareFunctor = ztl::less<void>>inline
		ForwardIterator max_element(ForwardIterator first, ForwardIterator last, CompareFunctor&& comp = ztl::less<void>())
	{
			if(first == last)
			{
				return last;
			}
			ForwardIterator largest = first;
			++first;
			for(; first != last; ++first)
			{
				if(comp(*largest, *first))
				{
					largest = first;
				}
			}
			return largest;
	}
	template<typename ForwardIterator, typename CompareFunctor = ztl::less<void>>inline
		ForwardIterator min_element(ForwardIterator first, ForwardIterator last, CompareFunctor&& comp = ztl::less<void>())
	{
			return ztl::move(max_element(first, last, ztl::swap_argement(ztl::forward(comp))));
	}
	template<typename ForwardIterator>inline
		ztl::pair<ForwardIterator, ForwardIterator> minmax_element(ForwardIterator first, ForwardIterator last)
	{
			ztl::pair<ForwardIterator, ForwardIterator> result(first, first);
			if(ztl::distance(first, last) > 1)
			{
				while(++first != last)
				{
					if(*first < *result.first)
					{
						*result.first = *first;
					}
					if(*result.second < *first)
					{
						*result.second = *first;
					}
				}
			}
			return ztl::move(result);
	}
	template<typename ForwardIterator>inline
		ztl::pair<typename iterator_traits<ForwardIterator>::value_type, typename iterator_traits<ForwardIterator>::value_type>
		minmax(ForwardIterator first, ForwardIterator last)
	{
			ztl::pair<typename iterator_traits<ForwardIterator>::value_type, typename iterator_traits<ForwardIterator>::value_type> result(*first, *first);
			if(ztl::distance(first, last) > 1)
			{
				while(++first != last)
				{
					if(*first < result.first)
					{
						result.first = *first;
					}
					if(result.second < *first)
					{
						result.second = *first;
					}
				}
			}
			return ztl::move(result);
	}

	template<typename Type>inline
		Type ztl_max(Type&& left, Type&& right)
	{
			return left > right ? left : right;
	}
	template<typename Type>inline
		Type ztl_min(Type&& left, Type&& right)
	{
			return left < right ? left : right;
	}

	
	//��������������ָ���Ԫ�ص���ֵԪ�صĵ�����
	template<typename ForwardIterator, typename BinaryPredicate = ztl::less<void>>inline
		ForwardIterator middle_element(ForwardIterator first, ForwardIterator middle, ForwardIterator last, BinaryPredicate&& pred = ztl::less<void>())
	{
			if(pred(*first, *middle))
			{
				if(pred(*middle, *last))
				{
					return middle;
				}
				else if(pred(*first, *last))
				{
					return last;
				}
				else
				{
					return first;
				}
			}
			else
			{
				if(pred(*last, *middle))
				{
					return middle;
				}
				else if(pred(*first, *last))
				{
					return first;
				}
				else
				{
					return last;
				}
			}
	}
	//����������֮�е���Сֵ
	template<typename InputIterator, typename BinaryPredicate = ztl::less<void>>
	InputIterator min_element(InputIterator first, InputIterator middle, InputIterator last, BinaryPredicate pred = ztl::less<void>())
	{
		if(pred(*first, *middle))
		{
			if(pred(*last, *first))
			{
				return last;
			}
			else
			{
				return first
			}
		}
		else
		{
			if(pred(*last, *middle))
			{
				return last;
			}
			else
			{
				return middle;
			}
		}
	}
	template<typename Type, typename LeftFunctor, typename RightFunctor>inline
		Type min_if(Type&& left, Type&& right, LeftFunctor&& left_functor, RightFunctor&& right_functor)
	{
			if(left < right)
			{
				left_functor();
				return left;
			}
			else
			{
				right_functor();
				return right;
			}

	}
	
	template<typename Type, typename LeftFunctor, typename RightFunctor>inline
		Type max_if(Type&& left, Type&& right, LeftFunctor&& left_functor, RightFunctor&& right_functor)
	{
			return min_if(ztl::forward<Type>(right), ztl::forward<Type>(left), ztl::forward<Type>(left_functor), ztl::forward<Type>(right_functor));
	}
	//�ƶ�����
	/*

	move (InputIterator first, InputIterator last, OutputIterator result)��C11�棬�����������е����Ԫ���ƶ���������С���Ҫע����ǣ���һ�������ƶ�����һ�������std::move������ͷ�ļ�<utility>�С�
	move_backward (BidirectionalIterator1 first,BidirectionalIterator1 last,BidirectionalIterator2 result)��C11�棬�����������е����Ԫ����β��ͷ�ƶ����������

	*/
	//��һ�������ƶ�������һ������,��Ԫ�ص���move
	template<typename InputIterator, typename OutputIterator>inline
		OutputIterator move(InputIterator first, InputIterator last, OutputIterator result)
	{
			while(first != last)
			{
				result = ztl::move(first);
				++result;
				++first;
			}
			return ztl::move(result);
	}
	template<typename BidirectionalIterator, typename OutputIterator>
	OutputIterator move_backward(BidirectionalIterator first, BidirectionalIterator last, OutputIterator result)
	{
		--last;
		--first;
		while(last != first)
		{
			result = ztl::move(last);
			++result;
			--last;
		}
		return ztl::move(result);
	}
	//����
	/*
	nth_element (RandomAccessIterator first, RandomAccessIterator nth,RandomAccessIterator last)�����������ţ�ʹ��ָ����λ�ó��ֵ�Ԫ�ؾ������������Ӧ���ڸ�λ�ó��ֵ��Ǹ�Ԫ�أ�����ָ��λ��֮ǰ��Ԫ�ض�С��ָ��λ��Ԫ�أ���ָ��λ��֮���Ԫ�ض�����ָ��λ��Ԫ�ء�
	nth_element (RandomAccessIterator first, RandomAccessIterator nth,RandomAccessIterator last, Compare comp)�����ذ汾
	is_partitioned (InputIterator first, InputIterator last, UnaryPredicate pred)��C11�棬�ж�����������Ϊ��pred��Ԫ���Ƿ���ͷ��
	partition (ForwardIterator first,ForwardIterator last, UnaryPredicate pred)�����������ţ�ʹ������ν��pred��Ԫ��λ����ǰ������ֵΪָ������ν��pred�ĵ�һ��Ԫ�صĵ�������
	stable_partition (BidirectionalIterator first,BidirectionalIterator last,UnaryPredicate pred)�����������ţ�ʹ������ν��pred��Ԫ����ǰ��������ν��pred��Ԫ���ں�������Ԫ���ڲ������˳�򲻱䡣һ��������ʱ������ʵ�ֱ�������
	partition_copy (InputIterator first, InputIterator last, OutputIterator1 result_true, OutputIterator2 result_false, UnaryPredicate pred)��C11�棬��������������ν��pred��Ԫ�ظ��Ƶ�result_true������Ԫ�ظ��Ƶ�result_false
	partition_point (ForwardIterator first, ForwardIterator last, UnaryPredicate pred)��C11�棬��������Ѿ���partition���۰���ҵ��ֽ�㡣
	*/

	//�Է�Χ��Ԫ�ؽ����з�,ν�ʷ���true�ķ�ǰ��,����false�ķź���,����BidirectionalIterator������p,[first,p)ʹν��Ϊtrue,[p,last)ʹν��false
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

				if(!( first < last ))
				{
					break;
				}
				ztl::swap(*first++, *last--);
			}
			return first;
	}

	template<typename BidirectionalIterator, typename BinaryPredicate = ztl::less_equal<void>>inline
		void nth_element(BidirectionalIterator first, BidirectionalIterator nth, BidirectionalIterator last, BinaryPredicate&& pred = ztl::less_equal<void>())
	{
			if(distance(first, last) > 0)
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
	//�ж���������pred true��Ԫ���ǲ���������ǰ��
	template<typename InputIterator, typename UnaryPredicate>
	bool is_partitioned(InputIterator first, InputIterator last, UnaryPredicate pred)
	{
		ztl::ztl_assert_true(first == last, "Continer is empty!");
		bool chose = true;
		int sum = 0;
		while(first != last)
		{
			if(pred(*first) != chose)
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
	//����
	/*
	partial_sort (RandomAccessIterator first, RandomAccessIterator middle,RandomAccessIterator last)��ʹ��[first, middle)Ϊ������������С����ЩԪ�ز�Ϊ����[middle,last)��Ԫ�����ⰲ�š�
	partial_sort (RandomAccessIterator first, RandomAccessIterator middle,RandomAccessIterator last, Compare comp)�����ذ汾
	partial_sort_copy (InputIterator first,InputIterator last, RandomAccessIterator result_first, RandomAccessIterator result_last)�����������и�����С��һ��Ԫ�ص���������У�������������
	partial_sort_copy (InputIterator first,InputIterator last, RandomAccessIterator result_first, RandomAccessIterator result_last, Compare comp)�����ذ汾
	is_sorted (ForwardIterator first, ForwardIterator last)��C11�汾���ж������Ƿ�Ϊ����
	is_sorted (ForwardIterator first, ForwardIterator last, Compare comp)��C11�汾�����ذ汾
	is_sorted_until (ForwardIterator first, ForwardIterator last)��C11�汾���������д�ͷ����ʼΪ����������еĽ�����
	is_sorted_until (ForwardIterator first, ForwardIterator last, Compare comp)��C11�汾�����ذ汾
	sort (RandomAccessIterator first, RandomAccessIterator last)������
	sort (RandomAccessIterator first, RandomAccessIterator last, Compare comp)�����ذ汾
	stable_sort ( RandomAccessIterator first, RandomAccessIterator last )���ȶ�����
	stable_sort ( RandomAccessIterator first, RandomAccessIterator last,Compare comp )�����ذ汾
	*/
	template<typename ForwardIterator, typename BinaryPredicate = ztl::less<void>>inline
		void merge_sort(ForwardIterator first, ForwardIterator last, BinaryPredicate&& pred)
	{
			if(last - first > 1)
			{
				ForwardIterator middle = first + ( last - first ) / 2;
				merge_sort(first, middle);
				merge_sort(middle, last);
				ztl::inplace_merge(first, middle, last, forward<BinaryPredicate>(pred));
			}
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
	//��������
	template<typename BidirectionalIterator, typename BinaryPredicate = ztl::less_equal<void>>inline
		void insert_sort(BidirectionalIterator first, BidirectionalIterator last, BinaryPredicate&& pred = ztl::less_equal<void>())
	{
			for(auto i = ztl::next(first); i < last; i++)
			{
				auto j = i;
				while(j > first && !pred(*( ztl::prev(j) ), *j))
				{
					ztl::swap(*( ztl::prev(j) ), *j);
					--j;
				}
			}
	}

	//���Լ��л�����,����,��������
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
			for(auto i = first; i < middle; ++i)
			{
				pop_heap(rbegin, rend, pred);
				rend--;
			}
	}
	template<typename ForwardIterator, typename BinaryPredicate = ztl::less<void>>inline
		bool is_sort(ForwardIterator  first, ForwardIterator  last, BinaryPredicate&& pred = ztl::less<void>())
	{
			ztl_assert_true(first == last, "Container is empty!");
			while(ztl::next(first) != last)
			{
				if(!pred(first, ztl::next(first)))
				{
					return false;
				}
				++first;
			}
			return true;
	}
	template < typename BidirectionalIterator,
		typename BinaryPredicate = ztl::less_equal < void >> inline
		BidirectionalIterator chose_pivot(BidirectionalIterator first, BidirectionalIterator last, BinaryPredicate&& pred = ztl::less_equal<void>())
	{
			ztl::ztl_assert_true(first == last, "error! Container is empty!");
			if(distance(first, last) == 1)
			{
				return ztl::move(first);
			}
			auto& pivot_iter = middle_element(first, ztl::next(first, distance(first, last) / 2), ztl::prev(last));
			ztl::swap(*first, *pivot_iter);

			////ѡ��һ��Ԫ��Ϊ��Ԫ��
			pivot_iter = first;
			const auto pivot = *pivot_iter;

			//������Ҫ��Ӧ����ʹν��Ϊtrue�����һ��Ԫ��Ϊ��Ԫ��,������true����ĳ�β
			auto middle = ztl::prev(ztl::partition(ztl::next(first), last, [&pivot, &pred](iterator_traits<BidirectionalIterator>::value_type& element)
			{
				return pred(element, pivot);
			}));
			//������middle������Ԫ��λ����
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

	template< typename RandomAccessIterator, typename CompareFunctor = ztl::less<void>>inline
		void heap_sort(RandomAccessIterator first, RandomAccessIterator last,
		CompareFunctor&& comp = ztl::less<void>())
	{
			make_heap(first, last, binary_negate<CompareFunctor>(comp));
			while(last != first)
			{
				pop_heap(first, last, binary_negate<CompareFunctor>(comp));
				--last;
			}

	}
}