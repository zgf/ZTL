#pragma once
#include "../concept/ztl_iterator_concept.h"
#include "../type/ztl_type_traits.h"
#include <iostream>
namespace ztl
{
	template<typename FrontInsertContainer>
	class front_insert_iterator :
		public virtual ztl::IOutputIterator<front_insert_iterator<FrontInsertContainer>, void, output_iterator, void, void, void>
	{
	public:
		typedef FrontInsertContainer							container_type;
		typedef FrontInsertContainer*							container_pointer;
		typedef front_insert_iterator<FrontInsertContainer>		mySelf;
		typedef typename FrontInsertContainer::value_type		value_type;
	protected:
		container_pointer containerPtr;
	public:
		front_insert_iterator() = delete;
		explicit front_insert_iterator(container_type& target) :containerPtr(&target)
		{
		}
		mySelf& operator++()
		{
			return *this;
		}
		mySelf& operator*()
		{
			return *this;
		}
		mySelf& operator=(const value_type& Val)
		{
			containerPtr->push_front(Val);
			return *this;
		}
		mySelf& operator=(value_type&& Val)
		{
			containerPtr->push_front(ztl::forward<value_type>(Val));
			return *this;
		}
		/*
		新姿势:为什么都是
		return *this;
		因为这个迭代器要配合copy函数,copy函数是覆盖语义.中间会有把源迭代器内容拷贝到目的迭代器.
		那么对于目的迭代器的=运算符重载.给予插入语义.对于用户来说,肯定没法直接修改库提供的迭代器的=语义.但是可以通过
		加入一个间接层来解决.直接用这个iterator_apdater就搞定了.
		*/
	public:
		static mySelf make_front_insert_iterator(container_type& target)
		{
			return ztl::move(mySelf(target));
		}
	};
	template<typename BackInsertContainer>
	class back_insert_iterator :
		public virtual ztl::IOutputIterator<back_insert_iterator<BackInsertContainer>, void, output_iterator, void, void, void>
	{
	public:
		typedef BackInsertContainer									container_type;
		typedef BackInsertContainer*								container_pointer;
		typedef back_insert_iterator<BackInsertContainer>			mySelf;
		typedef typename BackInsertContainer::value_type			value_type;
	protected:
		container_pointer containerPtr;
	public:
		back_insert_iterator() = delete;
		explicit back_insert_iterator(BackInsertContainer& target) :containerPtr(&target)
		{
		}
		mySelf& operator++()
		{
			return *this;
		}
		mySelf& operator*()
		{
			return *this;
		}
		mySelf& operator=(const value_type& Val)
		{
			containerPtr->push_back(Val);
			return *this;
		}
		mySelf& operator=(value_type&& Val)
		{
			containerPtr->push_back(ztl::forward<value_type>(Val));
			return *this;
		}
	public:
		static mySelf make_back_insert_iterator(container_type& target)
		{
			return ztl::move(mySelf(target));
		}
	};
	template<class Container> inline
		ztl::back_insert_iterator<Container> back_inserter(Container& Cont)
	{	// return a back_insert_iterator
			return (ztl::back_insert_iterator<Container>(Cont));
	}
	template<typename InsertContainer>
	class insert_iterator :
		public virtual ztl::IOutputIterator<insert_iterator<InsertContainer>, void, output_iterator, void, void, void>
	{
	public:
		typedef InsertContainer									container_type;
		typedef InsertContainer*								container_pointer;
		typedef insert_iterator<InsertContainer>				mySelf;
		typedef typename InsertContainer::value_type			value_type;
		typedef typename InsertContainer::iterator			iterator_type;
	protected:
		container_pointer										containerPtr;
		iterator_type											iter;
	public:
		insert_iterator() = delete;
		explicit insert_iterator(container_type& targetContainer, iterator_type& targetIterator)
			:containerPtr(&targetContainer), iter(targetIterator)
		{
		}
		mySelf& operator++()
		{
			return *this;
		}
		mySelf& operator*()
		{
			return *this;
		}
		mySelf& operator=(const value_type& Val)
		{
			containerPtr->insert(iter, Val);
			++iter;
			return *this;
		}
		mySelf& operator=(value_type&& Val)
		{
			containerPtr->insert(iter, Val);
			++iter;
			return *this;
		}
	public:/*self*/
		static mySelf make__insert_iterator(container_type& targetContainer, iterator_type& targetIterator)
		{
			return ztl::move(mySelf(targetContainer, targetIterator));
		}
	};
	template<typename BidirectionalIterator,
		typename IteratorCategory =
		typename iterator_traits<BidirectionalIterator>::iterator_category>
	class reverse_iterator
	{
	public:
		typedef reverse_iterator<BidirectionalIterator, IteratorCategory>					mySelf;
		typedef BidirectionalIterator														iterator_type;
		typedef typename iterator_traits<iterator_type>::value_type			value_type;
		typedef typename iterator_traits<iterator_type>::pointer_type			pointer_type;
		typedef typename iterator_traits<iterator_type>::reference_type		reference_type;
		typedef typename iterator_traits<iterator_type>::different_type		different_type;
		typedef typename iterator_traits<iterator_type>::iterator_category	iterator_category;
		typedef mySelf self_type;
		typedef const reference_type const_reference_type;
		typedef const pointer_type const_pointer_type;
	public:
		iterator_type iterator;
	public:/*IAssignable*/
		reverse_iterator()
		{
		}

