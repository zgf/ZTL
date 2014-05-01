#pragma once
#include "Z_TypeTraits.h"
#include <new.h>
const int ALIGN = 8;
const int MAX_BYTE = 128;
const int ArrayNum = MAX_BYTE / ALIGN;
/************************************************************************/
/*
	要分配的内存大于128byte直接用class DefaultAlloc分配，小于128调用第二级内存配置器memory_alloc
	构造和析构用alloc这个类做
	*/
/************************************************************************/
class DefaultAlloc
{
public:
	typedef void(*Handler)( );
	static void* Allocate(size_t n);//内存分配
	static void DeAllocate(void* p);//回收内存
	static void SetNewHandler(Handler func);//设置new失败的处理函数
};
void* DefaultAlloc::Allocate(size_t n)
{
	return operator new( n );
}
void DefaultAlloc::DeAllocate(void* p)
{
	operator delete( p );
}
void DefaultAlloc::SetNewHandler(Handler func)
{
	set_new_handler(func);
}
/*
建立一个内存池，维护一个内存块链表
*/
struct Address
{
	Address* NextAddress;
};
class TinyAlloc
{
public:
	static void* Allocate(size_t NeedBtye);//从链表上分配内存
	static void* Refill(size_t NeedBtye);//从内存池挂载size_t大小区块到链表并返回第一个给客户使用
	static void* ChunkMemoryPool(size_t NeedBtye, size_t& BlockNum);//调整内存池大小，返回size_t*BlockNum区块的整块内存
	static void DeAllocate(void* p, size_t BlockSize);//将内存块重新挂载到链表上
private:
	static Address FreeList[ArrayNum];//内存链表
	static char* StartFree;//内存池开始地址
	static char* EndFree;//内存池结束地址
	static size_t HeapSize;//已使用的内存总量
	static size_t FreeListIndex(size_t NeedBtye);//获取所需内存块所在索引
	static size_t AdjustBtyeSizeUp(size_t NeedBtye);//上调所需区块大小为8倍数
	static size_t AdjustBtyeSizeDown(size_t GetBtye);//下调所需区块大小为8倍数
};
Address  TinyAlloc::FreeList[ArrayNum] = {nullptr};
char* TinyAlloc::StartFree = nullptr;
char* TinyAlloc::EndFree = nullptr;
size_t TinyAlloc::HeapSize = 0;

