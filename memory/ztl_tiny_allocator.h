#pragma once
namespace ztl
{
	struct slab_node
	{
		char*		   slab_start;
		char*		   slab_end;
		size_t		   rest_node_count;//自由节点数量
		slab_node*     next;//下一个slab;
	};
	class tiny_allocator
	{
	public:
		const static int		array_size = 7;
		const static int		alis_size  = 8;
		typedef size_t			size_type;
		typedef tiny_allocator	self_type;
		typedef char*			node_type;
	public:
		tiny_allocator(size_type size = 16, size_type object_size = 512);

		~tiny_allocator();
		
	public:
		void* allocate(const size_type nBtye);
	
	
		void deallocate(void* p, size_type nByte);
		
	public:
		static self_type& tiny_allocator::instance()
		{
			static self_type tiny_object;
			return tiny_object;
		}
		
		
		void* GetNewChunk(const size_type unit_size);
		
	public:
		void initObjectSizeList();
		
		size_type GetObjectSizeListIndex(const size_type nByte);
	
		void initChunkList(node_type start_pointer, size_type block_size, const size_type list_count);
		
		void initFullList();
		void initPartialList();
	private:

		node_type			free_list[array_size];//当前使用的slab
		size_type			max_object_size;
		size_type			list_size;
		int					object_size_list[array_size];
		slab_node			full_list[array_size];//已经使用完的slab
		slab_node			partial_list[array_size];//还有部分节点未使用的slab
	};
}