#pragma once
#include "ztl_char_traits.h"
#include "../memory/ztl_allocator.h"
#include "../iterator/iterator_traits.h"
#include "ztl_vector.h"
#include <iostream>
namespace ztl
{
	/*包装一层vector的迭代器*/
	template<typename ValueType>
	class string_iterator
	{
	public:
		/*IIteratorDefine*/
		typedef vector_iterator<ValueType>							vector_iterator_type;
		typedef typename vector_iterator_type::value_type			value_type;
		typedef typename vector_iterator_type::const_value_type		const_value_type;
		typedef typename vector_iterator_type::pointer_type			pointer_type;
		typedef typename vector_iterator_type::const_pointer_type	const_poiner_type;
		typedef typename vector_iterator_type::reference_type		reference_type;
		typedef typename vector_iterator_type::const_reference_type	const_reference_type;
		typedef typename vector_iterator_type::different_type		different_type;
		typedef typename vector_iterator_type::iterator_category	iterator_category;
		typedef string_iterator<ValueType>							self_type;
	public:
		string_iterator() :ptr(nullptr)
		{
		}
		string_iterator(const pointer_type& target) :ptr(target)
		{
		}
		string_iterator(pointer_type&& target) :ptr(target)
		{
		}
		string_iterator(const vector_iterator<value_type>& target) :ptr(target)
		{
		}
		string_iterator(vector_iterator<value_type>&& target) :ptr(target)
		{
		}
		string_iterator(const self_type& target) :ptr(target.ptr)
		{
		}
		string_iterator(self_type&& target) :ptr(std::move(target.ptr))
		{
		}
		string_iterator(nullptr_t) :ptr(nullptr)
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
	public:/*string_iterator*/
		explicit operator pointer_type()
		{
			return ptr.ptr;
		}
	public:/*firend class declaration*/
		template<typename CharType, typename CharTraits>
		friend class base_string;
	private:
		vector_iterator_type	ptr;//pointer_type to vector element;
	};
	/*字符序列式容器*/
	template<typename CharType, typename CharTraits>
	class base_string
	{
	public:/*base_string special define*/
		typedef typename CharTraits::char_type				  char_type;
		typedef CharTraits									  char_traits_type;
		typedef ztl::allocator<char_type>					  allocator_type;
	public:/*IContainerDefine*/
		typedef char_type					value_type;
		typedef const value_type			const_value_type;
		typedef value_type&					reference_type;
		typedef const_value_type&			const_reference_type;
		typedef value_type*					pointer_type;
		typedef const pointer_type			const_pointer_type;
		typedef string_iterator<value_type>	iterator_type;
		typedef const iterator_type			const_iterator_type;
		typedef typename ztl::iterator_traits <iterator_type> ::different_type			different_type;
		typedef typename std::make_unsigned<different_type>::type 					size_type;//size_type can represent anynon - negative value of difference_type
		typedef base_string<value_type, char_traits_type>				self_type;
	public:	/*IReverseDefine*/
		typedef  ztl::reverse_iterator<iterator_type> reverse_iterator_type;
		typedef const  reverse_iterator_type const_reverse_iterator_type;

	public:/*六大函数（构造函数，移动构造函数，移动赋值操作符，复制构造函数，赋值操作符，析构函数）*/

		//construct by empty base_string
		base_string() //:sequence(1,0)
		{
			check_end_character();

			//sequence.push_back(0);
		}

		//construct by move target base_string
		base_string(self_type&& target) :sequence(ztl::forward<vector<char_type>>(target.sequence))
		{

		}
		//construct by target base_string

		base_string(const self_type& target) :sequence(target.sequence)
		{

		}

		template<typename InputIterator, class = typename std::std::enable_if<ztl::is_iterator<InputIterator>::value,
			void>::type>
			base_string(InputIterator first, InputIterator last) : sequence(first, last)
		{
				check_end_character();
		}
		base_string(const_iterator_type first, const_iterator_type last) : sequence(first, last)
		{
			check_end_character();
		}
		base_string(std::initializer_list<value_type> target) :sequence(target)
		{
			check_end_character();
		}

		base_string(const self_type& right, size_type right_offset, size_type count)
			: sequence(right.cbegin() + right_offset, count == -1 ? right.cend() : right.cbegin() + right_offset + count)
		{	// construct from right [right_offset, right_offset + count)
			check_end_character();
		}
		base_string(const self_type& right, size_type count) :sequence(right.begin(), right.begin() + count)
		{	// construct from right [right,right + count)
			check_end_character();
		}
		base_string(const char_type* right,
			size_type right_offset,
			size_type count)
			: sequence(right + right_offset, count == -1 ? right + right_offset + count : right + char_traits_type::length(right))
		{	// construct from right [right_offset, right_offset + count)
			check_end_character();
		}

