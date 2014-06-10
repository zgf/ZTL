#pragma once
#include "ztl_type_traits.h"
/*Container*/
namespace ztl
{
	template<typename...TypeArgs>
	struct is_typelist;
	template<typename... LeftArgs>
	struct typelist_link;
	template<typename... TypeArgs>
	struct type_list
	{
		using self_type = type_list<TypeArgs...>;
		struct size
		{
			static const int value = sizeof...(TypeArgs);
		};

		struct is_empty
		{
			static const bool value = sizeof...(TypeArgs) == 0;
		};
		template<size_t index, typename... Args>
		struct get_index;
		template<size_t index, typename FirstArg, typename... Args>
		struct get_index<index, FirstArg, Args... >
		{
			static_assert(index >= 0, "Index is out of range!");
			static_assert(index < size::value, "Index is out of range!");
			using type = typename get_index<index - 1, Args...>::type;
		};
		template< typename FirstArg, typename... Args>
		struct get_index<0, FirstArg, Args...>
		{
			using type = FirstArg;
		};

		template<size_t index >
		struct get
		{
			using type = typename get_index<index, TypeArgs...>::type;
		};
		struct front
		{
			using type = typename get<0>::type;
		};
		struct back
		{
			using type = typename get<sizeof...(TypeArgs)-1>::type;
		};
		template<typename BackType>
		struct push_back
		{
			using type = type_list<TypeArgs..., BackType>;
		};
		template<typename FrontType>
		struct push_front
		{
			using type = type_list<FrontType, TypeArgs...>;
		};

		template<typename InsertType, size_t index, typename... Args>
		struct insert_impl;
		template<typename InsertType, size_t index, typename FirstArg, typename... Args>
		struct insert_impl<InsertType, index, FirstArg, Args... >
		{
			static_assert(index >= 0, "Index is out of range!");
			static_assert(index <= size::value, "Index is out of range!");

			using tail_type = typename insert_impl<InsertType, index - 1, Args...>::type;
			using type = typename tail_type::template push_front<FirstArg>::type;
		};
		template<typename InsertType, typename FirstArg, typename... Args>
		struct insert_impl<InsertType, 0, FirstArg, Args...>
		{
			using type = type_list<InsertType, FirstArg, Args...>;
		};
		template<typename InsertType >
		struct insert_impl<InsertType, 0>
		{
			using type = type_list<InsertType>;
		};
		template<size_t Where, typename InsertType>
		struct insert
		{
			using type = typename insert_impl<InsertType, Where, TypeArgs...>::type;
		};
		template<size_t index, typename... Args>
		struct erase_impl;
		template<size_t index, typename FirstArg, typename... Args>
		struct erase_impl<index, FirstArg, Args...>
		{
			using tail_type = typename erase_impl<index - 1, Args...>::type;
			using type = typename tail_type::template push_front<FirstArg>::type;
		};
		template<typename FirstArg, typename... Args>
		struct erase_impl<0, FirstArg, Args...>
		{
			using type = type_list<Args...>;
		};
		template<size_t Where>
		struct erase
		{
			using type = typename erase_impl<Where, TypeArgs...>::type;
		};

		template<typename TargetType, size_t start_index, size_t tuple_size>
		struct find_impl
		{
			static_assert(start_index >= 0, "Index is out of range!");
			static_assert(start_index <= size::value, "Index is out of range!");

			static const int value = if_else_value<is_same<typename get<start_index>::type, TargetType>::value, start_index, find_impl<TargetType, start_index + 1, tuple_size - 1>::value>::value;
		};
		template<typename TargetType, size_t start_index>
		struct find_impl<TargetType, start_index, 0>
		{
			static const int value = if_else_value<is_same<typename get<start_index>::type, TargetType>::value, start_index, -1>::value;
		};
		template<typename TargetType, size_t start_index = 0>
		struct find
		{
			static const int value = find_impl<TargetType, start_index, sizeof...(TypeArgs)-1>::value;
		};

