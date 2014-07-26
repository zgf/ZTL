#pragma once
#include "../ztl_iterator.h"
#include "../functional/ztl_operator_functor.h"
#include "../log/ztl_log.h"
#include "ztl_product_mutation.h"
#include "../container/ztl_vector.h"
#include "../type/ztl_type_traits.h"

namespace ztl
{
	//对序列每个元素都与前一个操作结果再进行操作，得到最终结果，例如累加
	template<typename InputIterator, typename Type, typename BinaryOperator = ztl::plus<void>>inline
		Type accumulate(InputIterator first, InputIterator last, Type init,const BinaryOperator&& binary_op=ztl::plus<void>())
	{
			for(; first != last; ++first)
			{
				init = binary_op(init, *first);
			}
			return init;
	}
	//保存一个序列的第一个值已经之后每个相邻值的一个二元操作的结果（标准的是差值）
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
	//前n元素和行成序列
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
	//binary_op2是应用与两个序列元素的，binary_op1是应用于生成的中间序列的
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