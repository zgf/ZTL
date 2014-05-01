#pragma once
#include <vector>
#include <unordered_map>
namespace ztl
{
	namespace memory
	{
		namespace allocator
		{
			class MemoryPool
			{

			};
			class Chunk
			{
			
			public:
				Chunk() = delete;
				Chunk(size_t blockSzie, unsigned char tnumBlocks) :numBlocks(tnumBlocks) ,firstBlockIndex (0)
				{
					ChunkPtr.blockPtr = static_cast<unsigned char*>(operator new[](numBlocks * blockSzie));
					slistNode listPtr = ChunkPtr;
					for(unsigned char i = 0; i < numBlocks; listPtr.blockPtr += blockSzie)
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
				void deallocate(void* p,size_t blockSize)
				{
					slistNode pNode;
					pNode.blockPtr = static_cast<unsigned char*>(p);
					pNode.listIndex = firstBlockIndex;
					firstBlockIndex = (pNode.blockPtr - ChunkPtr.blockPtr) / blockSize;
					++numBlocks;
				}
		
				union slistNode
				{
					unsigned char* blockPtr;
					unsigned char listIndex;
				};
				slistNode ChunkPtr;
				//��һ�����Ա�ʹ�õ�block����
				unsigned char firstBlockIndex;
				//���ÿ��������255
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
				FixObjectAllocator()=delete;
				
				FixObjectAllocator(const size_t tblockSize,const unsigned char tnumBlocks) :blockSize(tblockSize), numBlocks(tnumBlocks)
				{
					lastAllocIndex = -1;
					lastDeallocIndex = -1;
					emptyBlockIndex = -1;
				}
				void* allocate()
				{
					if(lastAllocIndex == -1 || chunkList[lastAllocIndex]->numBlocks == 0)
					{
						if (emptyBlockIndex != -1)
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
					if(lastDeallocIndex == -1 || !InThisChunk(p,lastDeallocIndex))
					{
						for(size_t i = 0; i < chunkList.size();i++)
						{
							auto endIndex = chunkList.size() - 1;
							if (IsEmptyChunk(i))
							{
								
								if (emptyBlockIndex == -1)
								{
									//���ֵ�block�ǵ�һ���տ�
									emptyBlockIndex = i;
								}//���ֵ�block�ǵڶ����տ�
								//����Ҫ����ǲ���emptyblock�����һ��Ԫ��
								else if(emptyBlockIndex == endIndex)
								{
									chunkList.pop_back();
									emptyBlockIndex = i;
								}
								else
								{
									std::swap(chunkList[i], chunkList.back());
									//����Ҫ����ǲ���lastalloc��lastdelloc�����һ��Ԫ��
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
								std::swap(chunkList[i], chunkList.back());
								//����Ҫ����ǲ���lastalloc��lastdelloc�����һ��Ԫ��
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
					return p > chunkList[index]->ChunkPtr.blockPtr && p < chunkList[index]->ChunkPtr.blockPtr + blockSize * numBlocks;
				}
				bool IsEmptyChunk(const size_t Index)
				{
					return chunkList[Index]->numBlocks == numBlocks;
				}
				
			private:
				//Chunk�Ŀ��С
				const size_t blockSize;
				const unsigned char numBlocks;
				std::vector<Chunk*> chunkList;
				//���һ�η����Chunk��ַ
				int lastAllocIndex;
				//����һ�ι黹��Chunk��ַ
				int lastDeallocIndex;
				//������ָ��
				int emptyBlockIndex;
			};
			class allocator_base
			{
			public:
				
				allocator_base(const size_t tChunkSize = 4096, const size_t tMaxObjectSize=128) :chunkSize(tChunkSize), maxObjectSize(tMaxObjectSize)
				{
					FixObjectMap = new FixObjectAllocator*[tMaxObjectSize];
					memset(FixObjectMap, 0, tMaxObjectSize*4);

				//	FixObjectMap.reserve(maxObjectSize);
				}
				unsigned char BlockCount(const size_t nByte)
				{
					size_t result = chunkSize / nByte;
					return result > 255 ? 255 : result;
				}
				void* allocate(const size_t nByte)
				{
					if (nByte > maxObjectSize)
					{
						return operator new(nByte);
					}
					else
					{

						/*auto& findIter = FixObjectMap.find(nByte);
						if (findIter == FixObjectMap.end())
						{

							FixObjectMap.insert(std::make_pair(nByte, std::move(FixObjectAllocator(nByte, BlockCount(nByte)))));
						}
						findIter = FixObjectMap.find(nByte);
						return findIter->second.allocate();*/
						if(FixObjectMap[nByte]==nullptr)
						{
							
							FixObjectMap[nByte] = new FixObjectAllocator(nByte, BlockCount(nByte));
						}
						return FixObjectMap[nByte]->allocate();
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
						/*auto& findIter = FixObjectMap.find(nByte);
						findIter->second.deallocate(p);*/
					}
				}
			private:
				//chunk�������Ĵ�С,������block������
				const size_t chunkSize;
				//С�����С����
				const size_t maxObjectSize;
				//key��blockSize
				//std::unordered_map<size_t, FixObjectAllocator, _hasher<size_t>> FixObjectMap;
				FixObjectAllocator** FixObjectMap;
			};
		}
	}
}
