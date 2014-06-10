#pragma once
#include <vector>
#include <unordered_map>
namespace ztl
{
	/*class MemoryPage
	{
	};
	class MemoryPool
	{
	std::vector<MemoryPage*> Pool;
	};*/
	class Chunk
	{
	public:
		Chunk() = delete;
		Chunk(size_t blockSzie, unsigned char tnumBlocks) :numBlocks(tnumBlocks), firstBlockIndex(0)
		{
			ChunkPtr = static_cast<unsigned char*>(operator new[](numBlocks * blockSzie));
			auto listPtr = ChunkPtr;
			for(unsigned char i = 0; i < numBlocks; listPtr += blockSzie)
			{
				*listPtr = ++i;
			}
		}
		~Chunk()
		{
			operator delete[](ChunkPtr);
		}
		void* allocate(size_t blockSzie)
		{
			if(numBlocks == 167)
			{
				int a = 0;
			}
			if(ChunkPtr == nullptr)
			{
				return nullptr;
			}

			auto ReutrnPtr = ChunkPtr + blockSzie * firstBlockIndex;
			firstBlockIndex = *ReutrnPtr;
			--numBlocks;
			//	std::cout << (int)numBlocks<<std::endl;

			return ReutrnPtr;
		}
		void deallocate(void* p, size_t blockSize)
		{
			auto blockPtr = static_cast<unsigned char*>(p);
			*blockPtr = firstBlockIndex;
			firstBlockIndex = (blockPtr - ChunkPtr) / blockSize;
			++numBlocks;
		}

		unsigned char* ChunkPtr;
		//第一个可以被使用的block索引
		unsigned char firstBlockIndex;
		//可用块数量最多255
		unsigned char numBlocks;
	};
	template<typename key>
	struct _hasher
	{
		size_t operator()(const key& Key)
		{
			return Key;
		}
	};
	class FixObjectAllocator
	{
	public:
		FixObjectAllocator() = delete;
		~FixObjectAllocator()
		{
			for(auto& Iter : chunkList)
			{
				delete Iter;
			}
		}
		FixObjectAllocator(const size_t tblockSize, const unsigned char tnumBlocks) :blockSize(tblockSize), numBlocks(tnumBlocks)
		{
			lastAllocIndex = -1;
			lastDeallocIndex = -1;
			emptyBlockIndex = -1;
		}
		void* allocate()
		{
			if(lastAllocIndex == -1 || chunkList[lastAllocIndex]->numBlocks == 0)
			{
				if(emptyBlockIndex != -1)
				{
					lastAllocIndex = emptyBlockIndex;
					emptyBlockIndex = -1;
				}
				else
				{
					int endIndex = (chunkList.size() - 1);
					if(endIndex == -1 || chunkList[endIndex]->numBlocks == 0)
					{
						chunkList.push_back(new Chunk(blockSize, numBlocks));
						lastAllocIndex = chunkList.size() - 1;
					}
					else
					{
						lastAllocIndex = endIndex;
					}
					/*for(int i = chunkList.size() - 1;; i--)
					{
					if(i == -1)
					{
					chunkList.push_back(Chunk(blockSize, numBlocks));
					lastAllocIndex = chunkList.size() - 1;
					break;
					}
					auto& Iter = chunkList[i];
					if(Iter.numBlocks != 0)
					{
					lastAllocIndex = i;
					break;
					}
					}*/
				}
			}
			return chunkList[lastAllocIndex]->allocate(blockSize);
		}