		base_string(const char_type *Ptr, size_type count) :sequence(Ptr, Ptr + count)
		{	// construct from [Ptr, Ptr + count)
			check_end_character();
		}

		base_string(const char_type *Ptr)
			: sequence(Ptr, Ptr + char_traits_type::length(Ptr))
		{	// construct from [Ptr, <null>)
			check_end_character();
		}

		base_string(size_type count, char_type value)
			: sequence(count + 1, value)
		{	// construct from count * value
			sequence.back() = 0;
		}

		~base_string()
		{
		}
		self_type& operator=(self_type&& target)
		{
			sequence = ztl::forward<vector<char_type>>(target.sequence);
			return *this;
		}
		self_type& operator=(const self_type& target)
		{
			sequence = target.sequence;
			return *this;
		}
		self_type& operator=(std::initializer_list<char_type> Ilist)
		{	// assign initializer_list
			assign(Ilist.begin(), Ilist.end());
			return *this;
		}
		self_type& operator=(const char_type *Ptr)
		{	// assign [Ptr, <null>)
			sequence.assign(Ptr, Ptr + char_traits_type::length(Ptr));
			check_end_character();
			return *this;
		}

		self_type& operator=(char_type value)
		{	// assign 1 * value
			sequence.assign(1, value);
			check_end_character();
			return *this;
		}
	public:/*IContainer*/
		iterator_type			begin()
		{
			return pointer_type(sequence.begin());
		}
		iterator_type			end()
		{
			return pointer_type(sequence.end() - 1);
		}
		void					clear()
		{
			sequence.clear();
		}

		const_iterator_type		cbegin()	const
		{
			return  sequence.cbegin();
		}
		const_iterator_type		cend()		const
		{
			return sequence.cend() - 1;
		}
		size_type				size()		const
		{
			return cend() - cbegin();
		}
		size_type	max_size()	const
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
		friend bool operator==(const char_type * left, const self_type& right)
		{	// test for NTCS vs. string equality
			return (right.compare(left) == 0);
		}
		friend bool operator==(const self_type& left, const char_type *right)
		{	// test for string vs. NTCS equality
			return (left.compare(right) == 0);
		}

		friend bool operator== (const self_type& left, const self_type& right)
		{
			return left.size() == right.size() && ztl::equal(left.cbegin(), left.cend(), right.cbegin());
		}
		friend bool operator!=(const self_type& left, const self_type& right)
		{	// test for string inequality
			return (!(left == right));
		}
		friend bool operator!=(const char_type *left, const self_type& right)
		{	// test for NTCS vs. string inequality
			return (!(left == right));
		}
		friend	bool operator!=(const self_type& left, const char_type *right)
		{	// test for string vs. NTCS inequality
			return (!(left == right));
		}
	public:/*IOrdering*/
		friend bool operator<(const self_type& left, const self_type& right)
		{	// test if string < string
			return (left.compare(right) < 0);
		}

		friend bool operator<(const char_type * left, const self_type& right)
		{	// test if NTCS < string
			return (right.compare(left) > 0);
		}

		friend bool operator<(const self_type& left, const char_type *right)
		{	// test if string < NTCS
			return (left.compare(right) < 0);
		}

		friend bool operator>(const self_type& left, const self_type& right)
		{	// test if string > string
			return (right < left);
		}

		friend bool operator>(const char_type * left, const self_type& right)
		{	// test if NTCS > string
			return (right < left);
		}

		friend bool operator>(const self_type& left, const char_type *right)
		{	// test if string > NTCS
			return (right < left);
		}

		friend bool operator<=(const self_type& left, const self_type& right)
		{	// test if string <= string
			return (!(right < left));
		}

		friend bool operator<=(const char_type * left, const self_type& right)
		{	// test if NTCS <= string
			return (!(right < left));
		}
		friend bool operator<=(const self_type& left, const char_type *right)
		{	// test if string <= NTCS
			return (!(right < left));
		}

		friend bool operator>=(const self_type& left, const self_type& right)
		{	// test if string >= string
			return (!(left < right));
		}

		friend bool operator>=(const char_type * left, const self_type& right)
		{	// test if NTCS >= string
			return (!(left < right));
		}

		friend bool operator>=(const self_type& left, const char_type *right)
		{	// test if string >= NTCS
			return (!(left < right));
		}

