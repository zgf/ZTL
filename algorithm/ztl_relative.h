//���ʱ��㷨�����м�Ĺ�ϵ�㷨
#include "../functional/ztl_operator_functor.h"
#include "../tuple/ztl_pair.h"
#include "../type/ztl_type_traits.h"

namespace ztl
{
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
			if(ztl::distance(first1, last1) != ztl::distance(first2, last2));
			{
				return false;
			}
			return ztl::equal(first1, last1, first2, ztl::forward<BinaryPredicate>(pred))
	}
	template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate = ztl::less<void>>inline
		bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2, BinaryPredicate&& binary_op = ztl::less<void>())
	{
			for(; (first1 != last1) && (first2 != last2); first1++, first2++)
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
			return (first1 == last1) && (first2 != last2);
	}
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

#undef max
#undef min
	template<typename Type>inline
	Type max(Type&& left, Type&& right)
	{
		return left > right ? left : right;
	}
	template<typename Type>inline
	Type min(Type&& left, Type&& right)
	{
		return left < right ? left : right;
	}

	template<typename Type, typename LeftFunctor, typename RightFunctor>inline
		Type min_if(Type&& left, Type&& right,  LeftFunctor&& left_functor, RightFunctor&& right_functor)
	{
		if (left < right)
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
		Type max_if(Type&& left, Type&& right,  LeftFunctor&& left_functor,  RightFunctor&& right_functor)
	{
			return min_if(ztl::forward<Type>(right), ztl::forward<Type>(left), ztl::forward<Type>(left_functor), ztl::forward<Type>(right_functor));
	}
}