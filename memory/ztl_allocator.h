#pragma once
#include "ztl_tiny_allocator.h"
#include "../type/ztl_type_traits.h"
#include "iterator/ztl_iterator_operator.h"
#include "../algorithm/ztl_algorithm.h"
#include "memory_algorithm.h"
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
		typedef allocator<value_type> self_type;
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
			return static_cast<pointer_type>(raw_allocate(sizeof(value_type)* number));
		}
		static void deallocate(void* p, size_t nByte)
		{
			if (p!=nullptr)
			{
				if(nByte <= 512)
				{
					tiny_allocator::instance().deallocate(p, nByte);
				}
				else
				{
					operator delete(p);
				}
			}
			
		}
		static void* raw_allocate(const size_t nBtye)
		{
			if(nBtye <= 512)
			{
				return tiny_allocator::instance().allocate(nBtye);
			}
			else
			{
				return operator new(nBtye);
			}
		}
	};



}