	public:/*IReverse*/
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
	public:/*IRandomAccess*/
		reference_type operator[](const size_type n)
		{
			return  *(sequence.begin() + n);
		}
		const_reference_type operator[](const size_type n) const
		{
			return  *(sequence.begin() + n);
		}
		reference_type at(const size_type n)
		{
			ztl::ztl_assert(n >= 0 && n < size(), "error!");
			return  *(sequence.data() + n);
		}
		const_reference_type at(const size_type n) const
		{
			ztl::ztl_assert(n >= 0 && n < size(), "error!");
			return  *(sequence.data() + n);
		}
	public:	/*vactor special API wrapper*/
		void shrink_to_fit()
		{
			sequence.shrink_to_fit();
		}
		reference_type front()
		{
			return sequence.front();
		}
		const_reference_type front()const
		{
			return  sequence.front();
		}
		size_type 		capacity()
		{
			//容量不包括null字符
			return  sequence.capacity()-1;
		}
		pointer_type	data()
		{
			return sequence.data();
		}
		const_pointer_type data() const
		{
			return const_cast<self_type*>(this)->data();
		}
		//调整容量
		void reserve(size_type count)
		{
			sequence.reserve(count);
		}
	public:/*InsertBack*/
		reference_type	back()
		{
			return *(end()-1);
		}
		const_reference_type	back()const
		{
			return const_cast<self_type*>(this)->back();
		}
		void			pop_back()
		{
			sequence.pop_back_n(1);
			sequence.back() = 0;
		}
		void			push_back(value_type&& Val)
		{
			sequence.back() = ztl::move(Val);
			check_end_character();
		}
		void			push_back(const value_type& Val)
		{
			sequence.back() = Val;
			check_end_character();
		}
		template<typename... FuncArgs>
		void			emplace_back(FuncArgs&&... Args)
		{
			check_remove_end_null();
			sequence.emplace_back(ztl::forward<FuncArgs>(Args)...);
			check_end_character();
		}

	public:	/*ISequenceContainer*/
		template<typename... FuncArgs>
		iterator_type	emplace(const iterator_type& Where, FuncArgs&&... Args)
		{
			check_remove_pos_null(Where.ptr - sequence.begin());
			auto& temp_iter = sequence.emplace(Where.ptr, ztl::forward<FuncArgs>(Args)...);
			check_end_character();
			return ztl::move(temp_iter);
		}
		//iterator_type	insert(const iterator_type& Where, value_type&& Val)
		//{
		//	check_remove_pos_null(Where.ptr - sequence.begin());
		//	auto& temp_iter = sequence.insert(Where.ptr, ztl::forward<value_type>(Val));
		//	check_end_character();
		//	return ztl::move(temp_iter);
		//}
		/*iterator_type	insert(const_iterator_type& Where, different_type n, const_value_type& Val)
		{
			check_remove_pos_null(Where.ptr - sequence.begin());
			auto& temp_iter = sequence.insert(Where.ptr, n, Val);
			check_end_character();
			return ztl::move(temp_iter);
		}*/
		template<typename InputIterator, class = typename std::std::enable_if<ztl::is_iterator<InputIterator>::value,
			void>::type>
			iterator_type	insert(const_iterator_type& Where, InputIterator first, InputIterator last)
		{
				check_remove_pos_null(Where.ptr - sequence.begin());
				auto& temp_iter = sequence.insert(Where.ptr, first, last);
				check_end_character();
				return ztl::move(temp_iter);
		}
		iterator_type	insert(const_iterator_type& Where, const_iterator_type first, const_iterator_type last)
		{
			check_remove_pos_null(Where.ptr - sequence.begin());
			auto& temp_iter = sequence.insert(Where.ptr, first, last);
			check_end_character();
			return ztl::move(temp_iter);
		}
		iterator_type	insert(const_iterator_type& Where, std::initializer_list<value_type> target)
		{
			check_remove_pos_null(Where.ptr - sequence.begin());
			auto& temp_iter = insert(Where, target.begin(), target.end());
			check_end_character();
			return ztl::move(temp_iter);
		}
		self_type& insert(size_type offset, const self_type& right)
		{	// insert right at offset
			insert(begin() + offset, right.cbegin(), right.cend());
			return (*this);
		}

		self_type& insert(size_type offset,
			const self_type& right, size_type right_offset, size_type count)
		{	// insert right [right_offset, right_offset + count) at offset
			insert(begin() + offset, right.cbegin() + right_offset, right.cbegin() + right_offset + count);
			return *this;
		}