//向上调整区块大小为8的倍数
size_t TinyAlloc::AdjustBtyeSizeUp(size_t NeedBtye)
{
	return  ( NeedBtye + ALIGN - 1 )&~( ALIGN - 1 );
}
size_t TinyAlloc::AdjustBtyeSizeDown(size_t GetBtye)
{
	return ( GetBtye - ALIGN + 1 )& ~( ALIGN - 1 );
}
//获取所需内存块所在索引
size_t TinyAlloc::FreeListIndex(size_t NeedBtye)
{
	return NeedBtye / ALIGN - 1;
}
//从链表上获取所需大小的内存块
void* TinyAlloc::Allocate(size_t NeedBtye)
{
	NeedBtye = AdjustBtyeSizeUp(NeedBtye);
	size_t Index = FreeListIndex(NeedBtye);
	//如果指定位置的值为nullptr说明这个位置的链表还没有这个大小的挂载区块,需要refill
	if(FreeList[Index].NextAddress == nullptr)
	{
		return Refill(NeedBtye);
	}
	else
	{
		Address* ListBeginAddress = FreeList[Index].NextAddress;
		void* Result = ListBeginAddress;
		ListBeginAddress = ListBeginAddress->NextAddress;
		FreeList[Index].NextAddress = ListBeginAddress;
		return Result;
	}
}
void TinyAlloc::DeAllocate(void* p, size_t BlockSize)
{
	BlockSize = AdjustBtyeSizeUp(BlockSize);
	size_t Index = FreeListIndex(BlockSize);
	Address* NewListBeginAddress = static_cast<Address*>( p );
	NewListBeginAddress->NextAddress = FreeList[Index].NextAddress;
	FreeList[Index].NextAddress = NewListBeginAddress;
}
void* TinyAlloc::Refill(size_t NeedBtye)
{
	size_t Index = FreeListIndex(NeedBtye);
	//一次性挂载20个NeedBtye大小的内存块到链表上去
	size_t BlockNum = 20;
	//从内存池获取整块的所需大小的内存块
	void* Result = ChunkMemoryPool(NeedBtye, BlockNum);
	//设置从内存池已获取的内存总量
	HeapSize = HeapSize + BlockNum*NeedBtye;
	//获取的第1块满足大小的内存区块直接返回给客户端，获取了不止一块的内存块就串联起来挂载到链表上
	if(BlockNum == 1)
	{
		return Result;
	}
	else
	{
		void* ClientBlock = Result;
		Address* ListBeignAddress = (Address*)( static_cast<char*>(Result)+NeedBtye );//如果不这样，Address强制转换先和Result结合，+8就等于Result+8*sizeof(Address)
		//将Result地址上的区块串联起来
		--BlockNum;
		for(; BlockNum > 1; --BlockNum)
		{
			ListBeignAddress->NextAddress = (Address*)( (char*)ListBeignAddress + NeedBtye );
			ListBeignAddress = ListBeignAddress->NextAddress;
		}
		ListBeignAddress->NextAddress = nullptr;
		FreeList[Index].NextAddress = (Address*)( static_cast<char*>(Result)+NeedBtye );
		return ClientBlock;
	}
}
//根据需要获取的区块内存总量调整内存池
void* TinyAlloc::ChunkMemoryPool(size_t NeedBtye, size_t& BlockNum)
{
	//所需的内存总量
	size_t TotalNeedByte = NeedBtye * BlockNum;
	//内存池剩余内存总量
	size_t RestMemorySize = EndFree - StartFree;
	size_t Index = FreeListIndex(NeedBtye);
	if(RestMemorySize > TotalNeedByte)
	{
		//剩余内存总量大于所需内存总量
		void* Result = StartFree;
		StartFree = StartFree + TotalNeedByte;
		return Result;
	}
	else if(RestMemorySize > NeedBtye)
	{
		//剩余内存总量不足，但是足够供应一个以上的NeedByte大小的内存区块，
		//这里笔误RestMemorySize 写成了TotalNeedByte，导致堆栈损坏，找了我一小时多= = 大坑
		BlockNum = RestMemorySize / NeedBtye;
		void* Result = StartFree;
		StartFree = StartFree + BlockNum*NeedBtye;
		return Result;
	}
	else
	{
		//剩余内存总量连一个NeedByte大小的内存区块都没法供应了
		if(RestMemorySize > 0)
		{
			//如果内存池还有剩余，挂载到合适的Freelist中，因为准备换新的内存池了,尽量减少内存碎片
			//将内存池剩余容量下调到8的倍数
			int GetBtye = AdjustBtyeSizeDown(RestMemorySize);
			if(GetBtye >= 8)
			{
				size_t GetIndex = FreeListIndex(GetBtye);
				Address* ListBeginAddress = (Address*)StartFree;
				StartFree = StartFree + GetBtye;
				ListBeginAddress->NextAddress = FreeList[GetIndex].NextAddress;
				FreeList[GetIndex].NextAddress = ListBeginAddress;
			}
			//将剩余的内存释放
			int DeleteMemorySize = RestMemorySize - GetBtye;
			DeAllocate(StartFree, DeleteMemorySize);
		}
		//每次内存池大小都是8的倍数,并且随着使用内存总量越来越大，补充的内存也越来越大>>4 = 除以8
		size_t AddMemoryByte = TotalNeedByte * 2 + ( HeapSize >> 4 );
		StartFree = ( char* )operator new( AddMemoryByte );
		if(StartFree == nullptr)
		{
			//说明Heap空间耗尽,看看是否链表上还有已挂载的还未分配的足够大的空内存,有就卸载链表上的区块补充内存池
			for(size_t i = Index; i < ArrayNum; i++)
			{
				if(FreeList[i].NextAddress != nullptr)
				{
					//说明有可用内存
					StartFree = (char*)FreeList[i].NextAddress;
					Address* ListBegin = FreeList[i].NextAddress;
					FreeList[i].NextAddress = nullptr;
					while(ListBegin->NextAddress != nullptr)
					{
						ListBegin = ListBegin->NextAddress;
					}
					EndFree = (char*)ListBegin;
					BlockNum = ( EndFree - StartFree ) / NeedBtye;
					return ChunkMemoryPool(NeedBtye, BlockNum);
				}
			}
			//说明链表上也没有内存了，调用new-handler机制也许还有办法解决问题
			StartFree = ( char* )operator new( AddMemoryByte );
		}
		EndFree = StartFree + AddMemoryByte;
		return ChunkMemoryPool(NeedBtye, BlockNum);
	}
}
template<typename NodeType>
class Alloc
{
public:
	static NodeType* Allocate(size_t Byte)
	{
		//size_t Byte = sizeof(NodeType) * Num;
		if(Byte > 128)
		{
			return static_cast<NodeType*>( DefaultAlloc::Allocate(Byte) );
		}
		else
		{
			return static_cast<NodeType*>( TinyAlloc::Allocate(Byte) );
		}
	}
	static void DeAllocate(void* p, size_t Byte)
	{
		//	size_t Byte = sizeof(NodeType) * Num;
		if(Byte > 128)
		{
			DefaultAlloc::DeAllocate(p);
		}
		else
		{
			TinyAlloc::DeAllocate(p, Byte);
		}
	}

private:
	//	static DefaultAlloc FirstAlloc;
	//static TinyAlloc SecondAlloc;
};
//区间内的对象构造

