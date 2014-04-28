#pragma once
#include "ztl_concept.h"
#include "ztl_type_traits.h"
namespace ztl
{
	namespace apdapter
	{
		namespace iterator_apdater
		{
			template<typename FrontInsertContainer>
			class front_insert_iterator :
				public virtual ztl::concept::concept_iterator::IOutputIterator<front_insert_iterator<FrontInsertContainer>, void, void, void, void, ztl::traits::iterator_traits::output_iterator>
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
				explicit front_insert_iterator(FrontInsertContainer& target) :containerPtr(&target)
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
					containerPtr->push_front(std::forward<value_type>(Val));
					return *this;
				}
				/*
				新姿势:为什么都是
				return *this;
				因为这个迭代器要配合copy函数,copy函数是覆盖语义.中间会有把源迭代器内容拷贝到目的迭代器.
				那么对于目的迭代器的=运算符重载.给予插入语义.对于用户来说,肯定没法直接修改库提供的迭代器的=语义.但是可以通过
				加入一个间接层来解决.直接用这个iterator_apdater就搞定了.
				*/
			};
			template<typename BackInsertContainer>
			class back_insert_iterator :
				public virtual ztl::concept::concept_iterator::IOutputIterator<back_insert_iterator<BackInsertContainer>, void, void, void, void, ztl::traits::iterator_traits::output_iterator>
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
					containerPtr->push_back(std::forward<value_type>(Val));
					return *this;
				}

			};
			template<typename InsertContainer>
			class insert_iterator :
				public virtual ztl::concept::concept_iterator::IOutputIterator<insert_iterator<InsertContainer>, void, void, void, void, ztl::traits::iterator_traits::output_iterator>
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
			};
			template<typename BidirectionalIterator,
				typename IteratorCategory =
				typename ztl::traits::iterator_traits::iterator_traits<BidirectionalIterator>::iterator_category>
			class reverse_iterator :
				public virtual ztl::concept::concept_base::IEquality <
				reverse_iterator < BidirectionalIterator, IteratorCategory >> ,
				public virtual ztl::concept::concept_base::IAssignable <
				reverse_iterator < BidirectionalIterator, IteratorCategory >> ,
				public virtual ztl::concept::concept_base::IOrdering <
				reverse_iterator < BidirectionalIterator, IteratorCategory >> ,
				public virtual ztl::concept::concept_iterator::IRandomAcessIterator<
				reverse_iterator<BidirectionalIterator, IteratorCategory>,
				typename ztl::traits::iterator_traits::iterator_traits<BidirectionalIterator>::value_type,
				typename ztl::traits::iterator_traits::iterator_traits<BidirectionalIterator>::pointer_type,
				typename ztl::traits::iterator_traits::iterator_traits<BidirectionalIterator>::reference_type,
				typename ztl::traits::iterator_traits::iterator_traits<BidirectionalIterator>::different_type,
				typename ztl::traits::iterator_traits::iterator_traits<BidirectionalIterator>::iterator_category>
			{
			public:
				typedef reverse_iterator<BidirectionalIterator, IteratorCategory>					mySelf;
				typedef BidirectionalIterator														iterator_type;
				typedef typename ztl::traits::iterator_traits::iterator_traits<iterator_type>::value_type			value_type;
				typedef typename ztl::traits::iterator_traits::iterator_traits<iterator_type>::pointer_type			pointer_type;
				typedef typename ztl::traits::iterator_traits::iterator_traits<iterator_type>::reference_type		reference_type;
				typedef typename ztl::traits::iterator_traits::iterator_traits<iterator_type>::different_type		different_type;
				typedef typename ztl::traits::iterator_traits::iterator_traits<iterator_type>::iterator_category	iterator_category;

			public:
				iterator_type iterator;
			public:/*IAssignable*/
				reverse_iterator()
				{
				}

				reverse_iterator(mySelf& target) :iterator(target)
				{

				}
				mySelf&operator=(const mySelf& target)
				{
					iterator = target.base();
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
			public:/*IOrdering*/
				bool					operator<(const mySelf& right)const
				{
					return iterator < right.iterator;
				}
			public: /*IIterator*/
				reference_type			operator*()
				{
					auto temp = iterator;
					return *(--temp);
				}
			public:/*IForwardIterator*/
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
			public:/*Self*/
				iterator_type base()const 
				{
					return iterator;
				}
			};
		}
	}

}