		void deallocate(void* p)
		{
			if(lastDeallocIndex == -1 || !InThisChunk(p, lastDeallocIndex))
			{
				for(size_t i = 0; i < chunkList.size(); i++)
				{
					auto endIndex = chunkList.size() - 1;
					if(IsEmptyChunk(i))
					{
						if(emptyBlockIndex == -1)
						{
							//发现的block是第一个空块
							emptyBlockIndex = i;
						}//发现的block是第二个空块
						//这里要检查是不是emptyblock是最后一个元素
						else if(emptyBlockIndex == endIndex)
						{
							chunkList.pop_back();
							emptyBlockIndex = i;
						}
						else
						{
							std::swap(chunkList[i], chunkList.back());
							//这里要检查是不是lastalloc和lastdelloc是最后一个元素
							if(lastAllocIndex == endIndex)
							{
								lastAllocIndex = i;
							}
							if(lastDeallocIndex == endIndex)
							{
								lastDeallocIndex = i;
							}
							chunkList.pop_back();
						}
					}
					if(InThisChunk(p, i))
					{
						std::swap(chunkList[i], chunkList.back());
						//这里要检查是不是lastalloc和lastdelloc是最后一个元素
						if(lastAllocIndex == endIndex)
						{
							lastAllocIndex = i;
						}
						if(lastDeallocIndex == endIndex)
						{
							lastDeallocIndex = i;
						}
						lastDeallocIndex = i;
						break;
					}
				}
			}
			chunkList[lastDeallocIndex]->deallocate(p, blockSize);
		}
		const size_t& GetBlockSize()const
		{
			return blockSize;
		}
	private:
		bool InThisChunk(const void* p, const int index)const
		{
			return p > chunkList[index]->ChunkPtr && p < chunkList[index]->ChunkPtr + blockSize * numBlocks;
		}
		bool IsEmptyChunk(const size_t Index)
		{
			return chunkList[Index]->numBlocks == numBlocks;
		}

	private:
		//Chunk的块大小
		const size_t blockSize;
		const unsigned char numBlocks;
		std::vector<Chunk*> chunkList;
		//最近一次分配的Chunk地址
		int lastAllocIndex;
		//最新一次归还的Chunk地址
		int lastDeallocIndex;
		//空区块指针
		int emptyBlockIndex;
	};
	class allocator_base
	{
	public:

		allocator_base(const size_t tChunkSize = 4096, const size_t tMaxObjectSize = 128) :chunkSize(tChunkSize), maxObjectSize(tMaxObjectSize)
		{
			FixObjectMap = new FixObjectAllocator*[tMaxObjectSize];
			memset(FixObjectMap, 0, tMaxObjectSize * 4);

			//	FixObjectMap.reserve(maxObjectSize);
		}
		unsigned char BlockCount(const size_t nByte)
		{
			size_t result = chunkSize / nByte;
			return result > 255 ? 255 : result;
		}
		void* allocate(const size_t nByte)
		{
			if(nByte > maxObjectSize)
			{
				return operator new(nByte);
			}
			else
			{
				/*auto& findIter = FixObjectMap.find(nByte);
				if (findIter == FixObjectMap.end())
				{
				FixObjectMap.insert(std::make_pair(nByte, ztl::traits::type_traits::move(FixObjectAllocator(nByte, BlockCount(nByte)))));
				}
				findIter = FixObjectMap.find(nByte);
				return findIter->second.allocate();*/
				if(FixObjectMap[nByte] == nullptr)
				{
					FixObjectMap[nByte] = new FixObjectAllocator(nByte, BlockCount(nByte));
				}
				return FixObjectMap[nByte]->allocate();
			}
		}
		void deallocate(void* p, const size_t nByte)
		{
			if(nByte > maxObjectSize)
			{
				operator delete(p);
			}
			else
			{
				FixObjectMap[nByte]->deallocate(p);
				/*auto& findIter = FixObjectMap.find(nByte);
				findIter->second.deallocate(p);*/
			}
		}
		~allocator_base()
		{
			for(size_t i = 0; i < maxObjectSize; i++)
			{
				delete FixObjectMap[i];
			}
			delete FixObjectMap;
		}
	private:
		//chunk块整个的大小,决定了block的数量
		const size_t chunkSize;
		//小对象大小上限
		const size_t maxObjectSize;
		//key是blockSize
		//std::unordered_map<size_t, FixObjectAllocator, _hasher<size_t>> FixObjectMap;
		FixObjectAllocator** FixObjectMap;
	};
}