template<typename InputIterator, typename ValueType>
void IntervalConstructor(InputIterator StartPointer, InputIterator EndPointer, ValueType Val = ValueType())
{
	IntervalConstructor_aux(StartPointer, EndPointer, Val, __type_traits<IteratorTraits<InputIterator>::ValueType>::is_POD_type());
}
template<typename InputIterator, typename ValueType>
InputIterator IntervalConstructor(InputIterator StartPointer, size_t Num, ValueType Val = ValueType())
{
	InputIterator EndPointer = StartPointer + Num;
	//for(int i = 0;i <= Num;++EndPointer,++Num)
	//{
	// //太低效率了，待重构
	// Constructor(EndPointer,IteratorTraits<InputIterator>::ValueType());
	//}
	IntervalConstructor_aux(StartPointer, EndPointer, Val, __type_traits<IteratorTraits<InputIterator>::ValueType>::is_POD_type());
	return EndPointer;
}
template<typename InputIterator, typename ValueType>
void IntervalConstructor_aux(InputIterator StartPointer, InputIterator EndPointer, ValueType Val, __true_type)
{
	// InputIterator EndPointer = StartPointer + Num;
	//InputIterator ItWork = StartPointer;
	//IteratorTraits<InputIterator>::ValueType Val = IteratorTraits<InputIterator>::ValueType();
	for(; StartPointer != EndPointer; ++StartPointer)
	{
		*StartPointer = Val;
	}
}
template<typename InputIterator, typename ValueType>
void IntervalConstructor_aux(InputIterator StartPointer, InputIterator EndPointer, ValueType Val, __false_type)
{
	//InputIterator EndPointer = StartPointer + Num;
	// InputIterator ItWork = StartPointer;
	// auto Val = IteratorTraits<InputIterator>::ValueType();
	for(; StartPointer != EndPointer; ++StartPointer)
	{
		Constructor(StartPointer, Val);
	}
}
template<typename T1, typename T2>
void Constructor(T1* pointer, const T2& Value)
{
	new (pointer)T1(Value);
}
template<typename T1>
void Destructor(T1* pointer)
{
	Destructor(pointer, __type_traits<T1>::is_POD_type());
}
template<typename T1>
void Destructor(T1* pointer, __true_type)
{
	//POD类型什么都不做
	//pointer->~T1();
}
template<typename T1>
void Destructor(T1* pointer, __false_type)
{
	pointer->~T1();
}
//对POD类型和非POD类型做不同处理
template<typename InputIterator>
void Destory(InputIterator StartAddress, InputIterator EndAddress, __true_type)
{
	//POD类型什么也不用做
}
template<typename InputIterator>
void Destory(InputIterator StartAddress, InputIterator EndAddress, __false_type)
{
	//非POD类型要调用析构函数
	for(InputIterator Iter = StartAddress; Iter != EndAddress; ++Iter)
	{
		typedef IteratorTraits<InputIterator>::ValueType ValueType;
		Iter->~ValueType();
	}
}
//析构整个区间
template<typename InputIterator>
void Destory(InputIterator StartAddress, InputIterator EndAddress)
{
	Destory(StartAddress, EndAddress, __type_traits<InputIterator>::is_POD_type());
}

