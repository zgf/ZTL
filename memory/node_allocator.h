#pragma once
#include <new>
namespace ztl
{
	template<typename NodeType>
	class node_allocator
	{
	public:
		using size_type = size_t;
		using node_type = NodeType;
		static const int node_size = sizeof(NodeType);
		static const int chunk_size = 4096;
		static const int chunk_list_size = chunk_size / node_size;
	public:
		node_allocator() :current_buff((char*)GetNewChunk())
		{
			rest_block = chunk_list_size;
		}
		
		node_type* allocate_node()
		{
			if(rest_block != 1)
			{
				auto result = (node_type*)current_buff;
				current_buff += node_size;
				rest_block--;
				return ztl::move(result);
			}
			else
			{

				auto new_buff = (char*)GetNewChunk();
				*(int**)(new_buff + rest_block*node_size) = (int*)current_buff;
				current_buff = new_buff;
				auto result = (node_type*)current_buff;
				current_buff += node_size;
				rest_block = chunk_list_size - 1;
				return ztl::move(result);
			}
		}
		void* GetNewChunk()
		{
			return _aligned_malloc(chunk_size, node_size);
		}
		static node_allocator& instance()
		{
			static node_allocator pool;
			return pool;
		}
	public:
		char* current_buff;
		unsigned short rest_block;
	};
}