		reverse_iterator(const mySelf& target) :iterator(target.iterator)
		{
		}
		explicit reverse_iterator(const iterator_type&target) :iterator(target)
		{
		}
		mySelf&operator=(const mySelf& target)
		{
			iterator = target.base();
			return *this;
		}
		mySelf&operator=(const iterator_type& target)
		{
			iterator = target;
			return *this;
		}
		~reverse_iterator()
		{
		}
	public: /*IEquality*/
		bool					operator==(const mySelf& right) const
		{
			return iterator == right.iterator;
		}
		friend bool operator!= (const mySelf& left, const mySelf& right)
		{
			return !(left == right);
		}
	public:/*IOrdering*/
		bool					operator<(const mySelf& right)const
		{
			return right.iterator < iterator;
		}
		friend bool operator<= (const mySelf& left, const mySelf& right)
		{
			return !(left < right);
		}
		friend bool operator>(const mySelf& left, const mySelf& right)
		{
			return right < left;
		}
		friend bool operator>= (const mySelf& left, const mySelf& right)
		{
			return !(right < left);
		}
	public: /*IIterator*/
		reference_type			operator*()
		{
			auto temp = iterator;
			return *(--temp);
		}
		const_reference_type			operator*() const
		{
			auto temp = iterator;
			return *(--temp);
		}

