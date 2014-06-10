#pragma once
#include "../concept/ztl_base_concept.h"
#include "../type/ztl_type_traits.h"
#include "../tuple/ztl_pair.h"
#include "../algorithm/ztl_algorithm.h"
#include "../ztl_iterator.h"
#include "../log/ztl_log.h"
namespace ztl
{
	template<typename DerivedType, typename ValueType, typename IteratorType, typename DifferentType, typename PointerType = ValueType*,
		typename ReferenceType = ValueType&, typename ConstReferenceType = const ValueType&,
		typename ConstPointerType = const ValueType*,
		typename ConstIteratorType = const IteratorType,
		typename SzieType = DifferentType>
	class IContainer :
		public virtual ztl::IAssignable < IContainer < DerivedType, ValueType,
		IteratorType, DifferentType, PointerType, ReferenceType,
		ConstReferenceType, ConstPointerType, ConstIteratorType
		, SzieType >> ,
		public virtual ztl::IEquality < IContainer < DerivedType, ValueType,
		IteratorType, DifferentType, PointerType, ReferenceType,
		ConstReferenceType, ConstPointerType, ConstIteratorType
		, SzieType >>
	{
	public:
		typedef ValueType					value_type;
		typedef const ValueType				const_value_type;
		typedef ReferenceType				reference_type;
		typedef ConstReferenceType			const_reference_type;
		typedef PointerType					pointer_type;
		typedef ConstPointerType			const_pointer_type;
		typedef IteratorType				iterator_type;
		typedef ConstIteratorType			const_iterator_type;
		typedef DifferentType				different_type;
		typedef SzieType					size_type;//size_type can represent anynon - negative value of difference_type
		typedef DerivedType				self_type;

 	public:
		/*iterator_type			begin();
		iterator_type			end();		
		void					clear();
		const_iterator_type		cbegin()	const
		{
		return const_cast<self_type*>(this)->begin();
		}
		const_iterator_type		cend()		const
		{
		return const_cast<self_type*>(this)->end();
		}
		size_type				size()		const
		{
			return  ztl::distance(((self_type*)this)->begin(), ((self_type*)this)->end());
		}
		size_type				max_size()	const
		{
			return -1;
		}
		bool					empty()		const
		{
			return ((self_type*)this)->begin() == ((self_type*)this)->end();
		}
		void swap(self_type&& target)
		{
			ztl::swap(ztl::forward<self_type&&>(target),ztl::forward<self_type&&>(*((self_type*)this)));
		}
	public:	
		bool operator== (const self_type& right) const
		{
			return size() == right.size() && ztl::equal(((self_type*)this)->begin(), ((self_type*)this)->end(), right.begin());
		}
		friend bool operator!= (const self_type& left, const self_type& right)
		{
		return !(left == right);
		}
		*/
		
		
	};
	template<typename DerivedType, typename ValueType, typename IteratorType, typename DifferentType, typename PointerType = ValueType*,
		typename ReferenceType = ValueType&, typename ConstReferenceType = const ValueType&,
		typename ConstPointerType = const ValueType*,
		typename ConstIteratorType = const IteratorType,
		typename SzieType = DifferentType>
	class IReverse :
		public  IContainer < DerivedType, ValueType,
		IteratorType, DifferentType, PointerType, ReferenceType,
		ConstReferenceType, ConstPointerType, ConstIteratorType
		, SzieType >
	{
	public:
		typedef  ztl::reverse_iterator<iterator_type> reverse_iterator_type;
		typedef  ztl::reverse_iterator<const_iterator_type> const_reverse_iterator_type;
		
	public:
		reverse_iterator_type			rbegin()
		{
			return reverse_iterator_type(static_cast<DerivedType*>(this)->end());
		}
		reverse_iterator_type			rend()
		{
			return reverse_iterator_type(static_cast<DerivedType*>(this)->begin());
		}
		/*const_reverse_iterator_type		crbegin()	const
		{
			return const_cast<self_type*>(this)->rbegin();
		}
		const_reverse_iterator_type		crend()		const
		{
			return const_cast<self_type*>(this)->rend();
		}*/
	};
	template<typename DerivedType, typename ReferenceType, typename ConstReferenceType, typename SizeType>
	class IRandomAccess
	{
	public:
		/*ReferenceType operator[](const SizeType n)
		{
			return  *(((DerivedType*)this)->begin() += n);
			
		}
		ConstReferenceType operator[](const SizeType n) const
		{
			return  *(((DerivedType*)this)->begin() += n);
		}
		ReferenceType at(const SizeType n)
		{
			ztl::ztl_assert(n >= 0 && n < size(), "error!");
			return  *(((DerivedType*)this)->begin() += n);
		}
		ConstReferenceType at(const SizeType n) const
		{
			ztl::ztl_assert(n >= 0 && n < size(), "error!");
			return  *(((DerivedType*)this)->begin() += n);
		}*/
	};
	template<typename DerivedType, typename ValueType, typename IteratorType, typename DifferentType, typename PointerType = ValueType*,
		typename ReferenceType = ValueType&, typename ConstReferenceType = const ValueType&,
		typename ConstPointerType = const ValueType*,
		typename ConstIteratorType = const IteratorType,
		typename SzieType = DifferentType>
	class ISequence :
		