		self_type& insert(size_type offset,
			const char_type *Ptr, size_type count)
		{	// insert [Ptr, Ptr + count) at offset
			sequence.insert(sequence.begin() + offset, Ptr, Ptr + count);
			return (*this);
		}

		self_type& insert(size_type offset, const char_type *Ptr)
		{	// insert [Ptr, <null>) at offset
			insert(offset, Ptr, char_traits_type::length(Ptr));
			return (*this);
		}

		self_type& insert(size_type offset, size_type count, char_type value)
		{	// insert count * value at offset
			self_type temp(count, value);
			insert(begin() + offset, temp.begin(), temp.end());
			return (*this);
		}

		iterator_type insert(const_iterator_type _Where)
		{	// insert <null> at _Where
			return (insert(_Where, char_type()));
		}

		iterator_type insert(const_iterator_type _Where, char_type value)
		{	// insert value at _Where
			size_type offset = distance(begin(), _Where);
			insert(offset, 1, value);
			return (begin() + offset);
		}

		iterator_type insert(const_iterator_type _Where, size_type count, char_type value)
		{	// insert count * char_type at _Where
			size_type offset = distance(begin(), _Where);
			insert(offset, count, value);
			return (begin() + offset);
		}

		void			erase(const iterator_type& Where)
		{
			return sequence.erase(Where.ptr);
		}
		void			erase(const iterator_type& first, const iterator_type& last)
		{
			return sequence.erase(first.ptr, last.ptr);
		}
		void erase(size_type offset = 0)
		{	// erase elements [offset, ...)
			sequence.erase(sequence.begin() + offset, sequence.end());
		}

		void erase(size_type offset, size_type count)
		{	// erase elements [offset, offset + count)
			erase(sequence.begin() + offset, sequence.begin() + offset + count);
		}
		void			resize(const size_type n, const value_type& value)
		{
			check_remove_end_null();
			sequence.resize(n + 1, value);
			sequence.back() = 0;
		}
		void			resize(const size_type n)
		{
			resize(n, value_type());
		}
		template<class InputIterator>
		typename std::enable_if<ztl::is_iterator<InputIterator>::value, self_type&>::type assign(InputIterator first, InputIterator last)
		{
			self_type temp(first, last);
			swap(ztl::move(temp));
			return *this;
		}

		self_type&				assign(const different_type n, const_value_type& target)
		{
			self_type temp(n, target);

			swap(ztl::move(temp));

			return *this;
		}
		self_type& assign(self_type&& right)
		{	// assign by moving right
			self_type temp(ztl::move(right));
			swap(ztl::move(temp));
			return *this;
		}
		self_type& assign(const self_type& right)
		{	// assign right
			self_type temp(right);
			swap(ztl::move(temp));
			return *this;
		}

		self_type& assign(const self_type& right,
			size_type right_offset, size_type count)
		{	// assign right [right_offset, right_offset + count)
			//TODO
			self_type temp(right, right_offset, count);
			swap(ztl::move(temp));
			return *this;
		}

		self_type& assign(const char_type *Ptr, size_type count)
		{	// assign [Ptr, Ptr + count)
			//todo
			self_type temp(Ptr, count);
			swap(ztl::move(temp));
			return *this;
		}

		self_type& assign(const char_type *Ptr)
		{	// assign [Ptr, <null>)
			return (assign(Ptr, char_traits_type::length(Ptr)));
		}
		self_type& assign(std::initializer_list<char_type> Ilist)
		{	// assign initializer_list
			self_type temp(Ilist);
			swap(ztl::move(temp));
			return *this;
		}
	public:/*base_string特有API*/
		friend	self_type operator+(const self_type& left, const self_type& right)
		{	// return string + string
			self_type result;
			result.reserve(left.size() + right.size());
			result += left;
			result += right;
			return ztl::move(result);
		}

		friend	self_type operator+(const char_type *left, const self_type& right)
		{	// return NTCS + string
			self_type result;
			auto i = char_traits_type::length(left);
			auto j = right.size();
			result.reserve(i +j );
			result += left;
			result += right;
			return ztl::move(result);
		}

		friend	self_type operator+(const char_type left, const self_type& right)
		{	// return character + string
			self_type result;
			result.reserve(1 + right.size());
			result += left;
			result += right;
			return ztl::move(result);
		}

		friend	self_type operator+(const self_type& left, const char_type *right)
		{	// return string + NTCS
			self_type result;
			result.reserve(left.size() + char_traits_type::length(right));
			result += left;
			result += right;
			return ztl::move(result);
		}

