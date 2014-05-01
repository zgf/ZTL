#pragma once
#include "Z_Exception.h"
namespace ztl
{
	using std::nullptr_t;
	const nullptr_t null = 0;
	template<typename Type>
	class Nullable
	{
	public:
		enum class NullType :bool
		{
			Null,
			Val,
		};
	public:
		Type				data;
		NullType			hasVal;
	public:
		Nullable(nullptr_t)
		{
			data = Type();
			hasVal = NullType::Null;
		}
		Nullable(const Type& tData)
		{
			data = tData;
			hasVal = NullType::Val;
		}
		Nullable()
		{
			hasVal = NullType::Null;
		}
		~Nullable()
		{
		}
		Nullable(const Nullable& a)
		{
			this->hasVal = a.hasVal;
			if(a.hasVal == NullType::Val)
			{
				this->data = a.data;
			}
		}
	public:
		Nullable& operator=(const Nullable& a )
		{
			//a不可能含有指针
			this->data = a.data;
			this->hasVal = a.hasVal;
		}
		
		operator Type()
		{
			if(this->hasVal == NullType::Null)
			{
				throw ztl::Exception("Null Value Can't Convert Raw Type!");
			}
			return data;
		}
		bool operator==( const Nullable& a )
		{
			if(this->hasVal != a.hasVal)
			{
				return false;
			}
			else if(this->data == a.data)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		bool operator==( nullptr_t )
		{
			if(this->hasVal == NullType::Null)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		bool operator!=( const Nullable& a )
		{
			return !operator== ( a );
		}
		bool operator!=( nullptr_t )
		{
			return !operator== ( nullptr_t );
		}
	};
	//对指针类型的偏特化
	template<typename Type>
	class Nullable<Type*>
	{
		typedef  Type* Pointer;
	public:
		Pointer data;
	public:
		Nullable()
		{
			data = nullptr;
		}
		Nullable(const Nullable& a)
		{
			if(data)
			{
				delete data;
			}
			data = a;
		}
		Nullable(const Pointer& tData) :data(tData)
		{
		}
		Nullable(nullptr_t)
		{
			data = nullptr;
		}
		~Nullable()
		{
			delete data;
		}
	public:
		Nullable& operator=( const Nullable& a )
		{
			if(data)
			{
				delete data;
			}
			data = a;
		}
		operator Type*( )
		{
			return data;
		}
		bool operator==( const Nullable& a )
		{
			return data == a.data;
		}
		bool operator!= ( const Nullable& a )
		{
			return data != a.data;
		}
		bool operator!= ( nullptr_t )
		{
			return data != nullptr;
		}
		bool operator==( nullptr_t )
		{
			return data == nullptr;
		}
	};
}
