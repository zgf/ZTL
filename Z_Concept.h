namespace ztl
{
	namespace concept
	{
		namespace base_concept
		{
			template<typename DerivedType>
			class IAssignable
			{
			public:
				DerivedType& operator=(const DerivedType& target);
			};
		
			template<typename DerivedType>
			class IEquality
			{
			public:
				friend bool operator== (const DerivedType& left, const DerivedType& right);
			public:
				friend bool operator!= (const DerivedType& left, const DerivedType& right)
				{
					return !left == right;
				}
			};
			template<typename DerivedType>
			class IOrdering
			{
			public:
				friend bool operator<  (const DerivedType& left, const DerivedType& right);
			public:
				friend bool operator<= (const DerivedType& left, const DerivedType& right)
				{
					return !left < right;
				}
				friend bool operator>(const DerivedType& left, const DerivedType& right)
				{
					return right < left;
				}
				friend bool operator>= (const DerivedType& left, const DerivedType& right)
				{
					return !right < left;
				}
			};
		/*template<typename Type>
		class Test:public IAssignable<Test<Type>>
		{
		public:
			Type a;
			Test& operator=(Test& target)
			{
				a = 3;
				return target;
			}
		};*/
		}
		namespace iterator_concept
		{
			template<typename IteratorType, typename ValueType, typename IteratorCategory, typename DifferentType = ptrdiff_t, typename PointerType = ValueType*, typename ReferenceType = ValueType&>
			class IIterator :
				public base_concept::IEquality<IteratorType>, base_concept::IAssignable<IteratorType>
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
			template<typename IteratorType, typename ValueType, typename PointerType, typename ReferenceType, typename DifferentType, typename IteratorCategory>
			class IInputIterator :public IIterator<IteratorType, ValueType, IteratorCategory, DifferentType, PointerType, ReferenceType>
			{
			public:

				IteratorType& operator++();
			public:
				void operator++(int)
				{
					++(*this);
				}
			};

			template<typename IteratorType, typename ValueType, typename PointerType, typename ReferenceType, typename DifferentType, typename IteratorCategory>
			class IOutputIterator :public IIterator<IteratorType, ValueType, IteratorCategory, DifferentType, PointerType, ReferenceType>
			{
			public:
				IteratorType& operator++();
			public:
				void operator++(int)
				{
					++(*this);
				}
			};

			template<typename IteratorType, typename ValueType, typename PointerType, typename ReferenceType, typename DifferentType, typename IteratorCategory>
			class IForwardIterator :public IIterator<IteratorType, ValueType, IteratorCategory, DifferentType, PointerType, ReferenceType>
			{
			public:
				IteratorType& operator++();
			public:
				IteratorType operator++(int)
				{
					IteratorType temp(*this);
					++(*this);
					return std::move(temp);
				}
			};
			template<typename IteratorType, typename ValueType, typename PointerType, typename ReferenceType, typename DifferentType, typename IteratorCategory>
			class IBidrectionalIterator :public IForwardIterator< IteratorType, ValueType, PointerType, ReferenceType, DifferentType, IteratorCategory>
			{
			public:
				IteratorType& operator--();
			public:
				IteratorType operator--(int)
				{
					IteratorType temp(*this);
					--(*this);
					return std::move(temp);
				}
			};
			template<typename IteratorType, typename ValueType, typename PointerType, typename ReferenceType, typename DifferentType, typename IteratorCategory>
			class IRandomAcessIterator :
				public IBidrectionalIterator< IteratorType, ValueType, PointerType, ReferenceType, DifferentType, IteratorCategory>,
				base_concept::IOrdering<IteratorType>
			{
			public:
				IteratorType&		operator+=(const different_type n);
				IteratorType&		operator-=(const different_type n);
			public:
				friend IteratorType operator+(const IteratorType& Iter, different_type n)
				{
					IteratorType tempIter = Iter;
					return std::move(tempIter += n);
				}
				friend IteratorType operator+(const different_type n, const IteratorType& Iter)
				{
					return std::move(Iter + n);
				}
				friend IteratorType operator-(const IteratorType& Iter, different_type n)
				{
					IteratorType tempIter = Iter;
					return std::move(tempIter -= n);
				}
				friend IteratorType operator-(const different_type n, const IteratorType& Iter)
				{
					return std::move(Iter - n);
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
			class IForwardContainer :public
			virtual base_concept::IAssignable<IForwardContainer<DerivedType, ValueType,
					ReferenceType, ConstReferenceType, PointerType, ConstPointerType,
					IteratorType, ConstIteratorType,
					DifferentType, SzieType >> , 
			virtual base_concept::IEquality <IForwardContainer <DerivedType, ValueType,
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
			template<typename IteratorType>
			class IRandomAccess
			{
				public:
					iterator_type& operator[](size_type n);
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
			template<typename KeyType,typename IteratorType,typename SizeType,typename ValueType>
			class IAssociate
			{
			public:
				typedef KeyType key_type;
			public:
				IteratorType						find(const key_type& key);
				SizeType							count(const key_type& key);
				pair<IteratorType, IteratorType>	equal_range(const key_type& key);
				IteratorType						insert(const ValueType& Val);
				template<typename InputIterator>
				IteratorType						insert(InputIterator& start, InputIterator& end);
				void								erase(const key_type&key);
				IteratorType						erase(IteratorType& start, IteratorType& end);
			};
			template<typename KeyType, typename IteratorType, typename SizeType, typename ValueType>
			class IUnique
			{
			public:
				pair<IteratorType, bool>	insert(const ValueType& target);
				template<typename InputIterator>
				void						insert(InputIterator& start, InputIterator& end);
			};
			template<typename KeyType, typename IteratorType, typename ValueType>
			class IMultiple
			{
			public:
				IteratorType				insert(const ValueType& target);
				template<typename InputIterator>
				void						insert(InputIterator& start, InputIterator& end);
			};
			template<typename KeyType, typename IteratorType, typename ValueType>
			class ISimple
			{
			public:
				
			};
			template<typename KeyType, typename IteratorType, typename ValueType = pair<const KeyType,ValueType>,typename MapType>
			class IPair
			{
			public:
				typedef MapType mapped_type;
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
			template<typename HasherType, typename KeyCompare,typename SizeType>
			class IHash
			{
			public:
				typedef KeyCompare key_compare;
				typedef HasherType hasher;
			public:
				hasher hash_func()const;
				void resize();
				key_compare key_comp()const;
				SizeType bucket_count()const;
			};
		}
	}
	
}