#include <iostream>
#include "ztl_buddy_memory_allocator.h"
#include "ztl_tiny_allocator.h"
using std::cout;
using std::endl;
namespace ztl
{
	tiny_allocator::tiny_allocator(size_type size , size_type object_size ) :max_object_size(object_size), list_size(size)
	{
		memset(free_list, 0, array_size * sizeof(node_type));
		initObjectSizeList();
		
	}
	tiny_allocator::~tiny_allocator()
	{

	}
	void* tiny_allocator::allocate(const size_type nBtye)
	{
		node_type result = nullptr;
		auto index = GetObjectSizeListIndex(nBtye);
		if(free_list[index] == nullptr)
		{
			//加载节点链,多分配一块返回出去
			free_list[index] = (node_type)GetNewChunk(index);
			result = free_list[index];
			free_list[index] += object_size_list[index];
		}
		else if(*(int*)(free_list[index]) == -1)
		{
			//还剩最后一块
			result = free_list[index];
			//加载新的节点链
			free_list[index] = (node_type)GetNewChunk(index);
		}
		else
		{
			result = free_list[index];
			free_list[index] = *(char**)free_list[index];
			//free_list[index] += object_size_list[index];
		}
		return std::move(result);

	}

	void tiny_allocator::deallocate(void* p, size_type nByte)
	{
		auto index = GetObjectSizeListIndex(nByte);
		*(int*)(p) = (int)free_list[index];
		free_list[index] = (node_type)p;
	}
	

	inline void* tiny_allocator::GetNewChunk(const size_type index)
	{
		auto start_pointer = buddy_system::instance().allocate(index);
		initChunkList((node_type)start_pointer, object_size_list[index], 64);
		//*(int*)((node_type)start_pointer + (64 - 1) * (object_size_list[index]))=-1;
		return start_pointer;
	}
	inline void tiny_allocator::initObjectSizeList()
	{
		auto i = 0;
		object_size_list[i] = 8;
		i++;
		while(i < array_size)
		{
			object_size_list[i] = object_size_list[i - 1] << 1;
			i++;
		}
	}
	template<typename InteraglType>inline
		InteraglType log2_sepc(InteraglType x)
	{
			static const unsigned char log_2[257] = {
				1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
				3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
				4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
				5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
				5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
				6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
				6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
				6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,6
			};
			std::ptrdiff_t l = -1;
			while(x > 256)
			{
				l += 6; x >>= 8;
			}
			return std::move(l + log_2[x]);
	}
	inline size_t tiny_allocator::GetObjectSizeListIndex(const size_t nByte)
	{
		return  std::move(log2_sepc(nByte));
	}
	inline void tiny_allocator::initChunkList(node_type start_pointer,const size_type block_size, const size_type list_count)
	{
		for(size_type i = 1; i < list_count; i++)
		{
			*reinterpret_cast<node_type*>(start_pointer) = start_pointer + block_size;
			start_pointer += block_size;
		}
		*(int*)start_pointer = -1;
	}
}