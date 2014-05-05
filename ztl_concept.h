#pragma once
#include "ztl_concept_base.h"
#include "ztl_wrapper.h"
namespace ztl
{
	namespace concept
	{
		
		namespace concept_iterator
		{
			template<typename IteratorType, typename ValueType, typename IteratorCategory, typename DifferentType = ptrdiff_t, typename PointerType = ValueType*, typename ReferenceType = ValueType&>
			class IIterator /*:*/
				/*public concept_base::IEquality<IteratorType>, */
			/*public  concept_base::IAssignable<IteratorType>*/
			{
			public:
				typedef ValueType				value_type;
				typedef PointerType				pointer_type;
				typedef ReferenceType			reference_type;
				typedef DifferentType			different_type;
				typedef IteratorCategory		iterator_category;
			public:
				reference_type operator*()const;
			public:
				pointer_type operator->()const
				{
					return &*this;
				}
			};
			template<typename IteratorType, typename ValueType, typename IteratorCategory, typename DifferentType = ptrdiff_t, typename PointerType = ValueType*, typename ReferenceType = ValueType&>
			class IInputIterator :
				public IIterator<IteratorType, ValueType, IteratorCategory, DifferentType, PointerType, ReferenceType>
			{
			public:

				IteratorType& operator++();
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
				IteratorType& operator++();
			public:
				void operator++(int)
				{
					++(*this);
				}
			};

			template<typename IteratorType, typename ValueType, typename IteratorCategory, typename DifferentType = ptrdiff_t, typename PointerType = ValueType*, typename ReferenceType = ValueType&>
			class IForwardIterator :
				public virtual IIterator<IteratorType, ValueType, IteratorCategory, DifferentType, PointerType, ReferenceType>,
				public virtual concept_base::IEquality<IteratorType>,
				public virtual concept_base::IAssignable<IteratorType>
			{
			public:
				IteratorType& operator++();
			public:
				IteratorType operator++(int)
				{
					IteratorType temp(*this);
					++(*this);
					return ztl::traits::type_traits::move(temp);
				}
			};
			template<typename IteratorType, typename ValueType, typename IteratorCategory, typename DifferentType = ptrdiff_t, typename PointerType = ValueType*, typename ReferenceType = ValueType&>
			class IBidrectionalIterator :
				public IForwardIterator<IteratorType, ValueType, IteratorCategory, DifferentType, PointerType, ReferenceType>
			{
			public:
				IteratorType& operator--();
			public:
				IteratorType operator--(int)
				{
					IteratorType temp(*this);
					--(*this);
					return ztl::traits::type_traits::move(temp);
				}
			};
			template<typename IteratorType, typename ValueType, typename IteratorCategory, typename DifferentType = ptrdiff_t, typename PointerType = ValueType*, typename ReferenceType = ValueType&>
			class IRandomAcessIterator :
				public IBidrectionalIterator<IteratorType, ValueType, IteratorCategory, DifferentType, PointerType, ReferenceType>,
				concept_base::IOrdering<IteratorType>
			{
			public:
				IteratorType&		operator+=(const different_type n);
				IteratorType&		operator-=(const different_type n);
			public:
				friend IteratorType operator+(const IteratorType& Iter, different_type n)
				{
					IteratorType tempIter = Iter;
					return ztl::traits::type_traits::move(tempIter += n);
				}
				friend IteratorType operator+(const different_type n, const IteratorType& Iter)
				{
					return ztl::traits::type_traits::move(Iter + n);
				}
				friend IteratorType operator-(const IteratorType& Iter, different_type n)
				{
					IteratorType tempIter = Iter;
					return ztl::traits::type_traits::move(tempIter -= n);
				}
				friend IteratorType operator-(const different_type n, const IteratorType& Iter)
				{
					return ztl::traits::type_traits::move(Iter - n);
				}
				reference_type operator[](const different_type n)
				{
					return *(this += n);
				}
			};
		}
		namespace container_concept
		{
			template<typename DerivedType,typename ValueType,
				typename ReferenceType,typename ConstReferenceType,
				typename PointerType,typename ConstPointerType,
				typename IteratorType,typename ConstIteratorType,
				typename DifferentType,typename SzieType>
			class IContainer :
			public virtual concept_base::IAssignable<IContainer<DerivedType, ValueType,
					ReferenceType, ConstReferenceType, PointerType, ConstPointerType,
					IteratorType, ConstIteratorType,
					DifferentType, SzieType >> , 
			public virtual concept_base::IEquality <IContainer <DerivedType, ValueType,
					 ReferenceType, ConstReferenceType, PointerType, ConstPointerType,
					IteratorType, ConstIteratorType,
					DifferentType, SzieType >>
			{
			public:
				typedef ValueType					value_type;
				typedef ReferenceType				reference_type;
				typedef ConstReferenceType			const_reference_type;
				typedef PointerType					pointer_type;
				typedef ConstPointerType			const_pointer_type;
				typedef IteratorType				iterator_type;
				typedef ConstIteratorType			const_iterator_type;
				typedef DifferentType				different_type;
				typedef SzieType					size_type;
			public:
				iterator_type&			begin()		const; 
				iterator_type&			end()		const;
				const_iterator_type&	cbegin()	const;
				const_iterator_type&	cend()		const;
				size_type				size()		const;
				size_type				max_size()	const;
				bool					empty()		const;
				void					swap(const DerivedType& target);
			};
			template<typename ReverseType,typename ConstReverseType>
			class IReverse
			{
			public:
				typedef ReverseType reverse_iterator;
				typedef ConstReverseType const_reverse_iterator;
			public:
				reverse_iterator&			rbegin()	const;
				reverse_iterator&			rend()		const;
				const_reverse_iterator&		rcbegin()	const;
				const_reverse_iterator&		rcend()		const;
			};
			template<typename IteratorType,typename SizeType>
			class IRandomAccess
			{
				public:
					IteratorType& operator[](const SizeType n);
			};
			template<typename ValueType, typename IteratorType, typename DifferentType, typename ReferenceType>
			class ISequence
			{
			public:
				IteratorType	insert(const IteratorType& Where,const ValueType& Val);
				IteratorType	insert(const IteratorType& Where, DifferentType n, const ValueType& Val);
				template<typename InputIterator>
				IteratorType	insert(const IteratorType& Where,InputIterator& start,InputIterator& end);
				void			erase(const IteratorType& Where);
				void			erase(const IteratorType& start, const IteratorType& end);
			};
			template<typename ReferenceType>
			class IFrontInsert
			{
			public:
				ReferenceType	front()			const;
				void			push_front()		 ;
				void			pop_front()			 ;
			};
			template<typename ReferenceType>
			class IBackInsert
			{
			public:
				ReferenceType	back()			const;
				void			push_back()			 ;
				void			pop_back()			 ;
			};
			template<typename KeyType,typename IteratorType,typename SizeType>
			class IAssociate
			{

