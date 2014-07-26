#pragma once
#include "../algorithm/ztl_algorithm.h"
#include "boost/dynamic_bitset/dynamic_bitset.hpp"
#include "../type/complier_algorithm.h"
namespace ztl
{
	class tiny_allocator;
	struct buddy_node
	{
		size_t		current_offset;
		buddy_node* next;
	public:
		buddy_node() = default;
		buddy_node(size_t tcurrent_offset, buddy_node* tnext) :current_offset(tcurrent_offset), next(tnext)
		{
		}
	};
	class buddy_system
	{
	public:
		typedef size_t size_type;
	public:
		static const size_type page_size = 4096;
		static const size_type page_number = 1024*256;
		static const size_type unit_size = 512;
		static const size_type max_unit_number = page_size*page_number / unit_size;
		static const size_type max_order = ztl::Log2<max_unit_number>::value + 1;
		static const size_type unit_order = ztl::Log2<unit_size>::value;
	public:
		buddy_system();

	public:
		//因为buddy不会直接被用户使用,所以这里分配直接按need_order分配[0,13)
		void* allocate(const int need_order);

		void deallocate(size_type block_offset, size_type block_order, size_type unit_num);
	
		size_type get_offset(char* target);
		
		static buddy_system& instance()
		{
			static buddy_system pool;
			return pool;
		}
	private:
		void initBitMap();

		void initFreeList();

		void initBuff();
		void initOffsetArray();
	private:
		void list_erase(buddy_node*& start, const size_type target);

	private:
		char*						buff;
		boost::dynamic_bitset<>		bitset_map[max_order];
		buddy_node*					free_list[max_order];		
	};
	
}
