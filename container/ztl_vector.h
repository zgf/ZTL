#pragma once
#include "../memory/ztl_allocator.h"
#include "../concept/ztl_container_concept.h"
#include "../ztl_iterator.h"
namespace ztl
{
	/*自动递增内存块,当内存重分配时,会导致迭代器失效*/
	template<typename ValueType>
	class vector_iterator //:public ztl::IRandomAcessIterator<vector_iterator<ValueType>, ValueType, ztl::random_acess_iterator>
	{
	public:
		/*IIteratorDefine*/
		typedef ValueType				value_type;
		typedef const value_type		const_value_type;
		typedef value_type*				pointer_type;
		typedef const pointer_type		const_pointer_type;
		typedef value_type&				reference_type;
		typedef const reference_type	const_reference_type;
		typedef typename iterator_traits<pointer_type>::different_type		different_type;
		typedef random_acess_iterator		iterator_category;
		typedef vector_iterator<value_type>			self_type;
	public:
		vector_iterator() :ptr(nullptr)
		{

		}
		vector_iterator(const self_type &target) :ptr(target.ptr)
		{
		}
		vector_iterator(self_type&& target) :ptr(std::move(target.ptr))
		{
		}
		explicit vector_iterator(const pointer_type& target) :ptr(target)
		{

		}
		explicit vector_iterator(pointer_type&& target) :ptr(std::move(target))
		{

		}
		vector_iterator(nullptr_t) :ptr(nullptr)
		{
		}
	public:/*IIterator*/
		const_reference_type operator*()const
		{
			return *ptr;
		}
		reference_type operator*()
		{
			return *ptr;
		}
		const_pointer_type operator->()const
		{
			return &*this;
		}
		pointer_type operator->()
		{
			return &*this;
		}
	public:/*IForwardIter*/
		self_type& operator++()
		{
			++ptr;
			return *this;
		}
		self_type operator++(int)
		{
			self_type temp(*this);
			++(*this);
			return ztl::move(temp);
		}
	public:/*IBidrectionalIterator*/
		self_type& operator--()
		{
			--ptr;
			return *this;
		}
		self_type operator--(int)
		{
			self_type temp(*this);
			--(*this);
			return ztl::move(temp);
		}
	public:/*IRandomAcessIterator*/
		self_type&		operator+=(const different_type n)
		{
			ptr += n;
			return *this;
		}
		self_type&		operator-=(const different_type n)
		{
			ptr -= n;
			return *this;
		}
		different_type	operator-(const self_type& target)const
		{
			return ptr - target.ptr;
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
		/*friend different_type operator-(const self_type& left, const self_type& right)
		{
			return left.ptr - right.ptr;
		}*/
		friend self_type operator-(const different_type n, const self_type& Iter)
		{
			return ztl::move(Iter - n);
		}
		reference_type operator[](const different_type n)
		{
			return *(this += n);
		}

	public: /*IOrdering*/
		bool operator<  (const self_type& right)const
		{
			return ptr < right.ptr;
		}
		friend bool operator<= (const self_type& left, const self_type& right)
		{
			return !(left < right);
		}
		friend bool operator>(const self_type& left, const self_type& right)
		{
			return right < left;
		}
		friend bool operator>= (const self_type& left, const self_type& right)
		{
			return !(right < left);
		}
	public:/*IEquality*/

		bool operator==(const self_type& target)const
		{
			return target.ptr == ptr;
		}
		friend bool operator!= (const self_type& left, const self_type& right)
		{
			return !(left == right);
		}
	public:/*vector_iter特有*/
		explicit operator pointer_type()
		{
			return ptr;
		}
	private:
		pointer_type	ptr;//pointer to vector element;
	};
	template<typename ValueType>
	class vector
	{
	public:/*IContainerTypeDefine*/
		typedef ValueType					value_type;
		typedef const value_type				const_value_type;
		typedef value_type&				reference_type;
		typedef const_value_type&			const_reference_type;
		typedef value_type*					pointer_type;
		typedef const pointer_type			const_pointer_type;
		typedef vector_iterator<value_type>	iterator_type;
		typedef const iterator_type			const_iterator_type;
		typedef typename ztl::iterator_traits<vector_iterator<value_type>>::different_type			different_type;
		typedef different_type					size_type;//size_type can represent anynon - negative value of difference_type
		typedef vector<value_type>				self_type;
	public:	/*IReverseDefine*/
		typedef  ztl::reverse_iterator<iterator_type> reverse_iterator_type;
		typedef const  reverse_iterator_type const_reverse_iterator_type;

