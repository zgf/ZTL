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
				public virtual ztl::concept::concept_iterator::IOutputIterator<front_insert_iterator<FrontInsertContainer>, void, ztl::traits::iterator_traits::output_iterator, void, void, void>
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
					containerPtr->push_front(std::forward<value_type>(Val));
					return *this;
				}
				/*
				������:Ϊʲô����
				return *this;
				��Ϊ���������Ҫ���copy����,copy�����Ǹ�������.�м���а�Դ���������ݿ�����Ŀ�ĵ�����.
				��ô����Ŀ�ĵ�������=���������.�����������.�����û���˵,�϶�û��ֱ���޸Ŀ��ṩ�ĵ�������=����.���ǿ���ͨ��
				����һ����Ӳ������.ֱ�������iterator_apdater�͸㶨��.
				*/
			public:
				static mySelf make_front_insert_iterator(container_type& target)
				{
					return std::move(mySelf(target));
				}
			};
			template<typename BackInsertContainer>
			class back_insert_iterator :
				public virtual ztl::concept::concept_iterator::IOutputIterator<back_insert_iterator<BackInsertContainer>, void, ztl::traits::iterator_traits::output_iterator, void, void, void>
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
			public:
				static mySelf make_back_insert_iterator(container_type& target)
				{
					return std::move(mySelf(target));
				}
			};
			template<typename InsertContainer>
			class insert_iterator :
				public virtual ztl::concept::concept_iterator::IOutputIterator<insert_iterator<InsertContainer>, void, ztl::traits::iterator_traits::output_iterator, void, void, void>,
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
					return std::move(mySelf(targetContainer,targetIterator));
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
				typename ztl::traits::iterator_traits::iterator_traits<BidirectionalIterator>::iterator_category,
				typename ztl::traits::iterator_traits::iterator_traits<BidirectionalIterator>::different_type,
				typename ztl::traits::iterator_traits::iterator_traits<BidirectionalIterator>::pointer_type,
				typename ztl::traits::iterator_traits::iterator_traits<BidirectionalIterator>::reference_type
				>
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

				reverse_iterator(mySelf& target) :iterator(target.iterator)
				{

				}
				explicit reverse_iterator(iterator_type&target) :iterator(target)
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
				static mySelf make_reverse_iterator(iterator_type&target)
				{
					return std::move(mySelf(target));
				}
			};
			template<typename ElementType,typename CharType = char,typename CharTraits=std::char_traits<CharType>>
			class istream_iterator:
				public virtual ztl::concept::concept_iterator::IInputIterator<
					istream_iterator<ElementType>,ElementType,ztl::traits::iterator_traits::input_iterator>,
				public virtual ztl::concept::concept_base::IEquality<
					istream_iterator<ElementType>, CharType, CharTraits>
			{
			public:
				typedef ElementType												element_type;
				typedef CharType												char_type;
				typedef CharTraits												traits_type;
				typedef istream_iterator<element_type, char_type,traits_type>	mySelf;
				
				//����Ժ󻻳��Լ�����=.=mark
				typedef basic_istream<char_type, traits_type> istream_type;
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
					/*���������˼,���ָ��ָ����һ����,���Ҵ����л�ȡֵΪ0,��ô�ͽ�streamPtr����Ϊ��,iterator end����nullptr,�������γ���һ����Ч��range*/
					if(streamPtr != 0 && !(*streamPtr >> val))
					{
						streamPtr = nullptr;
					}	
				}
			};
			template<typename ElementType, typename CharType = char, typename CharTraits = std::char_traits<CharType>>
			class ostream_iterator :
				public virtual ztl::concept::concept_iterator::IOutputIterator<
	ostream_iterator<ElementType,CharType,CharTraits>,void,ztl::traits::iterator_traits::output_iterator,void,void,void>
			{
			public:
				typedef ElementType											 element_type;
				typedef CharType											 char_type;
				typedef CharTraits											 traits_type;
				typedef ostream_iterator<element_type,char_type,traits_type> mySelf;
				//����Ժ󻻳��Լ�����=.=mark
				typedef basic_ostream<char_type, traits_type> ostream_type;
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
					if (delim != nullptr)
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
				typename ztl::traits::iterator_traits::iterator_traits<BidirectionalIterator>::iterator_category>
			class move_iterator :
				public virtual ztl::concept::concept_base::IEquality <
				move_iterator < BidirectionalIterator, IteratorCategory >> ,
				public virtual ztl::concept::concept_base::IAssignable <
				move_iterator < BidirectionalIterator, IteratorCategory >> ,
				public virtual ztl::concept::concept_base::IOrdering <
				move_iterator < BidirectionalIterator, IteratorCategory >> ,
				public virtual ztl::concept::concept_iterator::IRandomAcessIterator<
				move_iterator<BidirectionalIterator, IteratorCategory>,
				typename ztl::traits::iterator_traits::iterator_traits<BidirectionalIterator>::value_type,
				typename ztl::traits::iterator_traits::iterator_traits<BidirectionalIterator>::iterator_category,
				typename ztl::traits::iterator_traits::iterator_traits<BidirectionalIterator>::different_type,
				typename ztl::traits::iterator_traits::iterator_traits<BidirectionalIterator>::pointer_type,
				typename ztl::traits::iterator_traits::iterator_traits<BidirectionalIterator>::reference_type
				>
			{
			public:
				typedef move_iterator<BidirectionalIterator, IteratorCategory>					mySelf;
				typedef BidirectionalIterator														iterator_type;
				typedef typename ztl::traits::iterator_traits::iterator_traits<iterator_type>::value_type			value_type;
				typedef typename ztl::traits::iterator_traits::iterator_traits<iterator_type>::pointer_type			pointer_type;
				typedef typename ztl::traits::iterator_traits::iterator_traits<iterator_type>::reference_type		reference_type;
				typedef typename ztl::traits::iterator_traits::iterator_traits<iterator_type>::different_type		different_type;
				typedef typename ztl::traits::iterator_traits::iterator_traits<iterator_type>::iterator_category	iterator_category;

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
					return std::move(*iterator);
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
					return std::move(mySelf(target));
				}
			};
		}
	}

}