#include "ztl_buddy_memory_allocator.h"
#include "ztl_tiny_allocator.h"
#include "memory_algorithm.h"
#include <new>
#include <Windows.h>
#include "node_allocator.h"
#include <iostream>
namespace ztl
{
	buddy_system::buddy_system()
	{
		initBuff();
		initFreeList();
		initBitMap();
	}
	//因为buddy不会直接被用户使用,所以这里分配直接按need_order分配[0,13)
	 void* buddy_system::allocate(const int need_order)
	{
		if(free_list[need_order] != nullptr)
		{
			auto old_offset = free_list[need_order]->current_offset;
			free_list[need_order] = free_list[need_order]->next;
			return buff + (old_offset << unit_order);
		}
		auto get_order = need_order;
		while(get_order < max_order&&free_list[get_order] == nullptr)
		{
			get_order++;
		}
		if(get_order == max_order)
		{
			return nullptr;
		}
		auto old_offset = free_list[get_order]->current_offset;
		free_list[get_order] = free_list[get_order]->next;
		for(int i = get_order - 1; i >= need_order; i--)
		{
			free_list[i] = ztl::node_allocator<buddy_node>::instance().allocate_node();  //
			free_list[i]->current_offset = old_offset;
			free_list[i]->next = 0;
			//	std::cout << "旧块" << old_offset << "被分为" << old_offset << "和" << new_offset<<std::endl;
			//	std::cout << old_offset << "放到链表上,并且位图第" << i << "的" << (old_offset >> (1 + i)) << "位与1异或" << std::endl;
			bitset_map[i + 1][old_offset >> (1 + i)] ^= 1;
			old_offset = old_offset + (1<<i);
		}
		return buff + (old_offset << unit_order);
	}
	 inline void buddy_system::deallocate(size_t block_offset, size_t block_order, size_t unit_num)
	{
		while(true)
		{
			auto debug = block_offset >> (1 + block_order);
			if(block_order == max_order - 1)
			{
				buddy_node* new_buff = (buddy_node*)(buff + block_offset*unit_size);
				construct(new_buff, buddy_node(block_offset, free_list[block_order]));
				free_list[block_order] = new_buff;
				break;
			}
			else if(bitset_map[block_order + 1][debug] == 1)
			{
				auto left_offset = 0;
				auto right_offset = 0;
				auto temp = (block_offset / unit_num);
				if((temp & 1) == 0)
				{
					//左节点
					left_offset = block_offset;
					right_offset = block_offset + unit_num;
					list_erase(free_list[block_order], right_offset);
				}
				else
				{
					//右节点
					left_offset = block_offset - unit_num;
					right_offset = block_offset;
					list_erase(free_list[block_order], left_offset);
				}
				bitset_map[block_order + 1][block_offset >> (1 + block_order)] ^= 1;
				block_offset = left_offset;
				unit_num = unit_num << 1;
				block_order += 1;
			}
			else
			{
				bitset_map[block_order + 1][block_offset >> (1 + block_order)] ^= 1;

				buddy_node* new_buff = (buddy_node*)(buff + block_offset*unit_size);
				construct(new_buff, buddy_node(block_offset, free_list[block_order]));
				free_list[block_order] = new_buff;
				break;
			}
		}
	}
	inline size_t buddy_system::get_offset(char* target)
	{
		return (target - buff) / unit_size;
	}

	inline void buddy_system::initBitMap()
	{
		for(auto i = 0; i < max_order; i++)
		{
			bitset_map[i].resize(1 << i);
		}
		ztl::reverse(bitset_map, bitset_map + max_order);
	}
	inline void buddy_system::initFreeList()
	{
		memset(free_list, 0, max_order*sizeof(buddy_node*));
		ztl::construct((buddy_node*)buff, buddy_node(0, 0));
		free_list[max_order - 1] = (buddy_node*)buff;
	}

	inline void* aligned_malloc(size_t size, size_t alignment)
	{
		// 检查alignment是否是2^N
		assert(!(alignment & (alignment - 1)));
		// 计算出一个最大的offset，sizeof(void*)是为了存储原始指针地址
		size_t offset = sizeof(void*)+(--alignment);

		// 分配一块带offset的内存
		char* p = static_cast<char*>(malloc(offset + size));
		if(!p) return nullptr;

		// 通过“& (~alignment)”把多计算的offset减掉
		void* r = reinterpret_cast<void*>(reinterpret_cast<size_t>(p + offset) & (~alignment));
		// 将r当做一个指向void*的指针，在r当前地址前面放入原始地址
		static_cast<void**>(r)[-1] = p;
		// 返回经过对齐的内存地址
		return r;
	}
	inline void buddy_system::initBuff()
	{
		buff = (char*)VirtualAlloc(NULL, max_unit_number*unit_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	}

	void buddy_system::list_erase(buddy_node*& start, const size_t target)
	{
		buddy_node* pre = nullptr;
		while(start != nullptr)
		{
			if(start->current_offset == target)
			{
				if(pre == nullptr)
				{
					start = start->next;
				}
				else
				{
					pre->next = start->next;
				}
				return;
			}
			pre = start;
			start = start->next;
		}
	}
}