//整个区间拷贝
template<typename InputIterator, typename NewInputIterator>
NewInputIterator Copy(InputIterator StartAddress, InputIterator EndAddress, NewInputIterator NewStartAddress, __true_type)
{
	memcpy(NewStartAddress, StartAddress, ( EndAddress - StartAddress )*sizeof( IteratorTraits<InputIterator>::ValueType ));

	return NewStartAddress + ( EndAddress - StartAddress );
}

template<typename InputIterator, typename NewInputIterator>
NewInputIterator Copy(InputIterator StartAddress, InputIterator EndAddress, NewInputIterator NewStartAddress, __false_type)
{
	//析构是operator= 函数判定的事情，是否new资源转移的时候释放原有资源在接受新资源

	/*InputIterator Iter = StartAddress;
	NewInputIterator NewIter = NewStartAddress;*/
	for(; StartAddress != EndAddress; ++StartAddress, ++NewStartAddress)
	{
		*NewStartAddress = *StartAddress;
	}
	return NewStartAddress;
}
//特化版char*
template<>
char* Copy<char*>(char* StartAddress, char* EndAddress, char* NewStartAddress, __true_type)
{
	if(StartAddress == nullptr || EndAddress == nullptr)
	{
		NewStartAddress = nullptr;
		return nullptr;
	}
	//std::memmove(NewStartAddress, StartAddress, EndAddress - StartAddress);
	return NewStartAddress + ( EndAddress - StartAddress );
}
//需要判定是否是POD类型

template<typename InputIterator, typename NewInputIterator>
NewInputIterator Copy(InputIterator StartAddress, InputIterator EndAddress, NewInputIterator NewStartAddress)
{
	return  Copy(StartAddress, EndAddress, NewStartAddress, __type_traits<InputIterator>::is_POD_type());
}

// 大量使用static是为了不实例化就调用函数，提高效率,并且多个容器在同一个内存池上分配
//打印全部
template<typename InputIterator>
void PrintAllElemt(InputIterator StartAddress, InputIterator EndAddress)
{
	for(InputIterator Iter = StartAddress; Iter != EndAddress; ++Iter)
	{
		std::cout << *Iter;
		std::cout << " ";
	}
	std::cout << std::endl;
}
////区间内填充重复元素函数
//template<typename InputIterator,typename T>
//void FillRepeatElement(Inpu［ｐ［tIterator StartAddress,InputIterator EndAddress,const T& Value)
//{
// //PrintAllElemt(StartAddress,EndAddress);
// //cout<<endl;
//InputIterator Iter = StartAddress;
//for (;Iter != EndAddress;++Iter)
//{
//	Constructor(Iter,Value);
//}
//}
//////////////////确定Copy（地址无重叠） CopyBack（地址重叠，向高位地址拷贝，用于插入） CopyFront（地址重叠，向低位拷贝，用于删除）三种全局函数
//重构随机迭代器为Vector友元类

