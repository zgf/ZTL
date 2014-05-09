#pragma once
namespace ztl
{
	namespace functional
	{
		using namespace ztl::traits::type_traits;
		using namespace ztl::wrapper::tuples;
		class empty
		{
		};
		enum class pointer_category:bool
		{
			normal_function_ptr,
			object_function_ptr,
		};
		//用handle消除编译时类型限制
		template<typename dst_type, typename src_type>
		dst_type union_cast(src_type src)
		{
			union
			{
				src_type s;
				dst_type d;
			}u;
			u.s = src;
			return u.d;
		};
		union handle
		{
			using func_type = void(*)();
			using object_type = void(empty::*)();
			struct object_ptr_info
			{
				object_type object_func_ptr;
				void* object_ptr;
			};
			object_ptr_info member_funtor;
			func_type normal_functor;
		};
		template<typename Type>
		class function_traits : public function_traits<decltype(&Type::operator())>
		{
		public:
			typedef function_traits<decltype(&Type::operator())> base_type;
			typedef typename  base_type::decay_ptr_type func_ptr_type;
			template<typename Type>
			static pointer_category BindType(Type&& target, handle& temp)
			{
				//bind lambda这里坑很多啊- -
				//		func_ptr_type p = target;只有static可以转换
				//bool a = std::is_pointer<Type>::value;
				//static触发lambda隐式转换
				temp.normal_functor = reinterpret_cast<typename handle::func_type>(static_cast<func_ptr_type>(std::forward<Type&&>(target)));
				return pointer_category::normal_function_ptr;
			}
		};
		//这里的const 版本是专门为了bind lambda... 和支持const 后缀的函数
		template<typename ReturnType, typename ClassType, typename... FuncArgs>
		class function_traits<ReturnType(__thiscall ClassType::*)(FuncArgs...)const>
		{
		public:
			using func_ptr_type = ReturnType(__thiscall ClassType::*)(FuncArgs...);
			using decay_ptr_type = ReturnType(__cdecl*)(FuncArgs...);
			template<typename Type>
			static pointer_category BindType(Type&& target, handle& temp)
			{
				temp.member_funtor.object_ptr = nullptr;
				temp.member_funtor.object_func_ptr = reinterpret_cast<typename handle::object_type>(std::forward<Type&&>(target));
				return pointer_category::object_function_ptr;
			}
		};
		template<typename ReturnType, typename... FuncArgs>
		class function_traits<ReturnType(*)(FuncArgs...)>
		{
		public:
			using func_ptr_type = ReturnType(*)(FuncArgs...);
			typedef ReturnType return_type;
			template<typename Type>
			static pointer_category BindType(Type&& target, handle& temp)
			{
				temp.normal_functor = reinterpret_cast<typename handle::func_type>(std::forward<Type&&>(target));
				return pointer_category::normal_function_ptr;
			}
		};
		template<typename ReturnType, typename ClassType, typename... FuncArgs>
		class function_traits<ReturnType(__thiscall ClassType::*)(FuncArgs...)>
		{
		public:
			typedef ReturnType return_type;
			using func_ptr_type = ReturnType(__thiscall ClassType::*)(FuncArgs...);
			using decay_ptr_type = ReturnType(__cdecl*)(FuncArgs...);
			typedef ClassType class_type;
			template<typename Type>
			static pointer_category BindType(Type&& target, handle& temp)
			{
				temp.member_funtor.object_ptr = nullptr;
				temp.member_funtor.object_func_ptr = reinterpret_cast<typename handle::object_type>(std::forward<Type&&>(target));
				return pointer_category::object_function_ptr;
			}
		};
		//这种是仿函数- - 传入类型是个普通指针的也只有仿函数了,因为仿函数默认成员函数式operator()所以不用传了
		template<typename ReturnType>
		class function_traits<ReturnType*>
		{
		public:
			typedef ReturnType functor_type;
			