		friend	self_type operator+(const self_type& left, const char_type right)
		{	// return string + character
			self_type result;
			result.reserve(left.size() + 1);
			result += left;
			result += right;
			return ztl::move(result);
		}

		friend	self_type operator+(const self_type& left, self_type&& right)
		{	// return string + string
			return (ztl::move(right.insert(0, left)));
		}

		friend	self_type operator+(self_type&& left, const self_type& right)
		{	// return string + string
			return (ztl::move(left.append(right)));
		}

		friend self_type operator+(self_type&& left, self_type&& right)
		{	// return string + string
			if(right.size() <= left.capacity() - left.size()
				|| right.capacity() - right.size() < left.size())
			{
				return (ztl::move(left.append(right)));
			}
			else
			{
				return (ztl::move(right.insert(0, left)));
			}
		}

		friend	self_type operator+(const char_type *left, self_type&& right)
		{	// return NTCS + string
			return ztl::move(right.insert(0, left));
		}

		friend	self_type operator+(const char_type left, self_type&& right)
		{	// return character + string
			return (ztl::move(right.insert(0, 1, left)));
		}

		friend self_type operator+(self_type&& left, const char_type *right)
		{	// return string + NTCS
			return (ztl::move(left.append(right)));
		}

		friend	self_type operator+(self_type&& left, const char_type right)
		{	// return string + character
			return (ztl::move(left.append(1, right)));
		}
		size_type length()
		{
			return size();
		}
		self_type& append(std::initializer_list<char_type> Ilist)
		{	// append initializer_list
			return (append(Ilist.begin(), Ilist.end()));
		}
		self_type& append(const self_type& right)
		{	// append right
			return (append(right, 0, -1));
		}

		self_type& append(const self_type& right,
			size_type right_offset, size_type count)
		{	// append right [right_offset, right_offset + count)
			//TODO
			check_remove_end_null();
			sequence.insert(sequence.end(), right.cbegin() + right_offset, count == -1 ? right.cend() : right.cbegin() + right_offset + count);
			check_end_character();
			return *this;
		}

		self_type& append(const char_type *Ptr, size_type count)
		{	// append [Ptr, Ptr + count)
			//TODO
			check_remove_end_null();
			sequence.insert(sequence.end(), Ptr, Ptr + count);
			check_end_character();
			return *this;
		}

		self_type& append(const char_type *Ptr)
		{	// append [Ptr, <null>)
			return (append(Ptr, char_traits_type::length(Ptr)));
		}

		self_type& append(size_type count, char_type value)
		{	// append count * value
			//TODO
			check_remove_end_null();
			sequence.insert(sequence.end(), count, value);
			check_end_character();
			return *this;
		}

		template<class InputIterator>
		typename std::enable_if<  ztl::is_iterator<InputIterator>::value,
			self_type&>::type
			append(InputIterator first, InputIterator last)
		{	// append [first, last), input iterators
				return (replace(end(), end(), first, last));
		}

		self_type& append(const_pointer_type first, const_pointer_type last)
		{	// append [first, last), const pointers
			return replace(end(), end(), first, last);
		}

		self_type& append(const_iterator_type first, const_iterator_type last)
		{	// append [first, last), const_iterators
			return replace(end(), end(), first, last);
		}

		self_type& operator+=(std::initializer_list<char_type> Ilist)
		{	// append initializer_list
			return (append(Ilist.begin(), Ilist.end()));
		}

		self_type& operator+=(const self_type& right)
		{	// append right
			return (append(right));
		}

		self_type& operator+=(const char_type *Ptr)
		{	// append [Ptr, <null>)
			return (append(Ptr));
		}

		self_type& operator+=(char_type value)
		{	// append 1 * value
			return (append((size_type)1, value));
		}

		self_type& replace(size_type offset,
			size_type offset_count, const self_type& right, size_type right_offset, size_type count)
		{	// replace [offset, offset + offset_count) with right [right_offset, right_offset + count)
			erase(begin() + offset, begin() + offset + offset_count);
			insert(begin() + offset, right.cbegin() + right_offset, count == -1 ? right.cend() : right.cbegin() + right_offset + offset_count);
			return *this;
		}