	public:/*六大函数（构造函数，移动构造函数，移动赋值操作符，复制构造函数，赋值操作符，析构函数）*/
		vector()
		{
			/*构造时预分配16字节内存*/
			first_ptr = last_ptr = get_uninit_block(16);
			end_ptr = first_ptr + 16;
		}
		vector(self_type&& target) :first_ptr(target.first_ptr), last_ptr(target.last_ptr), end_ptr(target.end_ptr)
		{
			target.first_ptr = nullptr;
			target.last_ptr = nullptr;
			target.end_ptr = nullptr;
		}
		vector(const self_type& target)
		{
			auto count = target.end_ptr - first_ptr;
			first_ptr = get_uninit_block(count*element_size());
			last_ptr = uninitialized_copy(target.first_ptr, target.last_ptr, first_ptr);
			end_ptr = first_ptr + count;
		}
		vector(size_type n) :vector(n, value_type())
		{
		}
		vector(size_type n, const const_value_type& value)
		{
			first_ptr = get_uninit_block(n);
			last_ptr = ztl::uninitialized_fill_n(first_ptr, n, value);
			end_ptr = first_ptr + n;
		}
		template<typename InputIterator, class = typename std::enable_if<ztl::is_iterator<InputIterator>::value,
			void>::type>
			vector(InputIterator first, InputIterator last)
		{
				first_ptr = get_uninit_block(ztl::distance(first, last));
				end_ptr = last_ptr = uninitialized_copy(first, last, first_ptr);
		}
		vector(std::initializer_list<value_type> target)
		{
			first_ptr = get_uninit_block(target.size());
			end_ptr = last_ptr = uninitialized_copy(target.begin(), target.end(), first_ptr);
		}
		~vector()
		{
			destory_and_reclaim();
		}

		self_type& operator=(self_type&& target)
		{
			end_ptr = ztl::move(target.end_ptr);
			first_ptr = ztl::move(target.first_ptr);
			last_ptr = ztl::move(target.last_ptr);
			target.end_ptr = nullptr;
			target.first_ptr = nullptr;
			target.last_ptr = nullptr;
			return *this;
		}
		self_type& operator=(const self_type& target)
		{
			destory(first_ptr, end_ptr);
			resize_uninit_block(target.size()+1);
			last_ptr = uninitialized_copy(target.first_ptr, target.last_ptr,first_ptr);
			return *this;
		}
		self_type& operator=(std::initializer_list<value_type> target)
		{	
			destory(first_ptr, end_ptr);
			resize_uninit_block(target.size());
			last_ptr = uninitialized_copy(target.first_ptr, target.last_ptr, first_ptr);
			return *this;
		}

