#pragma once
//对指针本身的操作
#include "iterator_traits.h"
namespace ztl
{

	//advance
	//distance
	// increment iterator by offset, input iterators
	template<class InputIterator,
	class DifferenceType> inline
		void _advance(InputIterator& postion, DifferenceType offset, ztl::input_iterator)
	{
			for(; 0 < offset; --offset)
			{
				++postion;
			}
	}

	template<class ForwardIterator,
	class DifferenceType> inline
		void _advance(ForwardIterator& postion, DifferenceType offset, ztl::forward_iterator)
	{
			for(; 0 < offset; --offset)
			{
				++postion;
			}
	}

	template<class BidirectionalIterator,
	class DifferenceType> inline
		void _advance(BidirectionalIterator& postion, DifferenceType offset, ztl::bidirectional_iterator)
	{
			for(; 0 < offset; --offset)
			{
				++postion;
			}
			for(; offset < 0; ++offset)
			{
				--postion;
			}
	}

	template<class RandomAcessIterator,
	class DifferenceType> inline
		void _advance(RandomAcessIterator& postion, DifferenceType offset, ztl::random_acess_iterator)
	{
			postion += offset;
	}
	// increment iterator by offset, input iterators
	template<class InputIterator,
	class DifferenceType> inline
		void advance(InputIterator& postion, DifferenceType offset)
	{
			_advance(postion, offset, get_iterator_category(postion));
	}
	// return distance between iterators
	template<class InputIterator,
	class DifferenceType> inline
		void _distance(InputIterator first, InputIterator last, DifferenceType& offset,
		ztl::input_iterator)
	{
			for(; first != last; ++first)
				++offset;
	}

	template<class ForwardIterator,
	class DifferenceType> inline
		void _distance(ForwardIterator first, ForwardIterator last, DifferenceType& offset,
		ztl::forward_iterator)
	{
			for(; first != last; ++first)
				++offset;
	}

	template<class BidirectionalIterator,
	class DifferenceType> inline
		void _distance(BidirectionalIterator first, BidirectionalIterator last, DifferenceType& offset,
		bidirectional_iterator)
	{
			for(; first != last; ++first)
				++offset;
	}

	template<class RandomAcessIterator,
	class DifferenceType> inline
		void _distance(RandomAcessIterator first, RandomAcessIterator last, DifferenceType& offset,
		ztl::random_acess_iterator)
	{
			offset = last - first;
	}
	// return distance between iterators
	template<class InputIterator> inline
		typename ztl::iterator_traits<InputIterator>::different_type
		distance(InputIterator first, InputIterator last)
	{
			typename iterator_traits<InputIterator>::different_type offset = 0;
			_distance(first, last, offset, get_iterator_category(first));
			return offset;
	}
	template <class ForwardIterator>inline
	ForwardIterator next(ForwardIterator x,
		typename ztl::iterator_traits<ForwardIterator>::different_type n = 1)
	{
		advance(x, n); 
		return ztl::move(x);
	}

	template <class BidirectionalIterator>inline
		BidirectionalIterator prev_dispatch(BidirectionalIterator x,
		typename ztl::iterator_traits<BidirectionalIterator>::different_type n,bidirectional_iterator)
	{
			while(n != 0)
			{
				--x;
				--n;
			}
			return ztl::move(x);
	}
	template <class BidirectionalIterator>inline
		BidirectionalIterator prev_dispatch(BidirectionalIterator x,
		typename ztl::iterator_traits<BidirectionalIterator>::different_type n, random_acess_iterator)
	{
			return ztl::move(x -= n);
	}
	template <class BidirectionalIterator>inline
		BidirectionalIterator prev(BidirectionalIterator x,
		typename ztl::iterator_traits<BidirectionalIterator>::different_type n = 1)
	{
		return prev_dispatch(x,
			n,
			ztl::iterator_traits<BidirectionalIterator>::iterator_category());
	}
}