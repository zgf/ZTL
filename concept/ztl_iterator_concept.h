#pragma once
#include "../type/ztl_type_traits.h"
#include "../concept/ztl_base_concept.h"
namespace ztl
{
	
	template<typename IteratorType, typename ValueType, typename IteratorCategory, typename DifferentType = ptrdiff_t, typename PointerType = ValueType*, typename ReferenceType = ValueType&>
	class IIterator
	{
	public:
		typedef ValueType				value_type;
		typedef const value_type		const_value_type;
		typedef PointerType				pointer_type;
		typedef const pointer_type		const_poiner_type;
		typedef ReferenceType			reference_type;
		typedef const reference_type	const_reference_type;
		typedef DifferentType			different_type;
		typedef IteratorCategory		iterator_category;
		typedef IteratorType			self_type;
	public:
		const_reference_type operator*()const
		{
			return *(const_cast<self_type const&>(*static_cast<self_type*>(this)).ptr);
		}
		reference_type operator*();

	public:
		const_poiner_type operator->()const
		{
			return &*this;
		}
		pointer_type operator->()
		{
			return &*this;
		}

	};
	template<typename IteratorType, typename ValueType, typename IteratorCategory, typename DifferentType = ptrdiff_t, typename PointerType = ValueType*, typename ReferenceType = ValueType&>
	class IInputIterator :
		public IIterator<IteratorType, ValueType, IteratorCategory, DifferentType, PointerType, ReferenceType>
	{
	public:

		self_type& operator++();
	public:
		void operator++(int)
		{
			++(*this);
		}
	};

	template<typename IteratorType, typename ValueType, typename IteratorCategory, typename DifferentType = ptrdiff_t, typename PointerType = ValueType*, typename ReferenceType = ValueType&>
	class IOutputIterator :
		public IIterator<IteratorType, ValueType, IteratorCategory, DifferentType, PointerType, ReferenceType>
	{
	public:
		self_type& operator++();
	public:
		void operator++(int)
		{
			++(*this);
		}
	};

	template<typename IteratorType, typename ValueType, typename IteratorCategory, typename DifferentType = ptrdiff_t, typename PointerType = ValueType*, typename ReferenceType = ValueType&>
	class IForwardIterator :
		public virtual IIterator<IteratorType, ValueType, IteratorCategory, DifferentType, PointerType, ReferenceType>,
		public virtual ztl::IEquality<IteratorType>//,
		//public virtual ztl::IAssignable<IteratorType>
	{
	public:
		self_type& operator++();
	public:
		/*这里应该是VC编译器的bug.int的参数是由编译器去填的,根本没去基类找operator++(int)*/
		/*self_type operator++(int)
		{
			self_type temp(*this);
			++(*this);
			return ztl::move(temp);
		}*/
	};
	template<typename IteratorType, typename ValueType, typename IteratorCategory, typename DifferentType = ptrdiff_t, typename PointerType = ValueType*, typename ReferenceType = ValueType&>
	class IBidrectionalIterator :
		public virtual IForwardIterator<IteratorType, ValueType, IteratorCategory, DifferentType, PointerType, ReferenceType>
	{
	public:
		self_type& operator--();
	public:
		/*self_type operator--(int)
		{
			self_type temp(*this);
			--(*this);
			return ztl::move(temp);
		}*/
	};
	template<typename IteratorType, typename ValueType, typename IteratorCategory, typename DifferentType = ptrdiff_t, typename PointerType = ValueType*, typename ReferenceType = ValueType&>
	class IRandomAcessIterator :
		public virtual IBidrectionalIterator<IteratorType, ValueType, IteratorCategory, DifferentType, PointerType, ReferenceType>,
		public virtual ztl::IOrdering<IteratorType>
	{
	public:
		self_type&		operator+=(const different_type n);
		self_type&		operator-=(const different_type n);
		different_type	operator-(const self_type& target)const;

	public:
		friend self_type operator+(const self_type& Iter, different_type n)
		{
			self_type tempIter = Iter;
			return ztl::move(tempIter += n);
		}
		friend self_type operator+(const different_type n, const self_type& Iter)
		{
			return ztl::move(Iter + n);
		}
		friend self_type operator-(const self_type& Iter, different_type n)
		{
			self_type tempIter = Iter;
			return ztl::move(tempIter -= n);
		}
		friend self_type operator-(const different_type n, const self_type& Iter)
		{
			return ztl::move(Iter - n);
		}
		reference_type operator[](const different_type n)
		{
			return *(this += n);
		}
	};
}