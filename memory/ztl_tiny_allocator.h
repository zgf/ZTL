#pragma once
#include <new>
#include <Windows.h>
#include "ztl_buddy_memory_allocator.h"
namespace ztl
{
	class tiny_allocator
	{
	public:
		const static int		array_size = 64;
		const static int		alis_size  = 8;
		typedef size_t			size_type;
		typedef tiny_allocator	self_type;
		typedef char*			node_type;
	public:
		tiny_allocator(size_type size = 16 , size_type object_size = 512) :max_object_size(object_size), list_size(size)
		{
			memset(free_list, 0, array_size * sizeof(node_type));
			initObjectSizeList();
		}
		~tiny_allocator()
		{

		}
	public:
		void* allocate(const size_type nBtye)
		{
			auto index = GetObjectSizeListIndex(nBtye);
			if(free_list[index]==nullptr)
			{
				//加载节点链,多分配一块返回出去
				free_list[index] = (node_type)GetNewChunk(object_size_list[index]);
				auto result = free_list[index];
				free_list[index] += object_size_list[index];
				return result;
			}
			else if(*free_list[index] == 0)
			{
				//还剩最后一块
				auto result = free_list[index];
				//加载新的节点链
				free_list[index] = (node_type)GetNewChunk(object_size_list[index]);
				return result;
			}
			else
			{
				auto result = free_list[index];
				free_list[index] += object_size_list[index];
				return result;
			}
		}
	
		void deallocate(void* p, size_type nByte)
		{
			//pool.deallocate(p, nByte);
			auto index = GetObjectSizeListIndex(nByte);
			*(int*)(p) = (int)free_list[index];
			free_list[index] = (node_type)p;
		}
	public:
		static self_type& instance()
		{
			static self_type tiny_object;
			return tiny_object;
		}
		
		void* GetNewChunk(const size_type unit_size)
		{
			auto nByte = unit_size << 4;
			nByte = (nByte < 512) ? 512 : nByte;
			auto start_pointer = operator new(nByte);//buddy_system::instance().allocate(nByte);
			initChunkList((char*)start_pointer, unit_size, nByte / unit_size);
			return start_pointer;
			//return operator new(nBtye);
			//return _aligned_malloc(nBtye,alis_size);
			//return VirtualAlloc(NULL, nBtye, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
			//return HeapAlloc(GetProcessHeap(), NULL, nBtye);
		}
	private:
		void initObjectSizeList()
		{
			for(auto i = 0; i < array_size; i++)
			{
				object_size_list[i] = alis_size * (i+1);
			}
		}
		size_type GetObjectSizeListIndex(const size_type nByte)
		{
			if (nByte <=8)
			{
				return 0;
			}
			else
			{
				return  ((1 << (ztl::log2(nByte - 1) + 1)) >> 3) - 1;
			}
			
		}
		void initChunkList(node_type start_pointer,size_type block_size,const size_type list_count)
		{
			//auto old_ptr = start_pointer;
			for(auto i = 1; i < list_count; i++)
			{
				*reinterpret_cast<node_type*>(start_pointer) = start_pointer + block_size;
				start_pointer += block_size;
			}
			*start_pointer = 0;
		//	char i = *(old_ptr + (list_size-1) * (block_size));
		//	std::cout << (int)i << std::endl;
		}
	private:

		node_type			free_list[array_size];
		size_type			max_object_size;
		size_type			list_size;
		int					object_size_list[array_size];
	};

}