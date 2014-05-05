#pragma once
#include "ztl_concept_base.h"
#include "ztl_type_traits.h"
namespace ztl
{
	namespace wrapper
	{
		namespace tuples
		{
			template<typename Type>
			struct tuple_value : ztl::concept::concept_base::IEquality<tuple_value<Type>>
			{
				typedef Type value_type;
				typedef tuple_value<Type> mySelf;

				tuple_value()
				{

				}
				/*tuple_value(value_type&& val) :value(ztl::traits::type_traits::move(val))
				{
					
				}*/
				tuple_value(const value_type& val) :value(val)
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
					ztl::traits::type_traits::swap(value, val);
				}
				/*IEquality*/
				bool operator== (const mySelf& right) const
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
			struct GetIndexType_Helper
			{

			};
			template<size_t Length, size_t Index, typename CurrentType, typename... FuncArg>
			struct GetIndexType_Helper<Length, Index, CurrentType, FuncArg...>
			{
				typedef typename ztl::traits::type_traits::if_else<(Index == Length - 1 - sizeof...(FuncArg)), CurrentType,
					typename GetIndexType_Helper<Length, Index, FuncArg...>::type
				>::type type;
			};
			template<size_t Length, size_t Index>
			struct GetIndexType_Helper<Length, Index>
			{
				typedef nullptr_t type;
			};
			template<typename... FuncArg>
			struct tuple;
			template<typename CurrentType, typename... FuncArg>
			struct tuple<CurrentType, FuncArg...> 
				: public ztl::concept::concept_base::IEquality<tuple<CurrentType, FuncArg...>>,
				private tuple<FuncArg...>
			{
			public:
				typedef CurrentType current_type;
				//这样写报错“FuncArg”: 必须在此上下文中扩展参数包
				//	typedef FuncArg rest_arg;
				//这样写报错“FuncArg”: 此上下文中无法扩展参数包
				//真奇葩...
				//tyedef FuncArg... rest_arg
				typedef tuple<current_type, FuncArg...> mySelf;
				typedef tuple<FuncArg...> base_type;
				typedef tuple_value<current_type> wrapper_type;
			public:
				tuple() :current_element(wrapper_type>())
				{

				}
				tuple(CurrentType&& current, FuncArg&&...  Arg) : tuple<FuncArg...>(Arg...), current_element(ztl::traits::type_traits::forward<wrapper_type>(current))
				{

				}
				tuple(CurrentType& current, FuncArg&...  Arg) : tuple<FuncArg...>(Arg...), current_element(current)
				{

				}
				tuple(const tuple& right) : base_type(right.get_rest()), current_element(right.current_element.val)
				{

				}
				tuple(tuple&& right) :
					base_type(std::forward<base_type>(right.get_rest())),
					current_element(std::forward<wrapper_type>(right.current_element))
				{

				}
				mySelf& operator=(mySelf right)
				{
					swap(right);
				}
				template<size_t index>
				typename GetIndexType_Helper<1 + sizeof...(FuncArg), index, current_type, FuncArg...>::type& get()
				{

					return get_impl<index, typename GetIndexType_Helper<(1 + sizeof...(FuncArg)), index, current_type, FuncArg...>::type>(1 + sizeof...(FuncArg));
				}
				template<size_t index>
				const typename GetIndexType_Helper<1 + sizeof...(FuncArg), index, current_type, FuncArg...>::type& get()const
				{

					return get_impl<index, typename GetIndexType_Helper<(1 + sizeof...(FuncArg)), index, current_type, FuncArg...>::type>(1 + sizeof...(FuncArg));
				}
				template<size_t index, typename IndexElementType>
				IndexElementType& get_impl(const size_t Length)
				{
					if(index == Length - size())
					{
						return (IndexElementType&)current_element.value;
					}
					else
					{
						return (IndexElementType&)base_type::get_impl<index, IndexElementType>(Length);
					}
				}
				template<size_t index, typename IndexElementType>
				const IndexElementType& get_impl(const size_t Length)const
				{
					if(index == Length - size())
					{
						return (const IndexElementType&)current_element.value;
					}
					else
					{
						return (const IndexElementType&)base_type::get_impl<index, IndexElementType>(Length);
					}
				}
			public:
				void swap(tuple& right)
				{
					ztl::traits::type_traits::swap(current_element, right.current_element);
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
				bool operator== (const mySelf& right) const
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
			public:
				tuple_value<current_type> current_element;
				const int tuple_size = 1 + sizeof...(FuncArg);
			};
			template<>
			struct tuple<> : public ztl::concept::concept_base::IEquality<tuple<>>
			{
			public:
				typedef tuple<> mySelf;
				const size_t size() const
				{
					return 0;
				}
				void swap(tuple& right)
				{

				}
				mySelf& get_rest()
				{
					return *this;
				}
				template<size_t index, typename IndexElementType>
				IndexElementType& get_impl(const size_t Length)
				{
					throw "index error";
					return  (IndexElementType&)(*this);
				}
				bool operator== (const mySelf& right) const
				{
					return true;
				}
				template< typename... RightArg>
				bool operator ==(tuple<RightArg...>& right)
				{
					if (size() == right.size())
					{
						return true;
					}
					else
					{
						return false;
					}
				}
				template< typename... RightArg>
				bool operator !=(tuple<RightArg...>& right)
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
				return move(tuple<Arg...>(arg...));
			}

			//添加比较支持

		/*	template<typename... LeftArg, typename... RightArg>
			bool operator ==(tuple<LeftArg...>& left, tuple<RightArg...>& right)
			{
				if(left.size() != right.size())
				{
					return false;
				}
				else
				{
					return left.operator ==(right);
				}
			}*/
			template<typename FirstType, typename SecondType>
			class pair : 
				public ztl::concept::concept_base::IAssignable<pair<FirstType, SecondType>>,
				public ztl::concept::concept_base::IEquality<pair<FirstType, SecondType>>,
				public ztl::concept::concept_base::IOrdering<pair<FirstType, SecondType>>
			{
			public:
				typedef FirstType						first_type;
				typedef SecondType						second_type;
				typedef pair<first_type, second_type>	mySelf;
			public:
				first_type			first;
				second_type			second;
			public:/*IEquality*/
				bool operator== (const mySelf& right) const
				{
					return first == right.first && second == right.second;
				}
			public:	/*IAssignable*/
				pair() = default;
				pair(const mySelf& target) :first(target.first), second(target.second)
				{

				}
				pair& operator=(const mySelf& target)
				{
					first = target.first;
					second = target.second;
					return *this;
				}
				~pair() = default;
			
			public:
				
				pair(const first_type& left, const second_type& right) :first(left), second(right)
				{

				}
				template<typename T1, typename T2>
				pair(const pair<T1, T2>& target) : first(target.first), second(target.second)
				{

				}
				template<typename T1, typename T2>
				pair& operator=(const pair<T1, T2>& target)
				{
					first = target.first;
					second = target.second;
					return *this;
				}
			
			};
			template<typename FirstType, typename SecondType,typename ThirdType>
			class triple :public
				ztl::concept::concept_base::IAssignable<triple<FirstType, SecondType, ThirdType>>,
				ztl::concept::concept_base::IEquality<triple<FirstType, SecondType, ThirdType>>,
				ztl::concept::concept_base::IOrdering<triple<FirstType, SecondType, ThirdType>>
			{
			public:
				typedef FirstType						first_type;
				typedef SecondType						second_type;
				typedef ThirdType						third_type;
				typedef triple<first_type, second_type, third_type>	mySelf;

			public:
				first_type			first;
				second_type			second;
				third_type			third;
			public:	/*IAssignable*/
				triple() = default;
				triple(const mySelf& target) :first(target.first), second(target.second), third(target.third)
				{

				}
				triple& operator=(const mySelf& target)
				{
					first = target.first;
					second = target.second;
					third = target.third;
					return *this;
				}
				~triple() = default;
			public:/*IEquality*/
				bool operator== (const mySelf& right)const
				{
					return this->first == right.first && this->second == right.second&& this->third == right.third;
				}
			public:

				triple(const first_type& left, const second_type& middle, const third_type right) :first(left), second(middle), third(right)
				{

				}
				template<typename T1, typename T2,typename T3>
				triple(const triple<T1, T2, T3>& target) : first(target.first), second(target.second), third(target.third)
				{

				}
				template<typename T1, typename T2, typename T3>
				triple& operator=(const triple<T1, T2, T3>& target)
				{
					first = target.first;
					second = target.second;
					third = target.third;
					return *this;
				}
			};
		}
	}
}