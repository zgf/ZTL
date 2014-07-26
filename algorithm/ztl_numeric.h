#pragma once
#include "../ztl_iterator.h"
#include "../functional/ztl_operator_functor.h"
#include "../log/ztl_log.h"
#include "ztl_product_mutation.h"
#include "../container/ztl_vector.h"
#include "../type/ztl_type_traits.h"

namespace ztl
{
	//������ÿ��Ԫ�ض���ǰһ����������ٽ��в������õ����ս���������ۼ�
	template<typename InputIterator, typename Type, typename BinaryOperator = ztl::plus<void>>inline
		Type accumulate(InputIterator first, InputIterator last, Type init,const BinaryOperator&& binary_op=ztl::plus<void>())
	{
			for(; first != last; ++first)
			{
				init = binary_op(init, *first);
			}
			return init;
	}
	//����һ�����еĵ�һ��ֵ�Ѿ�֮��ÿ������ֵ��һ����Ԫ�����Ľ������׼���ǲ�ֵ��
	template<typename InputIterator, typename OutputIterator, typename BinaryOperator = ztl::minus<void>>inline
		OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, const BinaryOperator&& binary_op == ztl::minus<void>())
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
			ztl::vector<decltype(binary_op2(first1, first2))> result(ztl::distance(first1, last1));
			auto& result_end = ztl::transfrom(first1, last1, first2, result.begin(), binary_op2);
			return ztl::accumulate(result, result_end, value, binary_op1);
	}

	template<typename ForwardIterator, typename Type = int>inline
	void iota(ForwardIterator&& first, ForwardIterator&& last, Type&& init, Type&& increase = 1)
	{
		ztl::init_range(ztl::forward(first), ztl::forward(last), ztl::forward(init), ztl::forward(increase));
	}
	
	
}