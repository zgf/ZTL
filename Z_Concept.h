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
				typedef ValueType value_type;
				typedef PointerType pointer_type;
				typedef ReferenceType reference_type;
				typedef DifferentType different_type;
				typedef IteratorCategory iterator_category;
			public:
				reference_type operator*();
			public:
				pointer_type operator->()
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
				IteratorType& operator+=(const different_type n);
				IteratorType& operator-=(const different_type n);
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
			template<typename T>
			class IContainer:public base_concept::IAssignable<IContainer<T>>
			{

			};
		}
	}
	
}