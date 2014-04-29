#pragma once
#include <vector>
#include <unordered_map>
namespace ztl
{
	namespace memory
	{
		namespace allocator
		{

			class Chunk
			{
			
			public:
				Chunk() = delete;
				Chunk(size_t blockSzie,size_t tnumBlocks)
				{
					ChunkPtr.blockPtr = static_cast<size_t*>(operator new(tnumBlocks * blockSzie));
					firstBlockIndex = 0;
					numBlocks = tnumBlocks;
				//	std::cout << (int)numBlocks << std::endl;
					auto listPtr = ChunkPtr;
					for(size_t i = 0; i < numBlocks; listPtr.blockPtr += blockSzie)
					{
						listPtr.listIndex = ++i;
					}
				}
				~Chunk()
				{
					//operator delete(ChunkPtr.blockPtr);
				}
				void* allocate(size_t blockSzie)
				{
					if (ChunkPtr.blockPtr == nullptr)
					{
						return nullptr;
					}
					auto ReutrnPtr = ChunkPtr.blockPtr;
					ChunkPtr.blockPtr += blockSzie * firstBlockIndex;
					firstBlockIndex = ChunkPtr.listIndex;
					--numBlocks;
				//	std::cout << (int)numBlocks<<std::endl;
					return ReutrnPtr;
				}
				void deallocate(void* p,size_t BlockSize)
				{
					slistNode pNode;
					pNode.blockPtr = static_cast<size_t*>(p);
					pNode.listIndex = firstBlockIndex;
					firstBlockIndex = (pNode.blockPtr - ChunkPtr.blockPtr) / BlockSize;
					++numBlocks;
				}
		
				union slistNode
				{
					size_t* blockPtr;
					size_t listIndex;
				};
				slistNode ChunkPtr;
				//第一个可以被使用的block索引
				size_t firstBlockIndex;
				//可用块数量最多255
				size_t numBlocks;
			};
			class FixObjectAllocator
			{
			
			public:
				FixObjectAllocator()=delete;
				
				FixObjectAllocator(const size_t tblockSize,const size_t tnumBlocks) :blockSize(tblockSize), numBlocks(tnumBlocks)
				{
					lastAllocIndex = -1;
					lastDeallocIndex = -1;
					emptyBlockIndex = -1;
				}
				void* allocate()
				{
					if(lastAllocIndex == -1 || chunkList[lastAllocIndex].numBlocks == 0)
					{
						if (emptyBlockIndex != -1)
						{
							lastAllocIndex = emptyBlockIndex;
							emptyBlockIndex = -1;
						}
						else
						{
							
							for(int i = chunkList.size() - 1;; i--)
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
							}
						}
					}
					return chunkList[lastAllocIndex].allocate(blockSize);
				}
				
				void deallocate(void* p)
				{
					if(lastDeallocIndex == -1 || !InThisChunk(p,lastDeallocIndex))
					{
						for(size_t i = 0; i < chunkList.size();i++)
						{
							if (IsEmptyChunk(i))
							{
								auto endIndex = chunkList.size() - 1;
								if (emptyBlockIndex == -1)
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
							if (InThisChunk(p,i))
							{
								lastDeallocIndex = i;
								break;
							}
						}
					}
					chunkList[lastDeallocIndex].deallocate(p, blockSize);
				}
				const size_t& GetBlockSize()const
				{
					return blockSize;
				}
			private:
				bool InThisChunk(const void* p, const int index)const
				{
					return p > chunkList[index].ChunkPtr.blockPtr && p < chunkList[index].ChunkPtr.blockPtr + blockSize * numBlocks;
				}
				bool IsEmptyChunk(const size_t Index)
				{
					return chunkList[Index].numBlocks == numBlocks;
				}
				
			private:
				//Chunk的块大小
				const size_t blockSize;
				const size_t numBlocks;
				std::vector<Chunk> chunkList;
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
				
				allocator_base(const size_t tChunkSize = 4096, const size_t tMaxObjectSize=128) :chunkSize(tChunkSize), maxObjectSize(tMaxObjectSize)
				{
					
				}
				void* allocate(const size_t nByte)
				{
					if (nByte > maxObjectSize)
					{
						return operator new(nByte);
					}
					else
					{
						auto& findIter = FixObjectMap.find(nByte);
						if (findIter == FixObjectMap.end())
						{
							FixObjectMap.insert(std::make_pair(nByte, std::move(FixObjectAllocator(nByte, chunkSize/nByte))));
						}
						findIter = FixObjectMap.find(nByte);
						return findIter->second.allocate();
					}
				}
				void deallocate(void* p,const size_t nByte)
				{
					if (nByte > maxObjectSize)
					{
						operator delete(p);
					}
					else
					{
						auto& findIter = FixObjectMap.find(nByte);
						findIter->second.deallocate(p);
					}
				}
			private:
				//chunk块整个的大小,决定了block的数量
				const size_t chunkSize;
				//小对象大小上限
				const size_t maxObjectSize;
				//key是blockSize
				std::unordered_map<size_t,FixObjectAllocator> FixObjectMap;
			};
		}
	}
}