#pragma once
#include "../ztl_iterator.h"
#include "../log/ztl_log.h"
namespace ztl
{
	template<typename ValueType>
	class deque_iterator
	{
	public:
		/*IIteratorDefine*/
		typedef ValueType											value_type;
		typedef const value_type									const_value_type;
		typedef value_type*											pointer_type;
		typedef const pointer_type									const_poiner_type;
		typedef value_type&											reference_type;
		typedef const reference_type								const_reference_type;
		typedef std::ptrdiff_t											different_type;
		typedef ztl::random_acess_iterator							iterator_category;
		typedef deque_iterator<value_type>							self_type;
	public:
		deque_iterator() :ptr(nullptr)
		{
		}
		deque_iterator(const pointer_type& target) :ptr(target)
		{
		}
		deque_iterator(pointer_type&& target) :ptr(nullptr)
		{
			ptr = target;
			target = nullptr;
		}

		deque_iterator(const self_type& target) :ptr(target.ptr)
		{

		}
		deque_iterator(self_type&& target) :ptr(nullptr)
		{
			ptr = target.ptr;
			target.ptr = nullptr;
		}
		deque_iterator(nullptr_t) :ptr(nullptr)
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
		const_poiner_type operator->()const
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
		different_type	operator-(const self_type& target) const
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
			return !left < right;
		}
		friend bool operator>(const self_type& left, const self_type& right)
		{
			return right < left;
		}
		friend bool operator>= (const self_type& left, const self_type& right)
		{
			return !right < left;
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
	public:/*deque_iterator*/
		explicit operator pointer_type()
		{
			return ptr;
		}
	public:/*firend class declaration*/
		template<typename CharType, typename CharTraits>
		friend class base_string;
	private:
		pointer_type	ptr;//pointer_type to deque element;
	};

	/*双向增长的内存块,适合高效前端和后端插入和遍历*/
	template<typename ValueType>
	class deque
	{
	public:/*IContainerDefine*/
		typedef ValueType					value_type;
		typedef deque<value_type>			self_type;
		typedef deque_iterator<value_type>	iterator_type;
		typedef const value_type			const_value_type;
		typedef value_type&					reference_type;
		typedef const_value_type&			const_reference_type;
		typedef value_type*					pointer_type;
		typedef const pointer_type			const_pointer_type;
		typedef const iterator_type			const_iterator_type;
		typedef typename ztl::iterator_traits<iterator_type> ::different_type		different_type;
		typedef typename std::make_unsigned<different_type>::type 					size_type;//size_type
	public:	/*IReverseDefine*/
		typedef ztl::reverse_iterator<iterator_type>								reverse_iterator_type;
		typedef const reverse_iterator_type											const_reverse_iterator_type;
	public:/*六大函数（构造函数，移动构造函数，移动赋值操作符，复制构造函数，赋值操作符，析构函数）*/
		deque()
		{
			/*构造时预分配16字节内存*/
			begin_ptr = get_uninit_block(16);
			end_ptr = begin_ptr + 16;
			first_ptr = last_ptr = begin_ptr + 4;
		}
		deque(self_type&& target) :first_ptr(target.first_ptr), last_ptr(target.last_ptr), end_ptr(target.end_ptr), begin_ptr(target.begin_ptr)
		{
			target.begin_ptr = nullptr;
			target.first_ptr = nullptr;
			target.last_ptr = nullptr;
			target.end_ptr = nullptr;
		}
		deque(const self_type& target)
		{
			auto count = target.end_ptr - target.begin_ptr;
			begin_ptr = get_uninit_block(count*element_size());
			auto offset = target.first_ptr - target.begin_ptr;
			first_ptr = begin_ptr + offset;
			last_ptr = uninitialized_copy(target.first_ptr, target.last_ptr, first_ptr);
			end_ptr = begin_ptr + count;
		}
		deque(size_type n) :deque(n, value_type())
		{
		}
		deque(size_type n, const const_value_type& value)
		{
			begin_ptr = first_ptr = get_uninit_block(n);
			end_ptr = last_ptr = ztl::uninitialized_fill_n(first_ptr, n, value);
		}
		template<typename InputIterator, class = typename std::enable_if<ztl::is_iterator<InputIterator>::value,
			void>::type>
			deque(InputIterator first, InputIterator last)
		{
				begin_ptr = first_ptr = get_uninit_block(ztl::distance(first, last));
				end_ptr = last_ptr = uninitialized_copy(first, last, first_ptr);
		}
		deque(std::initializer_list<value_type> target)
		{
			begin_ptr = first_ptr = get_uninit_block(target.size());
			end_ptr = last_ptr = uninitialized_copy(target.begin(), target.end(), first_ptr);
		}
		~deque()
		{
			destory_and_reclaim();
		}