//CopyBack（地址重叠，向高位地址拷贝，用于插入）返回值为目的地的起始元素位置
template<typename InputIterator, typename NewInputIterator>
NewInputIterator CopyBack(NewInputIterator DesEndPoint, InputIterator SrcStartPoint, InputIterator SrcEndPoint)
{
	return CopyBack_aux(DesEndPoint, SrcStartPoint, SrcEndPoint, __type_traits< IteratorTraits<InputIterator>::ValueType>::is_POD_type());
}
//POD类型，直接拷贝
template<typename InputIterator, typename NewInputIterator>
NewInputIterator CopyBack_aux(NewInputIterator DesEndPoint, InputIterator SrcStartPoint, InputIterator SrcEndPoint, __true_type)
{
	//EndPoint 是超尾
	// 	 int num = SrcEndPoint - SrcStartPoint;
	// 	 auto DesStartPoint = DesEndPoint - num;
	// 	 memmove(DesStartPoint,SrcStartPoint,num*sizeof(IteratorTraits<InputIterator>::ValueType));
	// 	 return DesStartPoint;
	int n = SrcEndPoint - SrcStartPoint;
	auto DesStartPoint = DesEndPoint - n;

	//PrintAllElemt(SrcStartPoint,SrcEndPoint);
	//PrintAllElemt(DesStartPoint,DesEndPoint);
	--SrcEndPoint;
	--DesEndPoint;
	//auto copyEnd = DesEndPoint;
	//n = 10;
	for(; n > 0; --n, --SrcEndPoint, --DesEndPoint)
	{
		//Constructor(&*DesEndPoint,*SrcEndPoint);
		*DesEndPoint = *SrcEndPoint;
	}
	return DesStartPoint;
}
template<typename InputIterator, typename NewInputIterator>
NewInputIterator CopyBack_aux(NewInputIterator DesEndPoint, InputIterator SrcStartPoint, InputIterator SrcEndPoint, __false_type)
{
	//EndPoint是超尾
	//非POD类型，调用operator=来拷贝，防止浅拷贝后，指向同一堆地址的对象被析构两次，具体的深浅拷贝由operator=决定，析构由析构函数在Copy外事先完成
	//先在目的地构造,因为stringb不符合SGI的__type__traits规范，所以只能当做非POD类型处理，不能调用memcpy，如果是C++11，可以使用内置的type__traits
	//这样判断POD类型后直接使用memcpy复制搞定，so easy 微软的这里比我快应该是因为我这里有构造函数开销，它应该是调用了编译器扩展的类型判断使用了更快的拷贝，么有调用构造函数
	/***
	坐等支持C++11的VS IDE重构这里的代码
	···或者，我来string也重造一个
	***/

	int n = SrcEndPoint - SrcStartPoint;
	auto DesStartPoint = DesEndPoint - n;

	//PrintAllElemt(SrcStartPoint,SrcEndPoint);
	//PrintAllElemt(DesStartPoint,DesEndPoint);
	--SrcEndPoint;
	--DesEndPoint;
	//auto copyEnd = DesEndPoint;
	//n = 10;
	for(; n > 0; --n, --SrcEndPoint, --DesEndPoint)
	{
		Constructor(&*DesEndPoint, *SrcEndPoint);
		//*(DesEndPoint)  = *SrcEndPoint;
	}
	// 	vector<int>a(11);
	// 	vector<int*>address(11);
	// 	for (int i = 0;i <11;i++)
	// 	{
	// 		a[i] = (*(copyEnd - i));
	// 		address[i] = &*(copyEnd-i);
	// 	}
	return DesStartPoint;
}
template<>
char* CopyBack_aux<char*>(char* DesEndPoint, char* SrcStartPoint, char* SrcEndPoint, __true_type)
{
	int num = SrcEndPoint - SrcStartPoint;
	auto DesStartPoint = DesEndPoint - num;
	//memmove(DesStartPoint, SrcStartPoint, ( SrcEndPoint - SrcStartPoint ));
	return DesStartPoint + ( SrcEndPoint - SrcStartPoint );
}
//CopyFront（地址重叠，向低位拷贝，用于删除）返回目的地最后一个元素后面位置
template<typename InputIterator, typename NewInputIterator>
NewInputIterator CopyFront(NewInputIterator DesStartPoint, InputIterator SrcStartPoint, InputIterator SrcEndPoint)
{
	return CopyFront_aux(DesStartPoint, SrcStartPoint, SrcEndPoint, __type_traits< IteratorTraits<InputIterator>::ValueType>::is_POD_type());
}
//POD类型，直接拷贝
template<typename InputIterator, typename NewInputIterator>
NewInputIterator CopyFront_aux(NewInputIterator DesStartPoint, InputIterator SrcStartPoint, InputIterator SrcEndPoint, __true_type)
{
	//memmove(DesStartPoint,SrcStartPoint,(SrcEndPoint - SrcStartPoint)*sizeof(IteratorTraits<InputIterator>::ValueType));
	int n = SrcEndPoint - SrcStartPoint;
	for(; n > 0; --n, ++SrcStartPoint, ++DesStartPoint)
	{
		//Constructor(DesStartPoint,*SrcStartPoint);
		*DesStartPoint = *SrcStartPoint;
	}
	return DesStartPoint;
	// return DesStartPoint + (SrcEndPoint - SrcStartPoint);
}
template<typename InputIterator, typename NewInputIterator>
NewInputIterator CopyFront_aux(NewInputIterator DesStartPoint, InputIterator SrcStartPoint, InputIterator SrcEndPoint, __false_type)
{
	//static_assert(,"Error!" );
	//非POD类型，调用operator=来拷贝，防止浅拷贝后，指向同一堆地址的对象被析构两次，具体的深浅拷贝由operator=决定，析构由析构函数在Copy外事先完成
	int n = SrcEndPoint - SrcStartPoint;
	for(; n > 0; --n, ++SrcStartPoint, ++DesStartPoint)
	{
		Constructor(&*DesStartPoint, *SrcStartPoint);
		//*DesStartPoint = *SrcStartPoint;
	}
	return DesStartPoint;
}
template<>
char* CopyFront_aux<char*>(char* DesStartPoint, char* SrcStartPoint, char* SrcEndPoint, __true_type)
{
	//memmove(DesStartPoint, SrcStartPoint, ( SrcEndPoint - SrcStartPoint ));
	return DesStartPoint + ( SrcEndPoint - SrcStartPoint );
}
template<typename InputIterator>
void Swap(InputIterator First, InputIterator Second)
{
	auto temp = *First;
	*First = *Second;
	*Second = temp;
}
//要求必须是连续内存,对于非连续内存这样会出BUG，坐等重构
//实际的Sort应该是插入，快排，堆排的混合排序算法
template<typename InputIterator>
void QuickSort(InputIterator StartIter, InputIterator EndIter)
{
	InputIterator LowIter = StartIter;
	InputIterator HeightIter = --EndIter;
	//数组第一个，数组最后一个
	QuickSort_aux(LowIter, HeightIter);
}
template<typename InputIterator>
void QuickSort_aux(InputIterator StartIter, InputIterator EndIter)
{
	if(&*StartIter >= &*EndIter)
	{
		return;
	}

	InputIterator LowIter = StartIter;
	//StartIter++;
	InputIterator HeighIter = EndIter;
	//EndIter++;
	InputIterator ChoseIter = LowIter;
	while(&*StartIter <= &*EndIter)
	{
		while(*StartIter <= *ChoseIter)
		{
			if(StartIter == HeighIter)
			{
				break;
			}
			++StartIter;
		}
		while(*EndIter >= *ChoseIter)
		{
			if(EndIter == LowIter)
			{
				break;
			}
			--EndIter;
		}
		if(&*EndIter <= &*StartIter)
		{
			break;
		}

		//PrintAllElemt(Begin(),End());
		Swap(StartIter, EndIter);
		//PrintAllElemt(Begin(),End());
	}
	//PrintAllElemt(Begin(),End());
	Swap(EndIter, ChoseIter);
	//	PrintAllElemt(Begin(),End());
	QuickSort_aux(LowIter, EndIter - 1);
	//	PrintAllElemt(Begin(),End());
	QuickSort_aux(EndIter + 1, HeighIter);
}