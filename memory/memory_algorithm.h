#pragma once
#include "../algorithm/ztl_product_mutation.h"
#include "../algorithm/ztl_delete_copy_replace.h"
namespace ztl
{

	/*¹¹Ôì*/

	template<typename PostionType, typename... ValueType>inline
		void construct(PostionType* target, ValueType&&... value)
	{
			new(target)PostionType(forward<ValueType&&>(value)...);
	}

	/*Îö¹¹*/

	template<typename ValueType>inline
		void destory_dispath(ValueType*, ztl::true_type)
	{
			//pointer->~ValueType();
	}
	template<typename ValueType>inline
		void destory_dispath(ValueType* pointer, ztl::false_type)
	{
			pointer->~ValueType();
	}
	template<typename ValueType>inline
		void destory(ValueType* pointer)
	{
			destory_dispath(pointer, has_trivial_destructor<ValueType>::self_type());
	}

	template<typename ForwardIterator>inline
		void destory(ForwardIterator start, ForwardIterator end)
	{
			while(start != end)
			{
				destory(&*start);
				start++;
			}
	}

	template<typename InputIterator, typename ForwardIterator>inline
		ForwardIterator uninitialized_copy(InputIterator start, InputIterator end, ForwardIterator des)
	{
			return ztl::move(ztl::copy(start, end,des));
	}
	

	template<typename ForwardIterator, typename ValueType>inline
		ForwardIterator uninitialized_fill(ForwardIterator start, ForwardIterator end, const ValueType& value)
	{
		return ztl::move(fill(start,end, value));
	}
	
	template<typename ForwardIterator, typename DifferentType, typename ValueType>inline
		ForwardIterator uninitialized_fill_n(ForwardIterator start, DifferentType count, const ValueType& value)
	{
		return ztl::move(fill_n(start,start + count, value));
	};

	template<typename ForwardIterator, typename DifferentType, typename ValueType >
	void uninitialized_fill_type(ForwardIterator start, DifferentType count, const ValueType&   value)
	{
		return ztl::move(ztl::fill_type(ztl::start, ztl::count, ztl::value));
	}
}