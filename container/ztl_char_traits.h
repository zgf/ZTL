#pragma once
#include <crtdefs.h>
#include <wchar.h>
#include <string.h>

namespace ztl
{
	template<typename CharType>
	struct char_traits;
	template<>
	struct char_traits<wchar_t>
	{	// properties of a string or stream wchar_t element
		typedef wchar_t char_type;
		//typedef char_type char_type;	// for overloads
		typedef wint_t int_type;
		
		static int  compare(const char_type *first_left, const char_type *first_right,
			size_t count)
		{	// compare [first_left, first_left + count) with [first_right, ...)
			return (count == 0 ? 0
				:  wmemcmp(first_left, first_right, count));
		}

		static size_t  length(const char_type *first)
		{	// find length of null-terminated sequence
			return (*first == 0 ? 0
				:  wcslen(first));
		}

		static char_type * copy(char_type *first_left, const char_type *first_right,
			size_t count)
		{	// copy [first_right, first_right + count) to [first_left, ...)
			return (count == 0 ? first_left
				: (char_type *) wmemcpy(first_left, first_right, count));
		}

		static char_type * _Copy_s(
			_Out_writes_(_Size_in_words) char_type *first_left, size_t _Size_in_words,
			_In_reads_(count) const char_type *first_right, size_t count)
		{	// copy [first_right, first_right + count) to [first_left, ...)
			if(0 < count)
				wmemcpy_s(first_left, _Size_in_words, first_right, count);
			return (first_left);
		}

		static const char_type * find(const char_type *first, size_t count,
			const char_type& target)
		{	// look for target in [first, first + count)
			return (count == 0 ? (const char_type *)0
				: (const char_type *) wmemchr(first, target, count));
		}

		static char_type * move(char_type *first_left, const char_type *first_right,
			size_t count)
		{	// copy [first_right, first_right + count) to [first_left, ...)
			return (count == 0 ? first_left
				: (char_type *) wmemmove(first_left, first_right, count));
		}

		static char_type * assign(char_type *first, size_t count,
			char_type target)
		{	// assign count * target to [first, ...)
			return ((char_type *) wmemset(first, target, count));
		}

		static void  assign(char_type& left, const char_type& right) 
		{	// assign an element
			left = right;
		}

			static bool  eq(const char_type& left,
			const char_type& right) 
		{	// test for element equality
			return (left == right);
		}

			static bool  lt(const char_type& left,
			const char_type& right) 
		{	// test if left precedes right
			return (left < right);
		}

			static char_type  to_char_type(
			const int_type& target) 
		{	// convert metacharacter to character
			return (target);
		}

			static int_type  to_int_type(
			const char_type& target) 
		{	// convert character to metacharacter
			return (target);
		}

			static bool  eq_int_type(const int_type& left,
			const int_type& right) 
		{	// test for metacharacter equality
			return (left == right);
		}

			static int_type  not_eof(
			const int_type& target) 
		{	// return anything but EOF
			return (target != eof() ? target : !eof());
		}

			static int_type  eof() 
		{	// return end-of-file metacharacter
			return (WEOF);
		}
	};

	// STRUCT char_traits<char> (FROM <string>)
	template<> struct char_traits<char>
	{	// properties of a string or stream char element
		typedef char char_type;
	//	typedef char_type char_type;
		typedef int int_type;
		
		static int  compare(const char_type *first_left, const char_type *first_right,
			size_t count)
		{	// compare [first_left, first_left + count) with [first_right, ...)
			return (count == 0 ? 0
				:  memcmp(first_left, first_right, count));
		}

		static size_t  length(const char_type *first)
		{	// find length of null-terminated string
			return (*first == 0 ? 0
				:  strlen(first));
		}

		static char_type * copy(char_type *des_start,size_t dest_size, const char_type *src_start,
			size_t count)
		{	// copy [src_start, src_start + count) to [des_start, ...)
			if (count != 0)
			{
				memcpy_s(des_start, dest_size, src_start, count);
			}
			return des_start;
		}

		static const char_type * find(const char_type *first, size_t count,
			const char_type& target)
		{	// look for target in [first, first + count)
			return (count == 0 ? (const char_type *)0
				: (const char_type *) memchr(first, target, count));
		}

		static char_type * move(char_type *first_left, const char_type *first_right,
			size_t count)
		{	// copy [first_right, first_right + count) to [first_left, ...)
			return (count == 0 ? first_left
				: (char_type *) memmove(first_left, first_right, count));
		}

		static char_type * assign(char_type *first, size_t count,
			char_type target)
		{	// assign count * target to [first, ...)
			return ((char_type *) memset(first, target, count));
		}

		static void  assign(char_type& left, const char_type& right) 
		{	// assign an element
			left = right;
		}

			static bool  eq(const char_type& left,
			const char_type& right) 
		{	// test for element equality
			return (left == right);
		}

			static bool  lt(const char_type& left,
			const char_type& right) 
		{	// test if left precedes right
			return ((unsigned char)left < (unsigned char)right);
		}

			static char_type  to_char_type(
			const int_type& target) 
		{	// convert metacharacter to character
			return ((char_type)target);
		}

			static int_type  to_int_type(
			const char_type& target) 
		{	// convert character to metacharacter
			return ((unsigned char)target);
		}

			static bool  eq_int_type(const int_type& left,
			const int_type& right) 
		{	// test for metacharacter equality
			return (left == right);
		}

			static int_type  not_eof(
			const int_type& target) 
		{	// return anything but EOF
			return (target != eof() ? target : !eof());
		}

			static int_type  eof() 
		{	// return end-of-file metacharacter
				return -1;
		}
	};
}