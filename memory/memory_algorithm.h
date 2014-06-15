#pragma once
namespace ztl
{

	/*构造*/

	template<typename PostionType, typename... ValueType>inline
		void construct(PostionType* target, ValueType&&... value)
	{
			new(target)PostionType(forward<ValueType&&>(value)...);
	}

	/*析构*/

	template<typename ValueType>inline
		void destory_dispath(ValueType* pointer, ztl::true_type)
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
		ForwardIterator uninitialized_copy_dispath(InputIterator start, InputIterator end, ForwardIterator des, ztl::false_type)
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
		ForwardIterator uninitialized_copy_dispath(InputIterator start, InputIterator end, ForwardIterator des, ztl::true_type)
	{
			using value_type = typename iterator_traits<InputIterator>::value_type;
			using different_type = typename iterator_traits<InputIterator>::different_type;

			const different_type count = ztl::distance(start, end);
			memcpy_s(&*des, count*sizeof(value_type), &*start, count*sizeof(value_type));
			des += count;
			return des;
	}

	template<typename InputIterator, typename ForwardIterator>inline
		ForwardIterator uninitialized_copy(InputIterator start, InputIterator end, ForwardIterator des)
	{
			return ztl::move(uninitialized_copy_dispath(start, end, des, ztl::integral_constant<bool, ztl::is_pod<typename ztl::iterator_traits<InputIterator>::value_type>::value>()));
	}
	inline wchar_t* uninitialized_copy(wchar_t* start, wchar_t* end, wchar_t* des)
	{
		memmove(des, start, (end - start)*sizeof(wchar_t));
		return ztl::move(des + (end - start));
	}
	inline unsigned char* uninitialized_copy(unsigned char* start, unsigned char* end, unsigned char* des)
	{
		memmove(des, start, (end - start)*sizeof(unsigned char));
		return ztl::move(des + (end - start));
	}
	inline char* uninitialized_copy(char* start, char* end, char* des)
	{
		memmove(des, start, (end - start)*sizeof(char));
		return ztl::move(des + (end - start));
	}

	template<typename ForwardIterator, typename ValueType>inline
		ForwardIterator uninitialized_fill(ForwardIterator start, ForwardIterator end, const ValueType& val)
	{
			while(start != end)
			{
				*start = val;
				start++;
			}
			return end;
	}
	inline char* uninitialized_fill(char* start, char* end, const char& val)
	{
		memset(start, val, end - start);
		return end;
	}
	inline wchar_t* uninitialized_fill(wchar_t* start, wchar_t* end, const wchar_t& val)
	{
		memset(start, val, end - start);
		return end;
	}
	inline unsigned char* uninitialized_fill(unsigned char* start, unsigned char* end, const unsigned char& val)
	{
		memset(start, val, end - start);
		return end;
	}
	template<typename ForwardIterator, typename DifferentType, typename ValueType>inline
		ForwardIterator uninitialized_fill_n(ForwardIterator start, DifferentType count, const ValueType& val)
	{
			return ztl::move(uninitialized_fill(start, start + count, val));
	};

	template<typename ForwardIterator, typename DifferentType >
	void uninitialized_fill_type(ForwardIterator start, DifferentType count, typename iterator_traits<ForwardIterator>::value_type value)
	{
		memset(start, 0, 40);
		//auto old_start = start;
		auto add_count = 1;
		auto loop_number = ztl::log2(count);
		*start = value;
		for(auto i = 0; i < loop_number; i++)
		{
			uninitialized_copy(start, start + add_count, start + add_count);
			add_count <<= 1;
		}
		auto rest_block = count - add_count;
		uninitialized_copy(start, start + rest_block, start + add_count);
	}
}