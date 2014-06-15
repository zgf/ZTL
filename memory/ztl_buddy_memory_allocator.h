#pragma once
#include <windows.h>
#include <cmath>
#include "../algorithm/ztl_algorithm.h"
#include "boost/dynamic_bitset/dynamic_bitset.hpp"
#include "../type/complier_algorithm.h"
#include "memory_algorithm.h"
namespace ztl
{
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
		static const size_type page_number = 1024;
		static const size_type unit_size = 512;
		static const size_type max_unit_number = page_size*page_number / unit_size;
		static const size_type max_order = ztl::Log2<max_unit_number>::value + 1;
	public:
		buddy_system()
		{
			initBuff();
			initFreeList();
			initBitMap();
		}
	public:
		//因为buddy不会直接被用户使用,所以这里分配直接按need_order分配[0,13)
		void* allocate(const size_type need_order)
		{
			auto get_order = need_order;
			while(get_order < max_order&&free_list[get_order] == nullptr)
			{
				get_order++;
			}
			auto old_offset = free_list[get_order]->current_offset;
			free_list[get_order] = free_list[get_order]->next;
			auto new_offset = old_offset;
			for(auto i = get_order - 1; i >= need_order; i--)
			{
				new_offset = old_offset + (1 << i);
				//	std::cout << "旧块" << old_offset << "被分为" << old_offset << "和" << new_offset<<std::endl;
				auto new_buff = (buddy_node*)(buff + old_offset*unit_size);
				ztl::construct(new_buff, buddy_node(old_offset, 0));
				free_list[i] = new_buff;
				//	std::cout << old_offset << "放到链表上,并且位图第" << i << "的" << (old_offset >> (1 + i)) << "位与1异或" << std::endl;
				bitset_map[i + 1][old_offset >> (1 + i)] ^= 1;
				old_offset = new_offset;
			}
			//new_offset就是需要的块
			return buff + new_offset*unit_size;
		}
		void deallocate(size_type block_offset, size_type block_order, size_type unit_num)
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
					//const auto unit_num = 1 << block_order;
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
					bitset_map[block_order + 1][block_offset >> (1 + block_order)] ^=1;
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
		size_type get_offset(char* target)
		{
			return (target - buff) / unit_size;
		}
	private:
		void initBitMap()
		{
			for(auto i = 0; i < max_order; i++)
			{
				bitset_map[i].resize(1 << i);
			}
			ztl::reverse(bitset_map, bitset_map + max_order);
		}
		void initFreeList()
		{
			memset(free_list, 0, max_order*sizeof(buddy_node*));
			ztl::construct((buddy_node*)buff, buddy_node(0, 0));
			free_list[max_order - 1] = (buddy_node*)buff;
		}
		void initBuff()
		{
			buff = (char*)VirtualAlloc(NULL, max_unit_number*unit_size, MEM_COMMIT | MEM_RESERVE | MEM_TOP_DOWN, PAGE_READWRITE);
		}
	private:
		void list_erase(buddy_node*& start, const size_type target)
		{
			buddy_node* pre = nullptr;
			while(start != nullptr)
			{
				if(start->current_offset = target)
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
	private:
		char*						buff;
		boost::dynamic_bitset<>		bitset_map[max_order];
		buddy_node*					free_list[max_order];
	};
}