			template<typename Type>
			static pointer_category BindType(Type&& target, handle& temp)
			{
				temp.member_funtor.object_ptr = new ReturnType(*target);
				return pointer_category::object_function_ptr;
			}
		};
		//传入的类型不是普通函数指针不是成员函数的指针不是R()形式函数,那就是lambda
		template<typename Type>
		class function;
		//仿函数
		template<typename Type>
		class function<Type*>
		{
		public:
			handle handler;
			pointer_category isObjectPtr;
		public:
			function()
			{
				handler.normal_functor = nullptr;
			}
			explicit function(nullptr_t)
			{
				handler.normal_functor = nullptr;
			}
			template<typename Type>
			function(Type&& target)
			{
				handler.member_funtor.object_ptr = nullptr;
				//这里不能用Type&&
				isObjectPtr = function_traits<typename remove_reference<Type>::type>::BindType(std::forward<Type&&>(target), handler);
			}
			template<typename Type>
			function& operator=(Type&& target)
			{
				if(handler.member_funtor.object_ptr != nullptr)
				{
					delete handler.member_funtor.object_ptr;
					handler.member_funtor.object_ptr = nullptr;
				}
				isObjectPtr = function_traits<typename remove_reference<Type>::type>::BindType(std::forward<Type&&>(target), handler);
			}
			function(const function& target)
			{
				isObjectPtr = function_traits<Type*>::BindType((typename remove_reference<Type>::type*)target.handler.member_funtor.object_ptr, handler);
			}
			//通过decltype和auto结合解决返回类型依赖调用结果问题
			template< typename... PushArgs>
			auto operator()(PushArgs&&... Args)->decltype((*static_cast<Type*>(handler.member_funtor.object_ptr))(Args...))
			{
				return (*static_cast<Type*>(handler.member_funtor.object_ptr))(Args...);
			}
		};
		//添加对仿函数支持.对于 function<dectyple(&functor)>(&functor)
		template<typename ReturnType, typename... FuncArgs>
		class function<ReturnType(FuncArgs...)>
		{
		public:
			typedef ReturnType return_type;
			using func_ptr_type = ReturnType(*)(FuncArgs...);
		public:
			handle handler;
			pointer_category isObjectPtr;
		public:
			function()
			{
				handler.normal_functor = nullptr;
			}
			explicit function(nullptr_t)
			{
				handler.normal_functor = nullptr;
			}
			template<typename Type>
			function(Type&& target)
			{
				//这里不能用Type&&
				isObjectPtr = function_traits<typename remove_reference<Type>::type>::BindType(std::forward<Type&&>(target), handler);
			}
			template<typename Type>
			function& operator=(Type&& target)
			{
				isObjectPtr = function_traits<typename remove_reference<Type>::type>::BindType(std::forward<Type&&>(target), handler);
			}
			function(const function& target)
			{
				handler = target.handler;
				isObjectPtr = target.isObjectPtr;
			}
			template<typename FirstType, typename... PushArgs>
			return_type operator()(FirstType&& firstArg, PushArgs&&... Args)
			{
				if(isObjectPtr == pointer_category::object_function_ptr)
				{
					if(handler.member_funtor.object_ptr == nullptr)
					{
						////说明第一个参数是调用的class
						//这种写法对fastcall就煞笔了.只能用于thiscall调用
						//从这里可以看出对象的实际类型根本不影响函数调用,只要传入的是一个课转换为void*类型的类型即可
						//return ((reinterpret_cast<typename if_else< is_class<FirstType>::value, FirstType, empty>::type*>((void*)*(int*)&firstArg))->*(reinterpret_cast<return_type(typename if_else< is_class<FirstType>::value, FirstType, empty>::type::*)(PushArgs...)>(handler.member_funtor.object_func_ptr)))(Args...);
						//thiscall该fastcall就支持fastcall了
						using functor_type = ReturnType(__thiscall*)(FuncArgs...);
						return union_cast<functor_type>(handler.member_funtor.object_func_ptr)(firstArg, std::forward<PushArgs&&>(Args)...);
					}
					else
					{
						//说明第一个参数不是调用者 
						using functor_type = ReturnType(__thiscall*)(void*, FuncArgs...);
						return union_cast<functor_type>(handler.member_funtor.object_func_ptr)(handler.member_funtor.object_ptr, firstArg, std::forward<PushArgs&&>(Args)...);
						//return ((reinterpret_cast<typename if_else< is_class<FirstType>::value, FirstType, empty>::type*>((void*)*(int*)&handler.member_funtor.object_ptr))->*(reinterpret_cast<return_type(typename if_else< is_class<FirstType>::value, FirstType, empty>::type::*)(FirstType, PushArgs...)>(handler.member_funtor.object_func_ptr)))(firstArg,Args...);
					}
				}
				else 
				{
					using functor_type = ReturnType(__cdecl*)(FuncArgs...);
					return reinterpret_cast<functor_type>(handler.normal_functor)(firstArg, std::forward<PushArgs&&>(Args)...);
				}
				
			}
			return_type operator()()
			{
				if(handler.member_funtor.object_ptr == nullptr)
				{
					using functor_type = ReturnType(__cdecl*)();
					return reinterpret_cast<functor_type>(handler.normal_functor)();
				}
				else
				{
					using functor_type = ReturnType(__thiscall*)(void*);
					return union_cast<functor_type>(handler.member_funtor.object_func_ptr)(handler.member_funtor.object_ptr);
				}
			}
		};