		self_type& replace(size_type offset,
			size_type offset_count, const char_type *Ptr, size_type count)
		{	// replace [offset, offset + offset_count) with [Ptr, Ptr + count)
			erase(offset, offset_count);
			insert(offset, Ptr, count);
			return(*this);
		}
		self_type& replace(size_type offset,
			size_type offset_count, size_type count, char_type value)
		{	// replace [offset, offset + offset_count) with count * value
			erase(offset, offset_count);
		//	cout << this->sequence.data();
			insert(offset, count, value);
			return(*this);
		}
		template<class InputIterator>
		typename std::enable_if<  ztl::is_iterator<InputIterator>::value,
			self_type&>::type
			replace(const_iterator_type first, const_iterator_type last,
			InputIterator right_first, InputIterator right_last)
		{	// replace [first, last) with [right_first, right_last), input iterators
				self_type right(right_first, right_last);
				replace(first, last, right);
				return (*this);
		}

		self_type& replace(size_type offset, size_type offset_count, const self_type& right)
		{	// replace [offset, offset + offset_count) with right
			return (replace(offset, offset_count, right, 0, -1));
		}

		self_type& replace(size_type offset, size_type offset_count, const char_type *Ptr)
		{	// replace [offset, offset + offset_count) with [Ptr, <null>)
			return (replace(offset, offset_count, Ptr, char_traits_type::length(Ptr)));
		}

		self_type& replace(const_iterator_type first, const_iterator_type last,
			const self_type& right)
		{	// replace [first, last) with right
			return (replace(
				distance(begin(), first), distance(first, last), right));
		}
		self_type& replace(const_iterator_type first, const_iterator_type last,
			std::initializer_list<char_type> Ilist)
		{	// replace with initializer_list
			return (replace(first, last, Ilist.begin(), Ilist.end()));
		}
		self_type& replace(const_iterator_type first, const_iterator_type last,
			const char_type *Ptr, size_type count)
		{	// replace [first, last) with [Ptr, Ptr + count)
			return (replace(
				distance(begin(), first), distance(first, last), Ptr, count));
		}

		self_type& replace(const_iterator_type first, const_iterator_type last,
			const char_type *Ptr)
		{	// replace [first, last) with [Ptr, <null>)
			return (replace(
				distance(begin(), first), distance(first, last), Ptr));
		}

		self_type& replace(const_iterator_type first, const_iterator_type last,
			size_type count, char_type value)
		{	// replace [first, last) with count * value
			return (replace(
				distance(begin(), first), distance(first, last), count, value));
		}

		self_type& replace(const_iterator_type first, const_iterator_type last,
			const_pointer_type right_first, const_pointer_type right_last)
		{	// replace [first, last) with [right_first, right_last), const pointers
			if(right_first == right_last)
			{
				erase(first, last);
			}
			else
			{
				erase(first, last);
				insert(first, right_first, right_last);
			}
			return (*this);
		}

		self_type& replace(const_iterator_type first, const_iterator_type last,
			const_iterator_type right_first, const_iterator_type right_last)
		{	// replace [first, last) with [right_first, right_last), const_iterators
			if(right_first == right_last)
			{
				erase(first, last);
			}
			else
			{
				erase(first, last);
				//check_remove_end_null();
				insert(first.ptr, right_first.ptr, right_last.ptr);
			}

			return (*this);
		}

		const char_type* c_str()const
		{
			return sequence.data();
		}

		size_type find(const self_type& right, size_type offset = 0) const
		{	// look for right beginning at or after offset
			return (find(right.data(), offset, right.size()));
		}

		size_type find(const char_type *Ptr,
			size_type offset, size_type count) const
		{	// look for [Ptr, Ptr + count) beginning at or after offset
			if(count == 0 && offset <= this->size())
				return (offset);	// null base_string always matches (if inside base_string)

			size_type number;
			if(offset < this->size() && count <= (number = size() - offset))
			{	// room for match, look for it
				const char_type *uptr, *vptr;
				for(number -= count - 1, vptr = data() + offset;
					(uptr = char_traits_type::find(vptr, number, *Ptr)) != 0;
					number -= uptr - vptr + 1, vptr = uptr + 1)
					if(char_traits_type::compare(uptr, Ptr, count) == 0)
						return (uptr - this->data());	// found a match
			}

			return -1;	// no match
		}

		size_type find(const char_type *Ptr, size_type offset = 0) const
		{	// look for [Ptr, <null>) beginning at or after offset
			return (find(Ptr, offset, char_traits_type::length(Ptr)));
		}

		size_type find(char_type value, size_type offset = 0) const
		{	// look for value at or after offset
			return (find((const char_type *)&value, offset, 1));
		}

		size_type rfind(const self_type& right, size_type offset = -1) const
		{	// look for right beginning before offset
			return (rfind(right.data(), offset, right.size()));
		}