		self_type& operator=(self_type&& target)
		{
			end_ptr = target.end_ptr;
			first_ptr = target.first_ptr;
			last_ptr = target.last_ptr;
			begin_ptr = target.begin_ptr;
			target.end_ptr = nullptr;
			target.first_ptr = nullptr;
			target.last_ptr = nullptr;
			target.begin_ptr = nullptr;
			return *this;
		}
		self_type& operator=(const self_type& target)
		{
			destory(first_ptr, end_ptr);
			resize_uninit_block(target.size(), target.begin_ptr - target.first_ptr);
			last_ptr = uninitialized_copy(target.first_ptr, target.last_ptr, first_ptr);
			return *this;
		}
		self_type& operator=(std::initializer_list<value_type> target)
		{
			destory(first_ptr, end_ptr);
			resize_uninit_block(target.size(), target.begin_ptr - target.first_ptr);
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
			//内存重分配,迭代器失效
			
			emplace_back(forward<FuncArgs>(Args)...);
			auto old_end = end() - sizeof...(FuncArgs);
			//[开始位置,中间位置,结束位置)
			//区间1[开始位置,中间位置),区间2[中间位置,结束位置)

			return ztl::rotate(begin() + offset, old_end, end());
		}
		iterator_type	insert(const iterator_type& Where, value_type&& Val)
		{
			auto offset = Where - begin();
			if(capacity() < size() + 1)
			{
				reserve(front_capacity(), 2 * (size() + back_capacity()) + 1);
			}
			return emplace(begin() + offset, forward<value_type&&>(Val));
		}
		iterator_type	insert(const iterator_type& Where, const value_type& Val)
		{
			auto offset = Where - begin();
			if(capacity() < size() + 1)
			{
				reserve(front_capacity(), 2 * (size() + back_capacity()) + 1);
			}
			return emplace(begin() + offset, Val);
		}
		iterator_type	insert(const iterator_type& Where, different_type n, const_value_type& Val)
		{
			auto offset = ztl::distance(begin(), Where);
			if(capacity() < size() + n)
			{
				reserve(front_capacity(), 2 * (size() + back_capacity()) + n);
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
					reserve(front_capacity(), 2 * (size() + back_capacity()) + distance(first, last));
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
			cout << this->first_ptr << endl;

			rotate(first, last, end());
			cout << this->first_ptr << endl;
			pop_back_n(distance(first, last));
			cout << this->first_ptr << endl;
		}
		void			resize(const size_type n, const value_type& value)
		{

			if(n > capacity())
			{
				reserve(n, n);
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
			ztl_assert(size() > 0, "error vector size < 0!");
			destory(last_ptr - 1);
			--last_ptr;
		}
		void			push_back(value_type&& Val)
		{
			if(last_ptr == end_ptr)
			{
				reserve(front_capacity(), 2 * (size() + back_capacity()) + 1);
			}
			construct(last_ptr, ztl::forward<value_type&&>(Val));
			++last_ptr;
		}
		void			push_back(const value_type& Val)
		{
			if(last_ptr == end_ptr)
			{
				reserve(front_capacity(), 2 * (size() + back_capacity()) + 1);
			}
			construct(last_ptr, Val);
			++last_ptr;
		}
		void pop_back_n(size_type n)
		{
			ztl::ztl_assert(n <= size(), "pop_back number > size");
			destory(end() - n, end());
			last_ptr -= n;
		}
		template<typename... FuncArgs>
		void			emplace_back(FuncArgs&&... Args)
		{
			if(capacity() < size() + (int)sizeof...(FuncArgs))
			{
				reserve(front_capacity(), 2 * (size() + back_capacity()) + sizeof...(FuncArgs));
			}
			ztl::construct(last_ptr, ztl::forward<FuncArgs>(Args)...);
			last_ptr += sizeof...(FuncArgs);
		}
	public:	/*IFrontInsert*/
		reference_type front()
		{
			return *first_ptr;
		}
		const_reference_type front()const
		{
			return *first_ptr;
		}
		void			push_front(value_type&& value)
		{
			if(first_ptr == begin_ptr)
			{
				reserve(2 * (front_capacity() + size()), back_capacity());
			}
			construct(--first_ptr, ztl::forward<value_type&&>(value));
		}
		void			push_front(const value_type& value)
		{
			if(first_ptr == begin_ptr)
			{
				reserve(2 * (front_capacity() + size()), back_capacity());
			}
			construct(--first_ptr, value);
		}
		void			pop_front()
		{
			ztl_assert(size() > 0, "error vector size < 0!");
			destory(first_ptr);
			first_ptr++;
		}
		void pop_front_n(size_type n)
		{
			ztl::ztl_assert(n <= size(), "pop_back number > size");
			first_ptr -= n;
			destory(begin(), begin() + n);
			
		}
		template<typename... FuncArgs>
		void			emplace_front(FuncArgs&&... Args)
		{
			if(capacity() < size() + (int)sizeof...(FuncArgs))
			{
				reserve(2 * (size() + front_capacity()) + sizeof...(FuncArgs), back_capacity());
			}
			ztl::construct(first_ptr -= sizeof...(FuncArgs), ztl::forward<FuncArgs>(Args)...);
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
	public:	/*vactor special API wrapper*/
		void shrink_to_fit()
		{
			auto new_begin_ptr = get_uninit_block(size());
			auto new_last_ptr = uninitialized_copy(first_ptr, last_ptr, new_begin_ptr);
			destory_and_reclaim();
			begin_ptr = first_ptr = ztl::move(new_begin_ptr);
			end_ptr = last_ptr = ztl::move(new_last_ptr);
		}

		size_type 		capacity()
		{
			return end_ptr - begin_ptr;
		}
		pointer_type	data()
		{
			return first_ptr;
		}
		const_pointer_type data() const
		{
			return const_cast<self_type*>(this)->data();
		}


	private: /*detail function*/
		size_t element_size()
		{
			return sizeof(value_type);
		}
		//调整容量,如果大于当前容量,重分配,否则,什么也不做
		void reserve(const size_type first_count,const size_type last_count)
		{
			if(first_count > first_ptr - begin_ptr || last_count > last_ptr - end_ptr)
			{
				//cout << first_ptr << endl;
				//cout << back_capacity() << endl;
				pointer_type new_begin = get_uninit_block(first_count + last_count + size());
				pointer_type new_first = new_begin + first_count;
				pointer_type new_last = uninitialized_copy(this->first_ptr, this->last_ptr, new_first);
				destory_and_reclaim();
				this->begin_ptr = std::move(new_begin);
				this->first_ptr = std::move(new_first);
				this->last_ptr = std::move(new_last);
				this->end_ptr = new_last + last_count;
				//cout << first_ptr << endl;
				//cout << back_capacity()<<endl;
			}
		}
		size_type front_capacity()
		{
			return first_ptr - begin_ptr;
		}
		size_type back_capacity()
		{
			return end_ptr - last_ptr;
		}
	private:/*memory management*/
		pointer_type get_uninit_block(size_type number)
		{
			return ztl::allocator<value_type>::allocate(number);
		}
		void resize_uninit_block(size_type number, size_type first_offset)
		{
			if(capacity() < number)
			{
				destory_and_reclaim();
				begin_ptr = get_uninit_block(number);
				end_ptr = begin_ptr + number;
				first_ptr = begin_ptr + first_offset;
			}
		}
		void destory_and_reclaim()
		{
			destory(first_ptr, last_ptr);
			ztl::allocator<value_type>::deallocate(begin_ptr, (end_ptr - begin_ptr)*element_size());
		}
	private:
		pointer_type	begin_ptr;
		pointer_type	first_ptr;
		pointer_type	last_ptr;
		pointer_type	end_ptr;
	};
}