		template<typename ReturnType, typename... FuncArgs>
		class function<ReturnType(__cdecl *)(FuncArgs...)>
		{
		public:
			typedef ReturnType return_type;
			using func_ptr_type = ReturnType(__cdecl *)(FuncArgs...);
		public:
			handle handler;
			pointer_category isObjectPtr;
			//func_ptr_type handler;
		public:
			function()
			{
				handler.normal_functor = nullptr;
			}
			explicit function(nullptr_t)
			{
				handler.normal_functor = nullptr;
			}
			function(const func_ptr_type& target)
			{
				isObjectPtr = function_traits<func_ptr_type>::BindType(target, handler);
			}
			function& operator=(const func_ptr_type& target)
			{
				isObjectPtr = function_traits<func_ptr_type>::BindType(target, handler);
				return*this;
			}
			function(const function& target)
			{
				handler = target.handler;
				isObjectPtr = target.isObjectPtr;
			}
			template<typename... PushArgs>
			return_type operator()(PushArgs&&... Args)
			{
				//push类型是默认类型,不是函数需要的类型.
				using functor_type = return_type(__cdecl*)(FuncArgs...);
				return reinterpret_cast<functor_type>(handler.normal_functor)(Args...);
			}
		};
		template<typename ReturnType, typename ClassType, typename... FuncArgs>
		class function<ReturnType(__thiscall ClassType::*)(FuncArgs...)>
		{
		public:
			typedef ReturnType return_type;
			using func_ptr_type = ReturnType(__thiscall ClassType::*)(FuncArgs...);
		public:
			handle handler;
			pointer_category isObjectPtr;
		public:
			function()
			{
				handler.normal_functor = nullptr;
			}
			explicit function(nullptr_t)
			{
				handler.normal_functor = nullptr;
			}
			function(const func_ptr_type& target)
			{
				isObjectPtr = function_traits<func_ptr_type>::BindType(target, handler);
			}
			function& operator=(const func_ptr_type& target)
			{
				isObjectPtr = function_traits<func_ptr_type>::BindType(target, handler);
				return*this;
			}
			function(const function& target)
			{
				handler = target.handler;
			}
			template<typename FirstType, typename... PushArgs>
			return_type operator()(FirstType&& firstArg, PushArgs&&... Args)
			{
				using functor_type = ReturnType(__thiscall*)(void*,FuncArgs...);
				return union_cast<functor_type>(handler.member_funtor.object_func_ptr)(firstArg, std::forward<PushArgs&&>(Args)...);
			}
		};

		template<int index>
		class placeholder
		{
		public:
			placeholder()
			{
			}
		};
		static placeholder<1>	_1;
		static placeholder<2>	_2;
		static placeholder<3>	_3;
		static placeholder<4>	_4;
		static placeholder<5>	_5;
		static placeholder<6>	_6;
		static placeholder<7>	_7;
		static placeholder<8>	_8;
		static placeholder<9>	_9;

		//占位符判断函数
		template<typename Type>
		struct is_placeholder;
		template<typename Type>
		struct is_placeholder
		{
			static const size_t arg_index = 0;
			static const bool value = false;
		};
		template<size_t index>
		struct is_placeholder<placeholder<index>>
		{
			static const size_t arg_index = index;
			static const bool value = true;
		};
		template<size_t index>
		struct is_placeholder<placeholder<index>&>
		{
			static const size_t arg_index = index;
			static const bool value = true;
		};