		template<typename ElementType, typename CompareType, template<typename, typename> class BinaryFunction, typename TrueType>
		struct do_if
		{
			using type = typename if_else<BinaryFunction<ElementType, CompareType>::value, TrueType, ElementType>::type;
		};
		template< template<typename, typename> class BinaryFunction, typename CompareType, typename TrueType>
		struct do_each_if
		{
			using type = type_list<typename do_if<TypeArgs, CompareType, BinaryFunction, TrueType>::type...>;
		};
		template<typename SrcType, typename DestType>
		struct replace_all
		{
			using type = typename do_each_if<std::is_same, SrcType, DestType>::type;
		};
		template<typename TargetType, typename... Args>
		struct remove_all_impl;
		template<typename TargetType, typename FirstType, typename... Args>
		struct remove_all_impl<TargetType, FirstType, Args... >
		{
			using current_type = typename if_else<std::is_same<TargetType, FirstType>::value, type_list<>, type_list<FirstType>>::type;
			using tail_type = typename remove_all_impl<TargetType, Args...>::type;
			using type = typename typelist_link<current_type, tail_type>::type;
		};
		template<typename TargetType>
		struct remove_all_impl<TargetType>
		{
			using type = type_list<>;
		};
		template<typename TargetType>
		struct remove_all
		{
			using type = typename remove_all_impl<TargetType, TypeArgs...>::type;
		};
		template< typename... Args>
		struct type_compact_impl;
		template<typename FirstType, typename... Args>
		struct type_compact_impl< FirstType, Args... >
		{
			using remove_type = typename type_list<Args...>::template remove_all<FirstType>::type;
			using tail_type = typename remove_type::type_compact::type;
			using type = typename typelist_link<type_list<FirstType>, tail_type>::type;
		};
		template<>
		struct type_compact_impl<>
		{
			using type = type_list<>;
		};
		struct type_compact
		{
			using type = typename type_compact_impl<TypeArgs...>::type;
		};
		template<size_t index>
		struct reverse_impl
		{
			using get_type = typename front::type;
			using rest_type = typename erase<0>::type;
			using new_type = typename typelist_link<rest_type, get_type>::type;
			using type = typename new_type::template reverse_impl<index - 1>::type;
		};
		template<>
		struct reverse_impl<0>
		{
			using type = self_type;
		};
		struct reverse
		{
			using type = typename reverse_impl<sizeof...(TypeArgs)-1>::type;
		};
	};
	template<>
	struct type_list<>
	{
		template<typename BackType>
		struct push_back
		{
			using type = type_list<BackType>;
		};
		template<typename FrontType>
		struct push_front
		{
			using type = type_list<FrontType>;
		};

		template<size_t Where, typename InsertType>
		struct insert;
		template<typename InsertType>
		struct insert<0, InsertType>
		{
			using type = type_list<InsertType>;
		};
		template<typename TargetType>
		struct remove_all
		{
			using type = type_list<>;
		};
		struct type_compact
		{
			using type = type_list<>;
		};
	};
	template<typename...TypeArgs>
	struct is_typelist : false_type
	{
	};
	template<typename... TypeArgs>
	struct is_typelist<type_list<TypeArgs...>> : true_type
	{
	};

	template<typename... LeftArgs>
	struct typelist_link
	{
		using type = type_list<LeftArgs...>;
	};
	template<typename... LeftArgs, typename... RightArgs>
	struct typelist_link<type_list<LeftArgs...>, type_list<RightArgs...>>
	{
		using type = type_list<LeftArgs..., RightArgs...>;
	};
	template<typename... LeftArgs, typename... RightArgs>
	struct typelist_link<type_list<LeftArgs...>, RightArgs...>
	{
		using type = type_list<LeftArgs..., RightArgs...>;
	};
	/*
	error C3515: 如果类模板部分专用化的参数是一个包扩展，则它应是最后一个参数
	template<typename... LeftArgs, typename... RightArgs>
	struct typelist_link<LeftArgs..., type_list<RightArgs...>>
	{
	using type = type_list<LeftArgs..., RightArgs...>;
	};*/
}