			public:
				IteratorType											find(const KeyType& key)					const;
				SizeType												count(const KeyType& key)					const;
				ztl::wrapper::tuples::pair<IteratorType, IteratorType>	equal_range(const KeyType& key)				const;
				void													erase(const KeyType&key);
				IteratorType											erase(IteratorType& start, IteratorType& end)	 ;
			};
			template< typename IteratorType, typename ValueType>
			class IUnique
			{
			public:
				ztl::wrapper::tuples::pair<IteratorType, bool>	insert(const ValueType& target);
				template<typename InputIterator>
				void											insert(InputIterator& start, InputIterator& end);
			};
			template<typename KeyType, typename IteratorType, typename ValueType>
			class IMultiple
			{
			public:
				typedef KeyType key_type;
			public:
				IteratorType				insert(const ValueType& target);
				template<typename InputIterator>
				void						insert(InputIterator& start, InputIterator& end);
			};
			template<typename IteratorType, typename ValueType>
			class ISimple
			{
			public:
				typedef ValueType key_type;
				//Key 和Value是同一个
			};
			template<typename KeyType, typename IteratorType, typename MapType, typename ValueType = ztl::wrapper::tuples::pair<const KeyType, MapType>>
			class IPair
			{
			public:
				typedef MapType mapped_type;
				typedef KeyType key_type;
			};
			template<typename KeyCompare, typename IteratorType, typename ValueCompare,typename KeyType>
			class ISort
			{
			public:
				typedef KeyCompare key_compare;
				typedef ValueCompare value_compare;
			public:
				key_compare				key_comp()							const;
				value_compare			value_comp()						const;
				IteratorType			lower_bound(const KeyType& key)		const;
				IteratorType			uppper_bound(const KeyType& key)	const;
			};
			template<typename HasherType,typename SizeType, typename KeyType>
			class IHash
			{
			public:
				typedef HasherType hasher;
			public:
				hasher					hash_func()		const;
				void					resize()			 ;
				SizeType				bucket_count()	const;
			};
		}
	}
	
}