	public:	/*IContainer*/
		iterator_type			begin()
		{
			return iterator_type(first_ptr);
		}
		iterator_type			end()
		{
			return iterator_type(last_ptr);
		}
		void					clear()
		{
			destory(first_ptr, last_ptr);
			last_ptr = first_ptr;
		}

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
			return  ztl::distance(cbegin(), cend());
		}
		size_type				max_size()	const
		{
			return -1;
		}
		bool					empty()		const
		{
			return cbegin() == cend();
		}
		void swap(self_type&& target)
		{
			ztl::swap(ztl::forward<self_type>(target), ztl::forward<self_type>(*this));
		}
		void swap(self_type& target)
		{
			ztl::swap(target, *this);
		}
	public:/*IEquality*/
		bool operator== (const self_type& right) const
		{
			return size() == right.size() && ztl::equal(cbegin(), cend(), right.cbegin());
		}
		friend bool operator!= (const self_type& left, const self_type& right)
		{
			return !(left == right);
		}
	public:
		/*IReverse*/
		reverse_iterator_type			rbegin()
		{
			return reverse_iterator_type(end());
		}
		reverse_iterator_type			rend()
		{
			return reverse_iterator_type(begin());
		}
		const_reverse_iterator_type		crbegin()	const
		{
			return const_cast<self_type*>(this)->rbegin();
		}
		const_reverse_iterator_type		crend()		const
		{
			return const_cast<self_type*>(this)->rend();
		}
	public:	/*ISequenceContainer*/
		template<typename... FuncArgs>
		iterator_type	emplace(const iterator_type& Where, FuncArgs&&... Args)
		{
			auto offset = Where - begin();
			auto old_end = end();
			emplace_back(forward<FuncArgs>(Args)...);
			auto old_end = end()- sizeof...(FuncArgs);

			//[开始位置,中间位置,结束位置)
			//区间1[开始位置,中间位置),区间2[中间位置,结束位置)

			return ztl::rotate(begin() + offset, old_end, end());
		}
		iterator_type	insert(const iterator_type& Where, value_type&& Val)
		{
			auto offset = Where - begin();
			if(capacity() < size() + 1)
			{
				reserve(2 * capacity());
			}
			return emplace(begin() + offset, forward<value_type&&>(Val));
		}
		iterator_type	insert(const iterator_type& Where, const value_type& Val)
		{
			auto offset = Where - begin();
			if(capacity() < size() + 1)
			{
				reserve(2 * capacity());
			}
			return emplace(begin() + offset, Val);
		}
		iterator_type	insert(const iterator_type& Where, different_type n, const_value_type& Val)
		{
			auto offset = ztl::distance(begin(), Where);
			if(capacity() < size() + n)
			{
				reserve(2 * capacity() + n);
			}
			auto old_end = end();
			last_ptr = uninitialized_fill_n(last_ptr, n, Val);
			return ztl::rotate(begin() + offset, old_end, end());
		}
		template<typename InputIterator, class = typename std::enable_if<ztl::is_iterator<InputIterator>::value,
			void>::type>
			iterator_type	insert(const iterator_type& Where, InputIterator first, InputIterator last)
		{
				auto offset = ztl::distance(begin(), Where);
				if(capacity() < this->size() + distance(first, last))
				{
					reserve(2 * capacity() + distance(first, last));
				}
				auto old_end = end();
				last_ptr = uninitialized_copy(first, last, last_ptr);
				return 	ztl::rotate(begin() + offset, old_end, end());
		}
		iterator_type	insert(const iterator_type& Where, std::initializer_list<value_type> target)
		{
			return insert(Where, target.begin(), target.end());
		}
		void			erase(const iterator_type& Where)
		{
			rotate(Where, Where + 1, end());
			pop_back();
		}
		void			erase(const iterator_type& first, const iterator_type& last)
		{
			//cout << this->first_ptr<<endl;

			rotate(first, last, end());
			//cout << this->first_ptr<<endl;
			pop_back_n(distance(first, last));
			//cout << this->first_ptr << endl;
		}
		void			resize(const size_type n, const value_type& value)
		{
			
			if(n > capacity())
			{
				reserve(2 * n);
				last_ptr = uninitialized_fill_n(last_ptr, n - size(), value);
			}
			else if(n > size())
			{
				last_ptr = uninitialized_fill_n(last_ptr, n - size(), value);
			}
			else
			{
				destory(first_ptr + n, last_ptr);
				last_ptr = first_ptr + n;
			}
		}
		void			resize(const size_type n)
		{
			resize(n, value_type());
		}
		template<typename InputIterator, class = typename std::enable_if<ztl::is_iterator<InputIterator>::value,
			void>::type>
			void			assign(InputIterator first, InputIterator last)
		{
				self_type temp(first, last);
				swap(ztl::move(temp));
		}
		template<typename ContainerType>
		void			assign(ContainerType container)
		{
			assign(container.begin(), container.end());
		}
		void			assign(const different_type n, const_value_type& target)
		{
			self_type temp(n, target);
			swap(ztl::move(temp));
		}
	public:/*IRandomAccess*/
		reference_type operator[](const size_type n)
		{
			return  *(begin() + n);
		}
		const_reference_type operator[](const size_type n) const
		{
			return  *(begin() + n);
		}
		reference_type at(const size_type n)
		{
			ztl::ztl_assert(n >= 0 && n < size(), "error!");
			return  *(begin() + n);
		}
		const_reference_type at(const size_type n) const
		{
			ztl::ztl_assert(n >= 0 && n < size(), "error!");
			return  *(begin() + n);
		}
	public:	/*IBackInsert*/
		reference_type	back()
		{
			return *(last_ptr - 1);
		}
		const_reference_type	back()const
		{
			return const_cast<self_type*>(this)->back();
		}
		void			pop_back()
		{
			ztl_assert(size() > 0, "error vector size <= 0!");
			destory(last_ptr - 1);
			--last_ptr;
		}
		void			push_back(value_type&& Val)
		{
			if(last_ptr == end_ptr)
			{
				reserve(2 * capacity());
			}
			construct(last_ptr, ztl::forward<value_type&&>(Val));
			++last_ptr;
		}
		void			push_back(const value_type& Val)
		{
			if(last_ptr == end_ptr)
			{
				reserve(2 * capacity());
			}
			construct(last_ptr, Val);
			++last_ptr;
		}
		template<typename... FuncArgs>
		void			emplace_back(FuncArgs&&... Args)
		{
			if(capacity() < size() + (int)sizeof...(FuncArgs))
			{
				reserve(2 * capacity() + sizeof...(FuncArgs));
			}
		
			ztl::construct(last_ptr, ztl::forward<FuncArgs>(Args)...);
			last_ptr += sizeof...(FuncArgs);
		}
		