		//编译时获取tuple的size
		template<typename... TupleArgs>
		struct get_tuple_size;
		template<typename... TupleArgs>
		struct get_tuple_size<tuple<TupleArgs...>>
		{
			static const size_t tuple_size = sizeof...(TupleArgs);
		};
		//整形序列生成器
		template<int ...Sequence>
		struct sequence_wrapper
		{
		};
		template<size_t tuple_size, size_t... Sequence>
		struct generator;

		template<size_t tuple_size, size_t... Sequence>
		struct generator :generator<tuple_size - 1, tuple_size - 1, Sequence...>
		{
		};
		template<size_t... Sequence>
		struct generator<0, Sequence...>
		{
			typedef sequence_wrapper<Sequence...> type;
		};
		template<size_t index, typename... Args>
		struct tuple_index_type;
		template<size_t index, typename... Args>
		struct tuple_index_type<index, tuple<Args...>>
		{
			typedef typename get_tuple_index_helper<sizeof...(Args), index, typename tuple<Args...>::current_type, Args...>::type type;
		};
		template<size_t n, typename BindType, typename CallType>
		typename tuple_index_type<n, CallType>::type select_value(BindType& bind_list, CallType&& call_list, const true_type&)
		{
			return call_list.get<n>();
		}
		template<size_t n, typename BindType, typename CallType>
		typename tuple_index_type<n, BindType>::type select_value(BindType& bind_list, CallType&& call_list, const false_type&)
		{
			return bind_list.get<n>();
		}
		template<size_t index, typename BindType, typename CallType>
		typename if_else<
			is_placeholder<typename tuple_index_type<index,
			BindType>::type>::value,
			typename tuple_index_type<
			is_placeholder<typename tuple_index_type<index
			, BindType>::type>::arg_index - 1,
			CallType>::type,
			typename tuple_index_type<index, BindType>::type>::type  select_index_value(BindType& bind_list, CallType&& call_list)
		{
				return select_value<if_else_value<is_placeholder<typename tuple_index_type<index
					, BindType>::type>::value, is_placeholder<typename tuple_index_type<index
					, BindType>::type>::arg_index - 1, index>::value>(bind_list, std::forward<CallType&&>(call_list), integral_constant<bool, is_placeholder<typename tuple_index_type<index, BindType>::type>::value>());
		}

		template<typename FuncType, typename... BindFuncArgs>
		class bind_object
		{
		public:
			typedef FuncType func_type;
			typedef function<FuncType> functor_type;
			typedef tuple<BindFuncArgs...> bind_list_type;
		public:
			functor_type functor_ptr;
			bind_list_type bind_list;
		public:
			bind_object()
			{
			}
			bind_object(FuncType&& func_ptr, BindFuncArgs&&... Args) :functor_ptr(std::forward<FuncType&&>(func_ptr)), bind_list(std::forward<BindFuncArgs&&>(Args)...)
			{
			}
			template<typename... PushFuncArgs>
			auto operator()(PushFuncArgs&&... Args)->decltype(Invoke(bind_list, tuple<PushFuncArgs...>(std::forward<PushFuncArgs&&>(Args)...), generator<sizeof... (BindFuncArgs)>::type()))
			{
				return Invoke(bind_list, tuple<PushFuncArgs...>(std::forward<PushFuncArgs&&>(Args)...), generator<sizeof... (BindFuncArgs)>::type());
			}
			template<typename BindType, typename CallType, size_t... Number>
			auto Invoke(BindType& bind_list, CallType&& call_list, const sequence_wrapper<Number...>&)->decltype(functor_ptr(select_index_value<Number>(bind_list, std::forward<CallType&&>(call_list))...))
			{
				return functor_ptr(select_index_value<Number>(bind_list, std::forward<CallType&&>(call_list))...);
			}
			operator functor_type()
			{
				return functor_ptr;
			}
		};
		

		////bind的实现
		template<typename FuncType, typename... BindArgs>
		bind_object<FuncType, BindArgs...> bind(FuncType&& func_ptr, BindArgs&&... Args)
		{
			return move(bind_object<FuncType, BindArgs...>(std::forward<FuncType&&>(func_ptr), std::forward<BindArgs&&>(Args)...));
		}
	}
}