		size_type rfind(const char_type *Ptr,
			size_type offset, size_type count) const
		{	// look for [Ptr, Ptr + count) beginning before offset
			if(count == 0)
				return (offset < size() ? offset
				: size());	// null always matches
			if(count <= size())
			{	// room for match, look for it
				const char_type *uptr = data() + (offset < size() -
					count ?
					offset
					: size() - count
					);
				for(;; --uptr)
				{
					if(char_traits_type::eq(*uptr, *Ptr) && char_traits_type::compare(uptr, Ptr, count) == 0)
					{
						return (uptr - data());	// found a match
					}
					else if(uptr == data())
					{
						break;	// at beginning, no more chance for match
					}
				}
			}

			return (-1);	// no match
		}

		size_type rfind(const char_type *Ptr, size_type offset = -1) const
		{	// look for [Ptr, <null>) beginning before offset
			return (rfind(Ptr, offset, char_traits_type::length(Ptr)));
		}

		size_type rfind(char_type value, size_type offset = -1) const
		{	// look for value before offset
			return (rfind((const char_type *)&value, offset, 1));
		}

		size_type find_first_of(const self_type& right,
			size_type offset = 0) const
		{	// look for one of right at or after offset
			return (find_first_of(right.data(), offset, right.size()));
		}

		size_type find_first_of(const char_type *Ptr,
			size_type offset, size_type count) const
		{	// look for one of [Ptr, Ptr + count) at or after offset
			if(0 < count && offset < size())
			{	// room for match, look for it
				const char_type *const vptr = data() + size();
				for(const char_type *uptr = data() + offset; uptr < vptr; ++uptr)
				{
					if(char_traits_type::find(Ptr, count, *uptr) != 0)
					{
						return (uptr - data());	// found a match
					}
				}
			}
			return (-1);	// no match
		}

		size_type find_first_of(const char_type *Ptr,
			size_type offset = 0) const
		{	// look for one of [Ptr, <null>) at or after offset
			return (find_first_of(Ptr, offset, char_traits_type::length(Ptr)));
		}

		size_type find_first_of(char_type value,
			size_type offset = 0) const
		{	// look for value at or after offset
			return (find((const char_type *)&value, offset, 1));
		}

		size_type find_last_of(const self_type& right,
			size_type offset = -1) const
		{	// look for one of right before offset
			return (find_last_of(right.data(), offset, right.size()));
		}

		size_type find_last_of(const char_type *Ptr,
			size_type offset, size_type count) const
		{	// look for one of [Ptr, Ptr + count) before offset
			if(0 < count && 0 < size())
			{	// worth searching, do it
				const char_type *uptr = data()
					+ (offset < size() ? offset : size() - 1);
				for(;; --uptr)
				{
					if(char_traits_type::find(Ptr, count, *uptr) != 0)
					{
						return (uptr - data());	// found a match
					}
					else if(uptr == data())
					{
						break;	// at beginning, no more chance for match
					}
				}
			}
			return (-1);	// no match
		}

		size_type find_last_of(const char_type *Ptr,
			size_type offset = -1) const
		{	// look for one of [Ptr, <null>) before offset
			return (find_last_of(Ptr, offset, char_traits_type::length(Ptr)));
		}

		size_type find_last_of(char_type value,
			size_type offset = -1) const
		{	// look for value before offset
			return (rfind((const char_type *)&value, offset, 1));
		}

		size_type find_first_not_of(const self_type& right,
			size_type offset = 0) const
		{	// look for none of right at or after offset
			return (find_first_not_of(right.data(), offset,
				right.size()));
		}

		size_type find_first_not_of(const char_type *Ptr,
			size_type offset, size_type count) const
		{	// look for none of [Ptr, Ptr + count) at or after offset
			if(offset < size())
			{	// room for match, look for it
				const char_type *const vptr = data() + size();
				for(const char_type *uptr = data() + offset; uptr < vptr; ++uptr)
				{
					if(char_traits_type::find(Ptr, count, *uptr) == 0)
					{
						return (uptr - data());
					}
				}
			}
			return (-1);
		}

		size_type find_first_not_of(const char_type *Ptr,
			size_type offset = 0) const
		{	// look for one of [Ptr, <null>) at or after offset
			return (find_first_not_of(Ptr, offset, char_traits_type::length(Ptr)));
		}

		size_type find_first_not_of(char_type value,
			size_type offset = 0) const
		{	// look for non value at or after offset
			return (find_first_not_of((const char_type *)&value, offset, 1));
		}

		size_type find_last_not_of(const self_type& right,
			size_type offset = -1) const
		{	// look for none of right before offset
			return (find_last_not_of(right.data(), offset, right.size()));
		}

