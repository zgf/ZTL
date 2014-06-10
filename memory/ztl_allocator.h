#pragma once
#include "ztl_tiny_allocator.h"
#include "../type/ztl_type_traits.h"
#include "iterator/ztl_iterator_operator.h"
namespace ztl
{
	template<typename ValueType>
	class allocator
	{
	public:
		typedef ValueType value_type;
		typedef value_type* pointer_type;
		typedef const pointer_type const_pointer_type;
		typedef value_type& reference_type;
		typedef const reference_type const_reference_type;
		typedef ptrdiff_t difference_type;
		typedef size_t	size_type;
		typedef allocator<value_type> mySelf;
	public:
		template<typename U>
		struct rebind
		{
			typedef allocator<U> other;
		};
	public:
		allocator()
		{
		}
		~allocator()
		{
		}
		static pointer_type allocate(size_t number)
		{
			return static_cast<pointer_type>(operator new(sizeof(value_type)* number));
			//return pool.allocate();
		}
		static void deallocate(void* p, size_t nByte)
		{
			//pool.deallocate(p, nByte);
			if (p != nullptr)
			{
				operator delete(p);
			}
		}
		static void* raw_allocate(const size_t nBtye)
		{
			return operator new[](Byte);
			//return pool.allocate(nBtye);
		}
	private:
		//	static allocator_base pool;
	};
	/*template <typename ValueType>
	allocator_base allocator<ValueType>::pool(4096,256);
	template<>
	class allocator<void>
	{
	typedef void value_type;
	};*/

	/*构造*/

	template<typename PostionType, typename... ValueType>inline
	void construct(PostionType* target, ValueType&&... value)
	{
		new(target)PostionType(forward< ValueType&&>(value)...);
	}

	/*析构*/

	template<typename ValueType>
	void destory_dispath(ValueType* pointer, ztl::true_type)
	{
		//pointer->~ValueType();
	}
	template<typename ValueType>
	void destory_dispath(ValueType* pointer, ztl::false_type)
	{
		pointer->~ValueType();
	}
	template<typename ValueType>
	void destory(ValueType* pointer)
	{
		destory_dispath(pointer, has_trivial_destructor<ValueType>::self_type());
	}

	template<typename ForwardIterator>
	void destory(ForwardIterator start, ForwardIterator end)
	{
		while(start != end)
		{
			destory(&*start);
			start++;
		}
	}

	template<typename InputIterator, typename ForwardIterator>
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
	template<typename InputIterator, typename ForwardIterator>
	ForwardIterator uninitialized_copy_dispath(InputIterator start, InputIterator end, ForwardIterator des, ztl::true_type)
	{
		memcpy(&*des, &*start, (end - start)*sizeof(typename iterator_traits<InputIterator>::value_type));
		des += ztl::distance(start, end);
		return des;
	}

	template<typename InputIterator, typename ForwardIterator>
	ForwardIterator uninitialized_copy(InputIterator start, InputIterator end, ForwardIterator des)
	{
		return ztl::move(uninitialized_copy_dispath(start, end, des, ztl::integral_constant<bool, ztl::is_pod<typename ztl::iterator_traits<InputIterator>::value_type>::value>()));
	}
	wchar_t* uninitialized_copy(wchar_t* start, wchar_t* end, wchar_t* des)
	{
		memmove(des, start, (end - start)*sizeof(wchar_t));
		return ztl::move(des + (end - start));
	}
	unsigned char* uninitialized_copy(unsigned char* start, unsigned char* end, unsigned char* des)
	{
		memmove(des, start, (end - start)*sizeof(unsigned char));
		return ztl::move(des + (end - start));
	}
	char* uninitialized_copy(char* start, char* end, char* des)
	{
		memmove(des, start, (end - start)*sizeof(char));
		return ztl::move(des + (end - start));
	}

	template<typename ForwardIterator, typename ValueType>
	ForwardIterator uninitialized_fill(ForwardIterator start, ForwardIterator end, const ValueType& val)
	{
		while(start != end)
		{
			*start = val;
			start++;
		}
		return end;
	}
	char* uninitialized_fill(char* start, char* end, const char& val)
	{
		memset(start, val, end - start);
		return end;
	}
	wchar_t* uninitialized_fill(wchar_t* start, wchar_t* end, const wchar_t& val)
	{
		memset(start, val, end - start);
		return end;
	}
	unsigned char* uninitialized_fill(unsigned char* start, unsigned char* end, const unsigned char& val)
	{
		memset(start, val, end - start);
		return end;
	}
	template<typename ForwardIterator, typename DifferentType, typename ValueType>
	ForwardIterator uninitialized_fill_n(ForwardIterator start, DifferentType count, const ValueType& val)
	{
		return ztl::move(uninitialized_fill(start, start + count, val));
	};
}