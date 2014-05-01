#pragma once
#include "Z_TypeTraits.h"
#include <new.h>
const int ALIGN = 8;
const int MAX_BYTE = 128;
const int ArrayNum = MAX_BYTE / ALIGN;
/************************************************************************/
/*
	Ҫ������ڴ����128byteֱ����class DefaultAlloc���䣬С��128���õڶ����ڴ�������memory_alloc
	�����������alloc�������
	*/
/************************************************************************/
class DefaultAlloc
{
public:
	typedef void(*Handler)( );
	static void* Allocate(size_t n);//�ڴ����
	static void DeAllocate(void* p);//�����ڴ�
	static void SetNewHandler(Handler func);//����newʧ�ܵĴ�����
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
����һ���ڴ�أ�ά��һ���ڴ������
*/
struct Address
{
	Address* NextAddress;
};
class TinyAlloc
{
public:
	static void* Allocate(size_t NeedBtye);//�������Ϸ����ڴ�
	static void* Refill(size_t NeedBtye);//���ڴ�ع���size_t��С���鵽�������ص�һ�����ͻ�ʹ��
	static void* ChunkMemoryPool(size_t NeedBtye, size_t& BlockNum);//�����ڴ�ش�С������size_t*BlockNum����������ڴ�
	static void DeAllocate(void* p, size_t BlockSize);//���ڴ�����¹��ص�������
private:
	static Address FreeList[ArrayNum];//�ڴ�����
	static char* StartFree;//�ڴ�ؿ�ʼ��ַ
	static char* EndFree;//�ڴ�ؽ�����ַ
	static size_t HeapSize;//��ʹ�õ��ڴ�����
	static size_t FreeListIndex(size_t NeedBtye);//��ȡ�����ڴ����������
	static size_t AdjustBtyeSizeUp(size_t NeedBtye);//�ϵ����������СΪ8����
	static size_t AdjustBtyeSizeDown(size_t GetBtye);//�µ����������СΪ8����
};
Address  TinyAlloc::FreeList[ArrayNum] = {nullptr};
char* TinyAlloc::StartFree = nullptr;
char* TinyAlloc::EndFree = nullptr;
size_t TinyAlloc::HeapSize = 0;

//���ϵ��������СΪ8�ı���
size_t TinyAlloc::AdjustBtyeSizeUp(size_t NeedBtye)
{
	return  ( NeedBtye + ALIGN - 1 )&~( ALIGN - 1 );
}
size_t TinyAlloc::AdjustBtyeSizeDown(size_t GetBtye)
{
	return ( GetBtye - ALIGN + 1 )& ~( ALIGN - 1 );
}
//��ȡ�����ڴ����������
size_t TinyAlloc::FreeListIndex(size_t NeedBtye)
{
	return NeedBtye / ALIGN - 1;
}
//�������ϻ�ȡ�����С���ڴ��
void* TinyAlloc::Allocate(size_t NeedBtye)
{
	NeedBtye = AdjustBtyeSizeUp(NeedBtye);
	size_t Index = FreeListIndex(NeedBtye);
	//���ָ��λ�õ�ֵΪnullptr˵�����λ�õ�����û�������С�Ĺ�������,��Ҫrefill
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
	//һ���Թ���20��NeedBtye��С���ڴ�鵽������ȥ
	size_t BlockNum = 20;
	//���ڴ�ػ�ȡ����������С���ڴ��
	void* Result = ChunkMemoryPool(NeedBtye, BlockNum);
	//���ô��ڴ���ѻ�ȡ���ڴ�����
	HeapSize = HeapSize + BlockNum*NeedBtye;
	//��ȡ�ĵ�1�������С���ڴ�����ֱ�ӷ��ظ��ͻ��ˣ���ȡ�˲�ֹһ����ڴ��ʹ����������ص�������
	if(BlockNum == 1)
	{
		return Result;
	}
	else
	{
		void* ClientBlock = Result;
		Address* ListBeignAddress = (Address*)( static_cast<char*>(Result)+NeedBtye );//�����������Addressǿ��ת���Ⱥ�Result��ϣ�+8�͵���Result+8*sizeof(Address)
		//��Result��ַ�ϵ����鴮������
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
//������Ҫ��ȡ�������ڴ����������ڴ��
void* TinyAlloc::ChunkMemoryPool(size_t NeedBtye, size_t& BlockNum)
{
	//������ڴ�����
	size_t TotalNeedByte = NeedBtye * BlockNum;
	//�ڴ��ʣ���ڴ�����
	size_t RestMemorySize = EndFree - StartFree;
	size_t Index = FreeListIndex(NeedBtye);
	if(RestMemorySize > TotalNeedByte)
	{
		//ʣ���ڴ��������������ڴ�����
		void* Result = StartFree;
		StartFree = StartFree + TotalNeedByte;
		return Result;
	}
	else if(RestMemorySize > NeedBtye)
	{
		//ʣ���ڴ��������㣬�����㹻��Ӧһ�����ϵ�NeedByte��С���ڴ����飬
		//�������RestMemorySize д����TotalNeedByte�����¶�ջ�𻵣�������һСʱ��= = ���
		BlockNum = RestMemorySize / NeedBtye;
		void* Result = StartFree;
		StartFree = StartFree + BlockNum*NeedBtye;
		return Result;
	}
	else
	{
		//ʣ���ڴ�������һ��NeedByte��С���ڴ����鶼û����Ӧ��
		if(RestMemorySize > 0)
		{
			//����ڴ�ػ���ʣ�࣬���ص����ʵ�Freelist�У���Ϊ׼�����µ��ڴ����,���������ڴ���Ƭ
			//���ڴ��ʣ�������µ���8�ı���
			int GetBtye = AdjustBtyeSizeDown(RestMemorySize);
			if(GetBtye >= 8)
			{
				size_t GetIndex = FreeListIndex(GetBtye);
				Address* ListBeginAddress = (Address*)StartFree;
				StartFree = StartFree + GetBtye;
				ListBeginAddress->NextAddress = FreeList[GetIndex].NextAddress;
				FreeList[GetIndex].NextAddress = ListBeginAddress;
			}
			//��ʣ����ڴ��ͷ�
			int DeleteMemorySize = RestMemorySize - GetBtye;
			DeAllocate(StartFree, DeleteMemorySize);
		}
		//ÿ���ڴ�ش�С����8�ı���,��������ʹ���ڴ�����Խ��Խ�󣬲�����ڴ�ҲԽ��Խ��>>4 = ����8
		size_t AddMemoryByte = TotalNeedByte * 2 + ( HeapSize >> 4 );
		StartFree = ( char* )operator new( AddMemoryByte );
		if(StartFree == nullptr)
		{
			//˵��Heap�ռ�ľ�,�����Ƿ������ϻ����ѹ��صĻ�δ������㹻��Ŀ��ڴ�,�о�ж�������ϵ����鲹���ڴ��
			for(size_t i = Index; i < ArrayNum; i++)
			{
				if(FreeList[i].NextAddress != nullptr)
				{
					//˵���п����ڴ�
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
			//˵��������Ҳû���ڴ��ˣ�����new-handler����Ҳ���а취�������
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
//�����ڵĶ�����

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
	// //̫��Ч���ˣ����ع�
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
	//POD����ʲô������
	//pointer->~T1();
}
template<typename T1>
void Destructor(T1* pointer, __false_type)
{
	pointer->~T1();
}
//��POD���ͺͷ�POD��������ͬ����
template<typename InputIterator>
void Destory(InputIterator StartAddress, InputIterator EndAddress, __true_type)
{
	//POD����ʲôҲ������
}
template<typename InputIterator>
void Destory(InputIterator StartAddress, InputIterator EndAddress, __false_type)
{
	//��POD����Ҫ������������
	for(InputIterator Iter = StartAddress; Iter != EndAddress; ++Iter)
	{
		typedef IteratorTraits<InputIterator>::ValueType ValueType;
		Iter->~ValueType();
	}
}
//������������
template<typename InputIterator>
void Destory(InputIterator StartAddress, InputIterator EndAddress)
{
	Destory(StartAddress, EndAddress, __type_traits<InputIterator>::is_POD_type());
}

//�������俽��
template<typename InputIterator, typename NewInputIterator>
NewInputIterator Copy(InputIterator StartAddress, InputIterator EndAddress, NewInputIterator NewStartAddress, __true_type)
{
	memcpy(NewStartAddress, StartAddress, ( EndAddress - StartAddress )*sizeof( IteratorTraits<InputIterator>::ValueType ));

	return NewStartAddress + ( EndAddress - StartAddress );
}

template<typename InputIterator, typename NewInputIterator>
NewInputIterator Copy(InputIterator StartAddress, InputIterator EndAddress, NewInputIterator NewStartAddress, __false_type)
{
	//������operator= �����ж������飬�Ƿ�new��Դת�Ƶ�ʱ���ͷ�ԭ����Դ�ڽ�������Դ

	/*InputIterator Iter = StartAddress;
	NewInputIterator NewIter = NewStartAddress;*/
	for(; StartAddress != EndAddress; ++StartAddress, ++NewStartAddress)
	{
		*NewStartAddress = *StartAddress;
	}
	return NewStartAddress;
}
//�ػ���char*
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
//��Ҫ�ж��Ƿ���POD����

template<typename InputIterator, typename NewInputIterator>
NewInputIterator Copy(InputIterator StartAddress, InputIterator EndAddress, NewInputIterator NewStartAddress)
{
	return  Copy(StartAddress, EndAddress, NewStartAddress, __type_traits<InputIterator>::is_POD_type());
}

// ����ʹ��static��Ϊ�˲�ʵ�����͵��ú��������Ч��,���Ҷ��������ͬһ���ڴ���Ϸ���
//��ӡȫ��
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
////����������ظ�Ԫ�غ���
//template<typename InputIterator,typename T>
//void FillRepeatElement(Inpu�ۣ��tIterator StartAddress,InputIterator EndAddress,const T& Value)
//{
// //PrintAllElemt(StartAddress,EndAddress);
// //cout<<endl;
//InputIterator Iter = StartAddress;
//for (;Iter != EndAddress;++Iter)
//{
//	Constructor(Iter,Value);
//}
//}
//////////////////ȷ��Copy����ַ���ص��� CopyBack����ַ�ص������λ��ַ���������ڲ��룩 CopyFront����ַ�ص������λ����������ɾ��������ȫ�ֺ���
//�ع����������ΪVector��Ԫ��

//CopyBack����ַ�ص������λ��ַ���������ڲ��룩����ֵΪĿ�ĵص���ʼԪ��λ��
template<typename InputIterator, typename NewInputIterator>
NewInputIterator CopyBack(NewInputIterator DesEndPoint, InputIterator SrcStartPoint, InputIterator SrcEndPoint)
{
	return CopyBack_aux(DesEndPoint, SrcStartPoint, SrcEndPoint, __type_traits< IteratorTraits<InputIterator>::ValueType>::is_POD_type());
}
//POD���ͣ�ֱ�ӿ���
template<typename InputIterator, typename NewInputIterator>
NewInputIterator CopyBack_aux(NewInputIterator DesEndPoint, InputIterator SrcStartPoint, InputIterator SrcEndPoint, __true_type)
{
	//EndPoint �ǳ�β
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
	//EndPoint�ǳ�β
	//��POD���ͣ�����operator=����������ֹǳ������ָ��ͬһ�ѵ�ַ�Ķ����������Σ��������ǳ������operator=����������������������Copy���������
	//����Ŀ�ĵع���,��Ϊstringb������SGI��__type__traits�淶������ֻ�ܵ�����POD���ʹ������ܵ���memcpy�������C++11������ʹ�����õ�type__traits
	//�����ж�POD���ͺ�ֱ��ʹ��memcpy���Ƹ㶨��so easy ΢���������ҿ�Ӧ������Ϊ�������й��캯����������Ӧ���ǵ����˱�������չ�������ж�ʹ���˸���Ŀ�����ô�е��ù��캯��
	/***
	����֧��C++11��VS IDE�ع�����Ĵ���
	���������ߣ�����stringҲ����һ��
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
//CopyFront����ַ�ص������λ����������ɾ��������Ŀ�ĵ����һ��Ԫ�غ���λ��
template<typename InputIterator, typename NewInputIterator>
NewInputIterator CopyFront(NewInputIterator DesStartPoint, InputIterator SrcStartPoint, InputIterator SrcEndPoint)
{
	return CopyFront_aux(DesStartPoint, SrcStartPoint, SrcEndPoint, __type_traits< IteratorTraits<InputIterator>::ValueType>::is_POD_type());
}
//POD���ͣ�ֱ�ӿ���
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
	//��POD���ͣ�����operator=����������ֹǳ������ָ��ͬһ�ѵ�ַ�Ķ����������Σ��������ǳ������operator=����������������������Copy���������
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
//Ҫ������������ڴ�,���ڷ������ڴ��������BUG�������ع�
//ʵ�ʵ�SortӦ���ǲ��룬���ţ����ŵĻ�������㷨
template<typename InputIterator>
void QuickSort(InputIterator StartIter, InputIterator EndIter)
{
	InputIterator LowIter = StartIter;
	InputIterator HeightIter = --EndIter;
	//�����һ�����������һ��
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