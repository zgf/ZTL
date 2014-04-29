#pragma once
#include "ztl_allocator_base.h"
namespace ztl
{
	namespace memory
	{
		namespace allocator
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
				static pointer_type allocate(const_reference_type number)
				{
					return pool.allocate(sizeof(value_type) * number);
				}
				static void deallocate(void* p,size_t nByte)
				{
					pool.deallocate(p, nByte);
				}
				static void* raw_allocate(const size_t nBtye)
				{
					return pool.allocate(nBtye);
				}
			private:
				static allocator_base pool;
			};
			template <typename ValueType>
			allocator_base allocator<ValueType>::pool(4096,128);
			template<>
			class allocator<void>
			{
				typedef void value_type;
			};
		}
		
	}
}