	public:	/*vactor special API*/
		void shrink_to_fit()
		{
			auto new_first_ptr = get_uninit_block(size());
			auto new_last_ptr = uninitialized_copy(first_ptr, last_ptr, new_first_ptr);
			this->destory_and_reclaim();
			this->first_ptr = ztl::move(new_first_ptr);
			this->last_ptr = ztl::move(new_last_ptr);
			this->end_ptr = last_ptr;
		}
		reference_type front()
		{
			return *first_ptr;
		}
		const_reference_type front()const
		{
			return *first_ptr;
		}
		size_type 		capacity()
		{
			return end_ptr - first_ptr;
		}
		pointer_type	data()
		{
			return first_ptr;
		}
		const_pointer_type data() const
		{
			return const_cast<self_type*>(this)->data();
		}
		//调整容量
		void reserve(size_type count)
		{
			if (count > capacity())
			{
				pointer_type new_first = get_uninit_block(count);
				pointer_type new_last = uninitialized_copy(this->first_ptr, this->last_ptr, new_first);
				destory_and_reclaim();
				this->first_ptr = new_first;
				this->last_ptr = new_last;
				this->end_ptr = new_first + count;
			}
		}
		void pop_back_n(size_type n)
		{
			ztl::ztl_assert(n <= size(), "pop_back number > size");
			destory(end() - n, end());
			last_ptr -= n;
		}
	private: /*detail function*/
		size_t element_size()
		{
			return sizeof(value_type);
		}
	private:/*memory management*/
		pointer_type get_uninit_block(size_type number)
		{
			return ztl::allocator<value_type>::allocate(number);
		}
		void resize_uninit_block(size_type number)
		{
			if(capacity() < number)
			{
				destory_and_reclaim();
				first_ptr = get_uninit_block(number);
				end_ptr = first_ptr + number;
			}
		}
		void destory_and_reclaim()
		{
			destory(first_ptr, last_ptr);
			ztl::allocator<value_type>::deallocate(first_ptr, (end_ptr - first_ptr)*element_size());
		}

	private:
		pointer_type first_ptr;
		pointer_type last_ptr;
		pointer_type end_ptr;
	};
}