		public  IReverse < DerivedType, ValueType,IteratorType, DifferentType, PointerType, ReferenceType,ConstReferenceType, ConstPointerType, ConstIteratorType, SzieType >
	{
	public:
		template<typename... FuncArgs>
		iterator_type	emplace(const iterator_type& Where, FuncArgs&&... Args);
		iterator_type	insert(const iterator_type& Where, value_type&& Val);
		iterator_type	insert(const iterator_type& Where, const value_type& Val);
	
		iterator_type	insert(const iterator_type& Where, different_type n, const_value_type& Val);
		template<typename InputIterator, class = typename std::enable_if<ztl::is_iterator<InputIterator>::value,
			void>::type>
		iterator_type	insert(const iterator_type& Where, InputIterator& first, InputIterator& last);
		iterator_type	insert(const iterator_type& Where, std::initializer_list<value_type> target);
		void			erase(const iterator_type& Where);
		void			erase(const iterator_type& first, const iterator_type& last);
		void			resize(const size_type n)
		{
			resize(n, value_type());
		}
		void			resize(const size_type n, const value_type& value);
		template<typename InputIterator, class = typename std::enable_if<ztl::is_iterator<InputIterator>::value,
			void>::type>
		void			assign(InputIterator& first, InputIterator& last)
		{
			self_type temp(n, target);
			ztl::swap(ztl::move(temp));
		}
		template<typename ContainerType>
		void			assign(ContainerType container)
		{
			assign(container.begin(), container.end());
		}
		void			assign(const different_type n, const_value_type& target)
		{
			self_type temp(n, target);
			ztl::swap(ztl::move(temp));
		}
	};
	template<typename ValueType,typename ReferenceType,typename ConstReferenceType>
	class IFrontInsert
	{
	public:
		ReferenceType		front();
		ConstReferenceType	front()const;
		void			push_front(ValueType&& value);
		void			push_front(const ValueType& _Val);
		void			pop_front();
		template<typename... FuncArgs>
		void			emplace_front(FuncArgs&&... Args);
	};
	template<typename ValueType,typename ReferenceType=ValueType&, typename ConstReferenceType=const ValueType&>
	class IBackInsert
	{
	public:
		ReferenceType	back();
		void			push_back(ValueType&& Val);
		void			push_back(const ValueType& Val);
		void			pop_back();
		template<typename... FuncArgs>
		void			emplace_back(FuncArgs&&... Args);
		ConstReferenceType	back()const
		{
			return const_cast<self_type cosnt&>(*this).back();
		}
	};


	template<typename KeyType, typename IteratorType, typename SizeType>
	class IAssociate
	{
	public:
		IteratorType							find(const KeyType& key)					const;
		SizeType								count(const KeyType& key)					const;
		ztl::pair<IteratorType, IteratorType>	equal_range(const KeyType& key)				const;
		void									erase(const KeyType&key);
		IteratorType							erase(IteratorType& first, IteratorType& last);
	};
	template< typename IteratorType, typename ValueType>
	class IUnique
	{
	public:
		ztl::pair<IteratorType, bool>	insert(const ValueType& target);
		template<typename InputIterator>
		void											insert(InputIterator& first, InputIterator& last);
	};
	template<typename KeyType, typename IteratorType, typename ValueType>
	class IMultiple
	{
	public:
		typedef KeyType key_type;
	public:
		IteratorType				insert(const ValueType& target);
		template<typename InputIterator>
		void						insert(InputIterator& first, InputIterator& last);
	};
	template<typename IteratorType, typename ValueType>
	class ISimple
	{
	public:
		typedef ValueType key_type;
		//Key 和Value是同一个
	};
	template<typename KeyType, typename IteratorType, typename MapType, typename ValueType = ztl::pair<const KeyType, MapType>>
	class IPair
	{
	public:
		typedef MapType mapped_type;
		typedef KeyType key_type;
	};
	template<typename KeyCompare, typename IteratorType, typename ValueCompare, typename KeyType>
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
	template<typename HasherType, typename SizeType, typename KeyType>
	class IHash
	{
	public:
		typedef HasherType hasher;
	public:
		hasher					hash_func()		const;
		void					resize();
		SizeType				bucket_count()	const;
	};
}