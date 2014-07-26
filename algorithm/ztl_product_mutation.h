#pragma once
#include "ztl_math.h"
namespace ztl
{
	//变异算法，序列映射算法
	template<typename InputIterator1,typename InputIterator2,typename OutputIterator,typename BinaryOperator>
	OutputIterator transfrom(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, OutputIterator result, const BinaryOperator&& binary_op)
	{
		for(; first1 != last1;++first1,++first2,++result)
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
	//设置区间初始值是某种递增状态,质变算法
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
	template<class InputIterator, class UnaryOperator>inline
		void for_each(InputIterator first, InputIterator last, const UnaryOperator&& unary_op)
	{
		for(; first != last; ++first)
		{
			unary_op(*first);
		}
	}
	

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
		for(decltype(count) i = 0; i < count; ++i)
		{
			*first++ = gen();
		}
		return first;
	}
}