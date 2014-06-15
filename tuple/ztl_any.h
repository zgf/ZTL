#pragma once
#include <typeinfo>
#include "../type/ztl_type_traits.h"
#include <type_traits>
namespace ztl
{
	class any
	{
	public: // structors

		any() 
			: content(0)
		{
		}

		template<typename ValueType>
		any(const ValueType & value)
			: content(new holder<ValueType>(value))
		{
		}

		any(const any & other)
			: content(other.content ? other.content->clone() : 0)
		{
		}

		// Move constructor
		any(any&& other) 
			: content(other.content)
		{
				other.content = 0;
		}

		// Perfect forwarding of ValueType
		template<typename ValueType>
		any(ValueType&& value, typename std::disable_if<ztl::is_same<any&, ValueType> >::type* = 0)
			: content(new holder< typename remove_reference<ValueType>::type >(static_cast<ValueType&&>(value)))
		{
		}

		~any() 
		{
			delete content;
		}

	public: // modifiers

		any & swap(any & rhs) 
		{
			ztl::swap(content, rhs.content);
			return *this;
		}

			any & operator=(const any& rhs)
		{
				any(rhs).swap(*this);
				return *this;
		}

		// move assignement
		any & operator=(any&& rhs) 
		{
			rhs.swap(*this);
			any().swap(rhs);
			return *this;
		}

			// Perfect forwarding of ValueType
			template <class ValueType>
		any & operator=(ValueType&& rhs)
		{
			any(static_cast<ValueType&&>(rhs)).swap(*this);
			return *this;
		}

	public: // queries

		bool empty() const 
		{
			return !content;
		}

			const std::type_info & type() const
		{
				return content ? content->type() : typeid(void);
		}

	private: // types

		class placeholder
		{
		public: // structors

			virtual ~placeholder()
			{
			}

		public: // queries

			virtual const std::type_info & type() const = 0;

			virtual placeholder * clone() const = 0;

		};

		template<typename ValueType>
		class holder : public placeholder
		{
		public: // structors

			holder(const ValueType & value)
				: held(value)
			{
			}

			holder(ValueType&& value)
				: held(static_cast<ValueType&&>(value))
			{
			}
		public: // queries

			virtual const std::type_info & type() const
			{
				return typeid(ValueType);
			}

			virtual placeholder * clone() const
			{
				return new holder(held);
			}

		public: // representation

			ValueType held;

		private: // intentionally left unimplemented
			holder & operator=(const holder &);
		};


	private: // representation

		template<typename ValueType>
		friend ValueType * any_cast(any *) ;

		placeholder * content;
	};

	class bad_any_cast : public std::bad_cast
	{
	public:
		virtual const char * what() const
		{
			return "boost::bad_any_cast: failed conversion using boost::any_cast";
		}
	};

	template<typename ValueType>
	ValueType * any_cast(any * operand) 
	{
		return operand &&
		operand->type() == typeid(ValueType)
		? &static_cast<any::holder<ValueType> *>(operand->content)->held
		: 0;
	}

		template<typename ValueType>
	inline const ValueType * any_cast(const any * operand) 
	{
		return any_cast<ValueType>(const_cast<any *>(operand));
	}

	template<typename ValueType>
	ValueType any_cast(any & operand)
	{
		typedef typename ztl::remove_reference<ValueType>::type nonref;
		nonref * result = any_cast<nonref>(&operand);
		if(!result)
			throw bad_any_cast();
		return *result;
	}

	template<typename ValueType>inline
	ValueType any_cast(const any & operand)
	{
		typedef typename remove_reference<ValueType>::type nonref;
		return any_cast<const nonref &>(const_cast<any &>(operand));
	}
}