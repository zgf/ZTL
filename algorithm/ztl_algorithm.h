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
	//旋转,逆序
	/*
	reverse (BidirectionalIterator first, BidirectionalIterator last)：把序列中的元素逆序
	reverse_copy (BidirectionalIterator first, BidirectionalIterator last, OutputIterator result)：复制序列的逆序
	rotate (ForwardIterator first, ForwardIterator middle,ForwardIterator last)：等效于循环左移序列，使得迭代器middle所指的元素成为首元素。
	rotate_copy (ForwardIterator first, ForwardIterator middle, ForwardIterator last, OutputIterator result)：等效于循环左移序列并复制到新的存储空间，使得迭代器middle所指的元素成为首元素。
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
	/*效率不行*/
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
	//效率不如上面的
	template<typename BidirectionalIterator> inline
	BidirectionalIterator rotate_dispath(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last, ztl::bidirectional_iterator)
	{
	reverse(first, middle);
	reverse(middle, last);
	reverse(first, last);
	return first;
	}


	//字符串左旋k位
	//[开始位置,中间位置,结束位置)
	//区间1[开始位置,中间位置),区间2[中间位置,结束位置)
	template<typename BidirectionalIterator> inline
		BidirectionalIterator rotate(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last)
	{
			return rotate_dispath(first, middle, last, iterator_traits<BidirectionalIterator>::iterator_category());
	}

	//交换
	/*
	iter_swap (ForwardIterator1 a, ForwardIterator2 b)：交换两个迭代器所指的元素对象
	swap (T& a, T& b)：交换两个对象。优先使用移动语义
	swap(T (&a)[N], T (&b)[N])：交换两个对象数组
	swap_ranges (ForwardIterator1 first1, ForwardIterator1 last1,ForwardIterator2 first2)：交换两个序列中对应元素
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

	//搜索
	/*
	ForwardIterator adjacent_find (ForwardIterator first, ForwardIterator last)：在序列中发现第一对相邻且值相等的元素
	ForwardIterator adjacent_find (ForwardIterator first, ForwardIterator last, BinaryPredicate pred)：重载版本。用给定谓词pred代替了operator ==
	binary_search (ForwardIterator first, ForwardIterator last,const T& val)：对一个升序序列做二分搜索，判定序列中是否有给定值val。
	binary_search (ForwardIterator first, ForwardIterator last,const T& val, Compare comp)：重载版本。用给定谓词pred代替了operator ==
	find (InputIterator first, InputIterator last, const T& val)：对一个输入序列，找到第一个等于给定值的元素
	ForwardIterator1 find_end (ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2)：在序列[first1,last1)中，找到序列[first2,last2)最后出现的位置
	ForwardIterator1 find_end (ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2, BinaryPredicate pred)：重载版本，用给定谓词pred代替operator==
	find_first_of (InputIterator first1, InputIterator last1, ForwardIterator first2, ForwardIterator last2)：在序列[first1, last1)中，找到集合[first2,last2)中任何一个元素的第一次出现
	find_first_of (InputIterator first1, InputIterator last1, ForwardIterator first2, ForwardIterator last2, BinaryPredicate pred)：重载版本，用给定谓词pred代替operator==
	find_if (InputIterator first, InputIterator last, UnaryPredicate pred)：在序列中返回满足谓词pred的第一个元素
	find_if_not (InputIterator first, InputIterator last, UnaryPredicate pred)：C11算法，在序列中返回不满足谓词pred的第一个元素
	search (ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2)：在序列[first1,last1)中，找到序列[first2,last2)首次出现的位置
	search (ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2, BinaryPredicate pred)：重载版本，用给定谓词pred代替operator==
	search_n (ForwardIterator first, ForwardIterator last, Size count, const T& val)：给定序列中，搜索给定值val连续出现n次的位置
	search_n ( ForwardIterator first, ForwardIterator last, Size count, const T& val, BinaryPredicate pred )：重载版本，用给定谓词pred代替operator==
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

	//在序列1里面找序列2第一次出现位置
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
	//在序列1里面找序列2最后一次出现位置
	template<typename BidirectionalIterator1, typename BidirectionalIterator2, typename BinaryPredicate = ztl::equal_to<void>>inline
		BidirectionalIterator2 find_end(BidirectionalIterator1 first1, BidirectionalIterator1 last1, BidirectionalIterator2 first2, BidirectionalIterator2 last2, BinaryPredicate&& pred = ztl::equal_to<void>())
	{
			return find_begin(reverse_iterator<BidirectionalIterator1>(last1), reverse_iterator<BidirectionalIterator1>(first1), reverse_iterator<BidirectionalIterator2>(last2), reverse_iterator<BidirectionalIterator2>(first2), forward<BinaryPredicate>(pred));
	}
	//如果找到了,返回搜索到的值,如果没找到,返回该元素应该在前方插入的位置.
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
	//在序列1中找到集合2任意元素第一次出现的位置
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
	//在序列1中找到集合2任意元素最后第一次出现的位置
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
	//在序列[first1,last1)中，找到序列[first2,last2)首次出现的位置
	template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate = ztl::equal_to<void>>inline
		InputIterator1 search(InputIterator1&& first1, InputIterator1&& last1, InputIterator2&& first2, InputIterator2&& last2, BinaryPredicate&& pred = ztl::equal_to<void>())
	{
			return find_begin(forward<InputIterator>(first1),
				forward<InputIterator>(last1),
				forward<InputIterator>(first2),
				forward<InputIterator>(last2),
				forward<BinaryPredicate>(pred));
	}
	//给定序列中，搜索给定值val连续出现n次的位置
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
	//有序序列中的边界查找
	/*
	equal_range(forIterBegin, forIterEnd, targetVal)：在已排序的序列中查找目标值的位置范围；返回范围的下界与上界。对于随机迭代器，用二分查找；否则线性查找。返回pair<ForwardIterator,ForwardIterator>
	equal_range(forIterBegin, forIterEnd, targetVal, binPred)：重载版本，其中binPred是给定的序关系函数。
	lower_bound(forIterBegin, forIterEnd, targetVal)：在升序的序列中查找第一个不小于targetVal的元素。实际上是二分查找。
	lower_bound(forIterBegin, forIterEnd, targetVal, binPred)：重载版本，其中binPred是给定的序关系函数。
	upper_bound(forIterBegin, forIterEnd, val)：在已排序的序列中查找目标值val出现的上界（即第一个大于目标值val的元素的位置）。
	upper_bound(forIterBegin, forIterEnd, val, binPred)：重载版本，其中binPred是给定的序关系函数。
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
	//对序列的每个元素执行函数调用
	/*	for_each(inIterBegin, inIterEnd,ufunc)：用函数对象ufunc调用序列中每一项元素
		transform (InputIterator first1, InputIterator last1, OutputIterator result, UnaryOperation op)：对序列中每一个元素，执行一元操作op，结果写入另一序列中
		transform (InputIterator1 first1, InputIterator1 last1,InputIterator2 first2, OutputIterator result,BinaryOperation binary_op)：对两个序列中对应的每一对元素，执行二元操作binary_op，结果写入另一序列中*/
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
	//测试序列的性质
	/*
	all_of (InputIterator first, InputIterator last, UnaryPredicate pred)：C11算法。如果序列所有元素均满足谓词pred，则返回true
	any_of (InputIterator first, InputIterator last, UnaryPredicate pred)：C11算法。如果序列存在元素满足谓词pred，则返回true
	none_of (InputIterator first, InputIterator last, UnaryPredicate pred)：C11版。如果序列中所有元素不满足为此pred，则返回true
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
	//比较
	/*
	equal(inIter1Begin, inIter1End, inIter2Begin)：比较两个序列的对应元素是否相等
	equal(inIter1Begin, inIter1End, inIter2Begin, binPred)：重载版本，其中binPred是给定的相等比较函数。
	lexicographical_compare(inIter1Begin, inIter1End, inIter2Begin, inIter2End)：对两个序列做词典比较。两个序列的对应元素用 > 运算符比较。如果第一个序列在词典序下小于第二个序列，返回true
	lexicographical_compare(inIter1Begin, inIter1End, inIter2Begin, inIter2End, binPred)：重载版本，其中binPred是给定的“小于”比较函数。
	mismatch (InputIterator1 first1, InputIterator1 last1,InputIterator2 first2)：比较两个序列的对应元素，返回用std::pair表示的第一处不匹配在两个序列的位置。比较时使用==运算符。
	mismatch (InputIterator1 first1, InputIterator1 last1,InputIterator2 first2, BinaryPredicate pred)：重载版本，其中binPred是给定的“相等”比较函数。
	*/
	//    equal:                    如果两个序列在标志范围内元素都相等，返回true。重载版本使用输入的操作符代替默认的等于操作符。
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

	//复制
	/*
	copy (InputIterator first, InputIterator last, OutputIterator result)：复制一个序列到另一个序列
	copy_backward (BidirectionalIterator1 first,BidirectionalIterator1 last,BidirectionalIterator2 result)：把一个序列复制到另一个序列，按照由尾到头顺序依次复制元素。
	copy_if (InputIterator first, InputIterator last,OutputIterator result, UnaryPredicate pred)：C11算法。复制序列中所有满足谓词pred的元素
	copy_n (InputIterator first, Size n, OutputIterator result)：C11算法。复制序列中前n个元素
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
	/*内存初始化*/
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
	//删除[编辑]
	/*
	remove(ForwardIterator first, ForwardIterator last, const T& val)：删除序列中等于给定值的所有元素，保留的元素存放在容器的前部且相对次序不变。容器的size不变。
		remove_if(ForwardIterator first, ForwardIterator last, UnaryPredicate pred)：删除序列中满足给定谓词pred的元素，保留的元素存放在容器的前部且相对次序不变。容器的size不变。
		remove_copy(InputIterator first, InputIterator last, OutputIterator result, const T& val)：把一个序列中不等于给定值的元素复制到另一个序列中。
		remove_copy_if(InputIterator first, InputIterator last, OutputIterator result, UnaryPredicate pred)：把一个序列中不满足给定谓词pred的元素复制到另一个序列中。
	//	替换[编辑]
		replace(ForwardIterator first, ForwardIterator last, const T& old_value, const T& new_value)：把序列中为给定值的元素替换为新值
		replace_if(ForwardIterator first, ForwardIterator last, UnaryPredicate pred, const T& new_value)：把序列中满足给定谓词pred的元素替换为新值
		replace_copy(InputIterator first, InputIterator last, OutputIterator result, const T& old_value, const T& new_value)：复制序列，对于等于老值的元素复制时使用新值
		replace_copy_if(InputIterator first, InputIterator last, OutputIterator result, UnaryPredicate pred, const T& new_value)：复制序列，对于满足给定谓词pred的元素复制新值。
	*/
	//remove (ForwardIterator first, ForwardIterator last, const T& val)：删除序列中等于给定值的所有元素，保留的元素存放在容器的前部且相对次序不变。容器的size不变。
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

	//remove (ForwardIterator first, ForwardIterator last, const T& val)：删除序列中等于给定值的所有元素，保留的元素存放在容器的前部且相对次序不变。容器的size不变。
	template<typename ForwardIterator, typename T>inline
		ForwardIterator remove(ForwardIterator&& first, ForwardIterator&& last, const T& value)
	{
			return remove_if(ztl::forward<ForwardIterator>(first), ztl::forward<ForwardIterator>(last), [&value](const iterator_traits<ForwardIterator>::value_type& target)
			{
				return target == value;
			});
	}
	//replace_if (ForwardIterator first, ForwardIterator last, UnaryPredicate pred, const T& new_value )：把序列中满足给定谓词pred的元素替换为新值
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

	//result是新区间的超尾
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
	//计数

	//单值过滤
	/*
	unique (ForwardIterator first, ForwardIterator last)：对序列中一群连续的相等的元素，仅保留第一个元素，该群其他元素被这群元素之后的其他值的元素替换。该函数不改变这些值的相互顺序，不改变容器的size。函数返回值为最后一个保留元素的下一个位置（past-the-end element）。建议函数返回后resize容器对象。
	unique (ForwardIterator first, ForwardIterator last,BinaryPredicate pred)：重载版本，用二元谓词pred替换operator==算符。
	unique_copy (InputIterator first, InputIterator last,OutputIterator result)：把一个序列中的元素拷贝到另一个序列；对于一群连续的相等的元素，仅拷贝第一个元素。
	unique_copy (InputIterator first, InputIterator last,OutputIterator result, BinaryPredicate pred)：重载版本，用二元谓词pred替换operator==算符。
	*/
	//	unique(ForwardIterator first, ForwardIterator last, BinaryPredicate pred)：重载版本，用二元谓词pred替换operator == 算符。
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
	//填充
	/*
	fill (ForwardIterator first, ForwardIterator last, const T& val)：用给定值填充序列中每个元素
	fill_n (OutputIterator first, Size n, const T& val)：用给定值填充序列的n个元素
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
	//生成
	/*
	generate (ForwardIterator first, ForwardIterator last, Generator gen)：对序列中每个元素，依次调用函数gen的返回值赋值。
	generate_n (OutputIterator first, Size n, Generator gen)：对序列中的n个元素，依次调用指定函数的返回值赋值。
	void init_range(ForwardIterator first, ForwardIterator last, Type init, Type increase = 1)设置区间初始值是某种递增状态
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
	//数学算法
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
	//保存一个序列的第一个值已经之后每个相邻值的一个二元操作的结果（标准的是差值）
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
	//堆操作
	/*
	is_heap (RandomAccessIterator first, RandomAccessIterator last)：C11版，判断序列是否为二叉堆
	is_heap_until (RandomAccessIterator first, RandomAccessIterator last,Compare comp)：C11版，重载版本
	is_heap_until (RandomAccessIterator first, RandomAccessIterator last)：C11版，返回有效二叉堆的最末范围
	is_heap (RandomAccessIterator first, RandomAccessIterator last,Compare comp)：C11版，重载版本
	make_heap (RandomAccessIterator first, RandomAccessIterator last)：对于一个序列，其元素任意放置，原地(in-place)构造一颗最大树。
	make_heap (RandomAccessIterator first, RandomAccessIterator last,Compare comp )：重载版本
	pop_heap (RandomAccessIterator first, RandomAccessIterator last)：堆的根节点被卸载到last-1位置，堆的元素数目减1并保持堆性质。
	pop_heap (RandomAccessIterator first, RandomAccessIterator last,Compare comp)：重载版本
	push_heap (RandomAccessIterator first, RandomAccessIterator last)：对一个范围为[first,last-1)的堆，增加一个新元素。新元素最初保存在last-1位置。
	push_heap(r,r,bpred)：重载版本
	sort_heap (RandomAccessIterator first, RandomAccessIterator last)：对一个堆，执行原地堆排序，得到一个升序结果。
	sort_heap (RandomAccessIterator first, RandomAccessIterator last,Compare comp)：重载版本
	*/
	// 下沉操作,当前节点不满足堆的性质,其后节点都满足
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

	//上浮操作.当前节点不满足堆的性质,之前节点都满足
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
			ztl::swap(*first, *( --last ));
			heapify_sink(first, first, last, forward<CompareFunctor>(comp));
	}
	//集合操作
	/*
	includes ( InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2 )：判断第2个已为升序的序列的元素是否都出现在第1个升序序列中
	includes ( InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp )：重载版本，用给定谓词pred代替operator<
	set_difference (InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result)：两个升序序列之差
	set_difference (InputIterator1 first1, InputIterator1 last1,InputIterator2 first2, InputIterator2 last2,OutputIterator result, Compare comp)：重载版本
	set_intersection (InputIterator1 first1, InputIterator1 last1,InputIterator2 first2, InputIterator2 last2,OutputIterator result)：两个升序序列（集合）的交
	set_intersection (InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare comp)：重载版本
	set_symmetric_difference (InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result)：两个升序序列的对称差
	set_symmetric_difference (InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare comp)：重载版本
	set_union (InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result)：两个升序序列的并
	set_union (InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare comp)：重载版本
	*/
	//判断有序序列序列2是否是序列1的子序列,可以不连续
	template< typename InputIterator1, typename InputIterator2, typename CompareFunctor = ztl::less<void>>inline
		bool includes(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, CompareFunctor&& comp = ztl::less<void>())
	{
			//这里很有意思啊...用<来构造>=语义，通过测试<和>=来构造==的语义
			//因为这是集合算法，集合只要求元素支持<没说要支持==
			for(; first2 != last2; ++first1)
			{
				//这里如果comp是true也就说*first2<*first1 也就是不可能满足*first2==*first1 所以false
				//反之说明*first2>=*first1
				if(first1 == last1 || comp(*first2, *first1))
				{
					return false;
				}
				//这里true说明 *first1 >= *first2
				//以上两个条件都满足的前提就是 ==的情况
				if(!comp(*first1, *first2))
				{
					++first2;
				}
			}
			return true;
	}
	

	//从区间1中减去2中元素,重复元素同理
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
	//set的交集计算，重复元素是 出现min（n1,n2）次
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
	//合并两个有序序列
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

	//集合的对称差集
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
	//合并
	/*
	inplace_merge (BidirectionalIterator first, BidirectionalIterator middle,BidirectionalIterator last)：对两个升序的序列[first,middle) 与[middle,last)，执行原地合并，合并后的序列仍保持升序。空间复杂度O(1)，时间复杂度O(N)
	inplace_merge (BidirectionalIterator first, BidirectionalIterator middle,BidirectionalIterator last, Compare comp)重载版本。operator< 被函数对象comp代替。
	merge (InputIterator1 first1, InputIterator1 last1,InputIterator2 first2, InputIterator2 last2,OutputIterator result)：两个升序的序列合并，结果序列保持升序。
	merge (InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare comp)：重载版本。operator< 被函数对象comp代替。
	*/
	//合并两个已排序序列
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
	//最小最大
	/*
	min (const T& a, const T& b)：两个值中的最小值。
	min (const T& a, const T& b, Compare comp)：重载版本
	max (const T& a, const T& b)：两个值中的最大值。
	max (const T& a, const T& b, Compare comp)：重载版本
	min_element (ForwardIterator first, ForwardIterator last)：返回序列中最小值的迭代器
	min_element (ForwardIterator first, ForwardIterator last,Compare comp) ：重载版本
	max_element (ForwardIterator first, ForwardIterator last)：返回序列中最大值的迭代器
	max_element (ForwardIterator first, ForwardIterator last,Compare comp) ：重载版本
	max (initializer_list<T> il, Compare comp)：C11版本
	minmax (const T& a, const T& b)：C11版，返回由最小值与最大值构成的std:pair
	minmax (const T& a, const T& b, Compare comp)：重载版本
	minmax (initializer_list<T> il)：C11版，返回由初始化列表中最小元素与最大元素构成的std:pair
	minmax (initializer_list<T> il, Compare comp)：重载版本
	minmax_element (ForwardIterator first, ForwardIterator last)：C11版，返回由序列中最小元素与最大元素构成的std:pair
	minmax_element (ForwardIterator first, ForwardIterator last, Compare comp)：重载版本
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

	
	//返回三个迭代器指向的元素的中值元素的迭代器
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
	//三个迭代器之中的最小值
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
	//移动语义
	/*

	move (InputIterator first, InputIterator last, OutputIterator result)：C11版，把输入序列中的逐个元素移动到结果序列。需要注意的是，把一个对象移动给另一个对象的std::move定义在头文件<utility>中。
	move_backward (BidirectionalIterator1 first,BidirectionalIterator1 last,BidirectionalIterator2 result)：C11版，把输入序列中的逐个元素自尾到头移动到结果序列

	*/
	//把一个序列移动给另外一个序列,逐元素调用move
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
	//划分
	/*
	nth_element (RandomAccessIterator first, RandomAccessIterator nth,RandomAccessIterator last)：对序列重排，使得指定的位置出现的元素就是有序情况下应该在该位置出现的那个元素，且在指定位置之前的元素都小于指定位置元素，在指定位置之后的元素都大于指定位置元素。
	nth_element (RandomAccessIterator first, RandomAccessIterator nth,RandomAccessIterator last, Compare comp)：重载版本
	is_partitioned (InputIterator first, InputIterator last, UnaryPredicate pred)：C11版，判断序列中满足为此pred的元素是否都在头部
	partition (ForwardIterator first,ForwardIterator last, UnaryPredicate pred)：对序列重排，使得满足谓词pred的元素位于最前，返回值为指向不满足谓词pred的第一个元素的迭代器。
	stable_partition (BidirectionalIterator first,BidirectionalIterator last,UnaryPredicate pred)：对序列重排，使得满足谓词pred的元素在前，不满足谓词pred的元素在后，且两组元素内部的相对顺序不变。一般是用临时缓冲区实现本函数。
	partition_copy (InputIterator first, InputIterator last, OutputIterator1 result_true, OutputIterator2 result_false, UnaryPredicate pred)：C11版，输入序列中满足谓词pred的元素复制到result_true，其他元素复制到result_false
	partition_point (ForwardIterator first, ForwardIterator last, UnaryPredicate pred)：C11版，输入队列已经是partition，折半查找到分界点。
	*/

	//对范围内元素进行切分,谓词返回true的放前面,返回false的放后面,返回BidirectionalIterator迭代器p,[first,p)使谓词为true,[p,last)使谓词false
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
	//判断序列满足pred true的元素是不是在序列前方
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
	//排序
	/*
	partial_sort (RandomAccessIterator first, RandomAccessIterator middle,RandomAccessIterator last)：使得[first, middle)为整个序列中最小的那些元素并为升序。[middle,last)的元素任意安排。
	partial_sort (RandomAccessIterator first, RandomAccessIterator middle,RandomAccessIterator last, Compare comp)：重载版本
	partial_sort_copy (InputIterator first,InputIterator last, RandomAccessIterator result_first, RandomAccessIterator result_last)：从输入序列复制最小的一批元素到结果序列中，并按升序排列
	partial_sort_copy (InputIterator first,InputIterator last, RandomAccessIterator result_first, RandomAccessIterator result_last, Compare comp)：重载版本
	is_sorted (ForwardIterator first, ForwardIterator last)：C11版本，判断序列是否为升序
	is_sorted (ForwardIterator first, ForwardIterator last, Compare comp)：C11版本，重载版本
	is_sorted_until (ForwardIterator first, ForwardIterator last)：C11版本，返回序列从头部开始为升序的子序列的结束处
	is_sorted_until (ForwardIterator first, ForwardIterator last, Compare comp)：C11版本，重载版本
	sort (RandomAccessIterator first, RandomAccessIterator last)：排序
	sort (RandomAccessIterator first, RandomAccessIterator last, Compare comp)：重载版本
	stable_sort ( RandomAccessIterator first, RandomAccessIterator last )：稳定排序
	stable_sort ( RandomAccessIterator first, RandomAccessIterator last,Compare comp )：重载版本
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
	//插入排序
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

	//会自己切换快排,堆排,插入排序
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

			////选第一个元素为枢元素
			pivot_iter = first;
			const auto pivot = *pivot_iter;

			//这里需要的应该是使谓词为true的最后一个元素为枢元素,而不是true区间的超尾
			auto middle = ztl::prev(ztl::partition(ztl::next(first), last, [&pivot, &pred](iterator_traits<BidirectionalIterator>::value_type& element)
			{
				return pred(element, pivot);
			}));
			//交换后middle就是枢元素位置了
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