		size_type find_last_not_of(const char_type *Ptr,
			size_type offset, size_type count) const
		{	// look for none of [Ptr, Ptr + count) before offset
			if(0 < size())
			{	// worth searching, do it
				const char_type *uptr = data()
					+ (offset < size() ? offset : size() - 1);
				for(;; --uptr)
				{
					if(char_traits_type::find(Ptr, count, *uptr) == 0)
					{
						return (uptr - data());
					}
					else if(uptr == data())
					{
						break;
					}
				}
			}
			return (-1);
		}

		size_type find_last_not_of(const char_type *Ptr,
			size_type offset = -1) const
		{	// look for none of [Ptr, <null>) before offset
			return (find_last_not_of(Ptr, offset, char_traits_type::length(Ptr)));
		}

		size_type find_last_not_of(char_type value,
			size_type offset = -1) const
		{	// look for non value before offset
			return (find_last_not_of((const char_type *)&value, offset, 1));
		}

		self_type substr(size_type offset = 0, size_type count = -1) const
		{	// return [offset, offset + count) as new base_string
			return (self_type(*this, offset, count));
		}

		int compare(const self_type& right) const
		{	// compare [0, size()) with right
			return (compare(0, size(), right.data(), right.size()));
		}

		int compare(size_type offset, size_type offset_count,
			const self_type& right) const
		{	// compare [offset, offset + offset_count) with right
			return (compare(offset, offset_count, right, 0, -1));
		}

		int compare(size_type offset,
			size_type offset_count, const self_type& right,
			size_type right_offset, size_type count) const
		{	// compare [offset, offset + offset_count) with right [right_offset, right_offset + count)
			ztl_assert(right.size() >= offset, "offset off end");

			if(right.size() - right_offset < count)
				count = right.size() - right_offset;	// trim count to size
			return (compare(offset, offset_count, right.data() + right_offset, count));
		}

		int compare(const char_type *Ptr) const
		{	// compare [0, size()) with [Ptr, <null>)
			return compare(0, size(), Ptr, char_traits_type::length(Ptr));
		}

		int compare(size_type offset, size_type offset_count, const char_type *Ptr) const
		{	// compare [offset, offset + offset_count) with [Ptr, <null>)
			return (compare(offset, offset_count, Ptr, char_traits_type::length(Ptr)));
		}

		int compare(size_type offset, size_type offset_count, const char_type *Ptr, size_type count) const
		{	// compare [offset, offset + offset_count) with [Ptr, Ptr + count)
			ztl_assert(size() >= offset, "offset off end");
			if(size() - offset < offset_count)
				offset_count = size() - offset;	// trim offset_count to size

			size_type result = char_traits_type::compare(data() + offset, Ptr,
				offset_count < count ? offset_count : count);
			return (result != 0 ? (int)result : offset_count < count ? -1
				: offset_count == count ? 0 : +1);
		}

		size_type copy(char_type *Ptr,
			size_type count, size_type offset = 0) 
		{	// copy [offset, offset + count) to [Ptr, Ptr + count)
			ztl_assert(this->size() >= offset, "offset off end");
			count = this->size() - offset;
			char_traits_type::copy(Ptr, count, this->data() + offset, count);
			return count;
		}

	private:/*base_string细节函数*/
		void check_end_character()
		{
			/*空终结符应该是0x0...*/
			if(sequence.empty()||sequence.back() != 0)
			{
				sequence.emplace_back(char_type(0));
			}
		}
		void check_remove_end_null()
		{
			if(sequence.back() == 0)
			{
				//cout << (int)sequence.back();
				sequence.pop_back();
			}
		}
		void check_remove_pos_null(size_t offset)
		{
			if(*(sequence.data() + offset) == 0)
			{
				/*说明是结尾*/
				sequence.pop_back();
			}
		}
	public:/*IO API*/

	private:
		vector<char_type>											sequence;
	};

	typedef base_string<char, char_traits<char>> string;
	template<class char_traits>
	inline std::basic_ostream<char, char_traits>& operator<<(std::basic_ostream<char, char_traits>& _Ostr, const ztl::string& _Val)
	{	// insert NTBS into char stream
		_Ostr << _Val.data();
		return _Ostr;
	}
}
/*enable_if位置问题- -*/
/*内存调试问题 错的地方和找到的地方查的远*/
/*插入删除变更字符串要处理尾部空字符问题*/
/*swap move 左值 右值问题*/
/*右值构造函数 移动构造函数写法*/
/*vector<char>和string的null字符串问题*/
/*编写swap要避免循环引用*/