		/*const_poiner_type operator->() const
		{
			return &*this;
		}*/
		pointer_type operator->()
		{
			return &*this;
		}
		self_type operator++(int)
		{
			self_type temp(*this);
			++(*this);
			return ztl::move(temp);
		}
		mySelf&					operator++()
		{
			--iterator;
			return *this;
		}
	public: /*IBidrectionalIterator*/
		mySelf&					operator--()
		{
			++iterator;
			return *this;
		}
		self_type operator--(int)
		{
			self_type temp(*this);
			--(*this);
			return ztl::move(temp);
		}
	public:	/*IRandomAcessIterator*/
		mySelf&					operator+=(const different_type n)
		{
			iterator -= n;
			return *this;
		}
		mySelf&					operator-=(const different_type n)
		{
			iterator += n;
			return *this;
		}
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
		friend different_type operator-(const self_type& left, const self_type& right)
		{
			return  right.iterator - left.iterator;
		}
		friend self_type operator-(const different_type n, const self_type& Iter)
		{
			return ztl::move(Iter - n);
		}
		reference_type operator[](const different_type n)
		{
			return *(this -= n);
		}
	public:/*Self*/
		iterator_type base()const
		{
			return iterator;
		}
		static mySelf make_reverse_iterator(iterator_type&target)
		{
			return ztl::move(mySelf(target));
		}
	};
	template<typename ElementType, typename CharType = char, typename CharTraits = std::char_traits<CharType>>
	class istream_iterator :
		public virtual IInputIterator<
		istream_iterator<ElementType>, ElementType, input_iterator>,
		public virtual IEquality <
		istream_iterator < ElementType, CharType, CharTraits >>
	{
	public:
		typedef ElementType												element_type;
		typedef CharType												char_type;
		typedef CharTraits												traits_type;
		typedef istream_iterator<element_type, char_type, traits_type>	mySelf;

		//这块以后换成自己的流=.=mark
		typedef std::basic_istream<char_type, traits_type> istream_type;
	public:
		istream_type* streamPtr;
		element_type val;
	public:
		mySelf() :streamPtr(nullptr), val(element_type())
		{
		}
		mySelf(istream_type& target) :streamPtr(&target)
		{
			get_val();
		}

		mySelf& operator=(istream_type& target)
		{
			streamPtr = &target;
			get_val();
		}
		~istream_iterator()
		{
		}
	public:	/*IInputIterator*/
		reference_type			operator*()
		{
			return val;
		}
		mySelf&					operator++()
		{
			get_val();
			return *this;
		}
	public:	/*IEquality*/
		bool operator==(const mySelf& right)
		{
			return this.streamPtr == right.streamPtr;
		}
	protected:/*self*/
		void get_val()
		{
			/*这里很有意思,如果指针指向了一个流,并且从流中获取值为0,那么就将streamPtr设置为空,iterator end都是nullptr,这样就形成了一个有效的range*/
			if(streamPtr != 0 && !(*streamPtr >> val))
			{
				streamPtr = nullptr;
			}
		}
	};
	template<typename ElementType, typename CharType = char, typename CharTraits = std::char_traits<CharType>>
	class ostream_iterator :
		public virtual ztl::IOutputIterator<
		ostream_iterator<ElementType, CharType, CharTraits>, void, output_iterator, void, void, void>
	{
	public:
		typedef ElementType											 element_type;
		typedef CharType											 char_type;
		typedef CharTraits											 traits_type;
		typedef ostream_iterator<element_type, char_type, traits_type> mySelf;
		//这块以后换成自己的流=.=mark
		typedef std::basic_ostream<char_type, traits_type> ostream_type;
	public:
		ostream_type*						streamPtr;
		const char_type*					delim;
	public:
		mySelf() = delete;
		mySelf(ostream_type& target, const char_type* tdelim = nullptr)
			:streamPtr(&target), delim(tdelim)
		{
		}

		mySelf& operator=(const element_type& target)
		{
			*streamPtr << target;
			if(delim != nullptr)
			{
				*streamPtr << delim;
			}
			return *this;
		}
		~ostream_iterator()
		{
		}
	public:
		mySelf& operator*()
		{
			return *this;
		}
		mySelf& operator++()
		{
			return*this;
		}
	};
	template<typename BidirectionalIterator,
		typename IteratorCategory =
		typename iterator_traits<BidirectionalIterator>::iterator_category>
	class move_iterator :
		public virtual IEquality <
		move_iterator < BidirectionalIterator, IteratorCategory >> ,
		public virtual IAssignable <
		move_iterator < BidirectionalIterator, IteratorCategory >> ,
		public virtual IOrdering <
		move_iterator < BidirectionalIterator, IteratorCategory >> ,
		public virtual IRandomAcessIterator<
		move_iterator<BidirectionalIterator, IteratorCategory>,
		typename iterator_traits<BidirectionalIterator>::value_type,
		typename iterator_traits<BidirectionalIterator>::iterator_category,
		typename iterator_traits<BidirectionalIterator>::different_type,
		typename iterator_traits<BidirectionalIterator>::pointer_type,
		typename iterator_traits<BidirectionalIterator>::reference_type
		>
	{
	public:
		typedef move_iterator<BidirectionalIterator, IteratorCategory>					mySelf;
		typedef BidirectionalIterator														iterator_type;
		typedef typename iterator_traits<iterator_type>::value_type			value_type;
		typedef typename iterator_traits<iterator_type>::pointer_type			pointer_type;
		typedef typename iterator_traits<iterator_type>::reference_type		reference_type;
		typedef typename iterator_traits<iterator_type>::different_type		different_type;
		typedef typename iterator_traits<iterator_type>::iterator_category	iterator_category;

	public:
		iterator_type iterator;
	public:/*IAssignable*/
		move_iterator()
		{
		}
		move_iterator(iterator_type& target) :iterator(target)
		{
		}
		move_iterator(mySelf& target) :iterator(target.iterator)
		{
		}
		mySelf&operator=(const mySelf& target)
		{
			iterator = target.base();
			return *this;
		}
		mySelf&operator=(iterator_type& target)
		{
			iterator = target;
			return *this;
		}
		~move_iterator()
		{
		}
	public: /*IEquality*/
		bool					operator==(const mySelf& right) const
		{
			return iterator == right.iterator;
		}
	public:/*IOrdering*/
		bool					operator<(const mySelf& right)const
		{
			return iterator < right.iterator;
		}
	public: /*IIterator*/
		reference_type			operator*()
		{
			return ztl::move(*iterator);
		}
	public:/*IForwardIterator*/
		mySelf&					operator++()
		{
			++iterator;
			return *this;
		}
	public: /*IBidrectionalIterator*/
		mySelf&					operator--()
		{
			--iterator;
			return *this;
		}
	public:	/*IRandomAcessIterator*/
		mySelf&					operator+=(const different_type n)
		{
			iterator += n;
			return *this;
		}
		mySelf&					operator-=(const different_type n)
		{
			iterator -= n;
			return *this;
		}
	public:/*Self*/
		iterator_type base()const
		{
			return iterator;
		}
		static mySelf& make_move_iterator(iterator_type& target)
		{
			return ztl::move(mySelf(target));
		}
	};
	template<typename IteratorType, typename ElementType>
	class raw_storage_iterator :
		virtual public ztl::IOutputIterator<
		raw_storage_iterator<IteratorType, ElementType>, void, output_iterator, void, void, void>
	{
	public:
		typedef IteratorType iterator_type;
		typedef ElementType element_type;
		typedef raw_storage_iterator<iterator_type, element_type> mySelf;
	protected:
		iterator_type Iter;
	public:/*ztl::IOutputIterator*/
		mySelf& operator++()
		{
			++Iter;
			return *this;
		}
		reference_type operator*()const
		{
			return *this;
		}
		mySelf& operator=(const element_type& Val)
		{
			new(static<void*>(&*Iter))element_type(Val);
			return *this;
		}
	};
}