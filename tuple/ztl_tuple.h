#pragma once
#include "../concept/ztl_base_concept.h"
#include "../type/ztl_type_traits.h"
namespace ztl
{
	template<typename Type>
	struct tuple_value : ztl::IEquality<tuple_value<Type>>
	{
		typedef Type value_type;
		typedef tuple_value<Type> self_type;

		tuple_value()
		{
		}
		tuple_value(value_type&& val) :value(std::forward<value_type&&>(val))
		{
		}

		tuple_value& operator=(tuple_value val)
		{
			this->swap(val.value);
			return *this;
		}
		tuple_value(const tuple_value& val) :value(val.value)
		{
		}
		tuple_value(tuple_value&& val) :value(val.value)
		{
		}
		tuple_value& operator=(value_type val)
		{
			this->swap(val);
			return *this;
		}

		~tuple_value()
		{
		}

		void swap(value_type& val)
		{
			swap(value, val);
		}
		/*ztl::IEquality*/
		bool operator== (const self_type& right) const
		{
			return value == right.value;
		}
		template<typename LeftType>
		bool operator== (const LeftType& right) const
		{
			return value == right.value;
		}
		template<typename LeftType>
		bool operator!= (const LeftType& right) const
		{
			return value != right.value;
		}
	public:
		Type value;
	};

	template<size_t Length, size_t Index, typename... FuncArg>
	struct get_tuple_index_helper
	{
	};
	template<size_t Length, size_t Index, typename CurrentType, typename... FuncArg>
	struct get_tuple_index_helper<Length, Index, CurrentType, FuncArg...>
	{
		typedef typename if_else<(Index == Length - 1 - sizeof...(FuncArg)), CurrentType,
			typename get_tuple_index_helper<Length, Index, FuncArg...>::type
		>::type type;
	};
	template<size_t Length, size_t Index>
	struct get_tuple_index_helper<Length, Index>
	{
		typedef nullptr_t type;
	};
	template<typename... FuncArg>
	struct tuple;

	template<size_t index, typename... Args>
	struct tuple_element_type;
	template<size_t index, typename... Args>
	struct tuple_element_type<index, tuple<Args...>>
	{
		//typedef typename type_list<Args...>::tempalte get<index>::type type;
		typedef typename get_tuple_index_helper<sizeof...(Args), index, typename tuple<Args...>::current_type, Args...>::type type;
	};

	template<typename CurrentType, typename... FuncArg>
	struct tuple<CurrentType, FuncArg...>
		: public ztl::IEquality<tuple<CurrentType, FuncArg...>>,
		private tuple<FuncArg...>
	{
	public:
		typedef CurrentType current_type;
		//这样写报错“FuncArg”: 必须在此上下文中扩展参数包
		//	typedef FuncArg rest_arg;
		//这样写报错“FuncArg”: 此上下文中无法扩展参数包
		//真奇葩...
		//tyedef FuncArg... rest_arg
		typedef tuple<current_type, FuncArg...> self_type;
		typedef tuple<FuncArg...> base_type;
		typedef tuple_value<current_type> wrapper_type;
	public:
		tuple_value<current_type> current_element;
		const int tuple_size = 1 + sizeof...(FuncArg);
	public:
		tuple() :current_element(wrapper_type())
		{
		}
		tuple(current_type&& current, FuncArg&&...  Arg) : tuple<FuncArg...>(std::forward<FuncArg&&>(Arg)...), current_element(std::forward<current_type&&>(current))
		{
		}
		//tuple的构造函数式依赖模板参数的,所以这里的参数类型是通用类型,可捕获左值右值,所以不用写const版本了
		/*tuple(const CurrentType& current,const FuncArg&...  Arg) : tuple<FuncArg...>(Arg...), current_element(current)
		{
		}*/
		//???这里不太清楚- -我传入的是右值,如果下面的注释去掉,却会绑定const版本,貌似确实啊,本来参数的原始类型就是const &类型
		tuple(tuple&& right) : base_type(std::forward<base_type&&>(right.get_rest())), current_element(std::forward<wrapper_type&&>(right.current_element))
		{
		}

		self_type& operator=(self_type right)
		{
			swap(right);
		}

		template<size_t index>
		typename tuple_element_type<index, self_type>::type get()
		{
			return get_impl<index, 0>(integral_constant<bool, index == 0>());
		}
		template< size_t index, size_t current_index>
		typename tuple_element_type<index - current_index, self_type>::type get_impl(integral_constant<bool, false>)
		{
			return base_type::get_impl<index, current_index + 1>(integral_constant<bool, index == current_index + 1>());
		}
		template< size_t index, size_t current_index>
		typename tuple_element_type<index - current_index, self_type>::type get_impl(integral_constant<bool, true>)
		{
			return current_element.value;
		}

	public:
		void swap(tuple& right)
		{
			swap(current_element, right.current_element);
			base_type::swap(right.get_rest());
		}
		const int& size()const
		{
			return tuple_size;
		}
		base_type& get_rest()
		{
			return *this;
		}
		const base_type& get_rest() const
		{
			return *this;
		}
	public:
		bool operator== (const self_type& right) const
		{
			if(size() != right.size())
			{
				return false;
			}
			else if(current_element != right.current_element)
			{
				return false;
			}
			else
			{
				return (get_rest() == right.get_rest());
			}
		}
		template<typename... RightArg>
		bool operator ==(const tuple<RightArg...>& right)
		{
			if(size() != right.size())
			{
				return false;
			}
			else if(current_element != right.current_element)
			{
				return false;
			}
			else
			{
				return (get_rest() == right.get_rest());
			}
		}
		template<typename... RightArg>
		bool operator !=(const tuple<RightArg...>& right)
		{
			return !(*this == right);
		}
	};
	template<>
	struct tuple<> : public ztl::IEquality<tuple<>>
	{
	public:
		typedef tuple<> self_type;
		typedef nullptr_t current_type;
		const size_t size() const
		{
			return 0;
		}
		void swap(tuple& right)
		{
		}
		self_type& get_rest()
		{
			return *this;
		}
		template< size_t index, size_t current_index>
		typename tuple_element_type<index, self_type>::type get_impl(integral_constant<bool, false>)
		{
			throw "bda_index";
			return tuple_element_type<index, self_type>::type();
		}
		template< size_t index, size_t current_index>
		typename tuple_element_type<index, self_type>::type get_impl(integral_constant<bool, true>)
		{
			return tuple_element_type<index, self_type>::type();
		}
		bool operator== (const self_type& right) const
		{
			return true;
		}
		template< typename... RightArg>
		bool operator ==(const tuple<RightArg...>& right)const
		{
			if(size() == right.size())
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		template< typename... RightArg>
		bool operator !=(const tuple<RightArg...>& right)const
		{
			if(size() != right.size())
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	};
	template<typename... Arg>
	tuple<Arg...> make_tuple(Arg&&...  arg)
	{
		return std::move(tuple<Arg...>(forward<Arg&&>(arg)...));
	}
}