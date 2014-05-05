#pragma once
namespace ztl
{
	namespace functional
	{
		class empty
		{
		};
		//用handle消除编译时类型限制
		union handle
		{
			using func_type = void(*)();
			using object_type = void(empty::*)();
			object_type object_func_ptr;
			func_type normal_functor;
		};
		template<typename Type>
		class function_traits : public function_traits<decltype(&Type::operator())>
		{
		public:
			typedef function_traits<decltype(Type::operator())> base_type;
			template<typename Type>
			static bool BindType(const Type& target, handle& temp)
			{
				//bind lambda这里坑很多啊- - 
				//		func_ptr_type p = target;
				temp.normal_functor = reinterpret_cast<typename handle::func_type>(static_cast<func_ptr_type>(target));
				return false;
			}
		};
		//这里的const 版本是专门为了bind lambda...
		template<typename ReturnType, typename ClassType, typename... FuncArgs>
		class function_traits<ReturnType(ClassType::*)(FuncArgs...)const>
		{
		public:
			using func_ptr_type = ReturnType(*)(FuncArgs...);
			template<typename Type>
			static bool BindType(const Type& target, handle& temp)
			{
				temp.object_func_ptr = reinterpret_cast<typename handle::object_type>(target);
				return true;
			}
		};
		template<typename ReturnType, typename... FuncArgs>
		class function_traits<ReturnType(*)(FuncArgs...)>
		{

		public:
			typedef ReturnType return_type;
			template<typename Type>
			static bool BindType(const Type& target, handle& temp)
			{
				temp.normal_functor = reinterpret_cast<typename handle::func_type>(target);
				return false;
			}
		};
		template<typename ReturnType, typename ClassType, typename... FuncArgs>
		class function_traits<ReturnType(ClassType::*)(FuncArgs...)>
		{
		public:
			typedef ReturnType return_type;
			typedef ClassType class_type;
			template<typename Type>
			static bool BindType(const Type& target, handle& temp)
			{
				temp.object_func_ptr = reinterpret_cast<typename handle::object_type>(target);
				return true;
			}
		};
		//传入的类型不是普通函数指针不是成员函数的指针不是R()形式函数,那就是lambda
		template<typename Type>
		class function;
		template<typename ReturnType, typename... FuncArgs>
		class function<ReturnType(FuncArgs...)>
		{
		public:
			typedef ReturnType return_type;
			using func_ptr_type = ReturnType(*)(FuncArgs...);
		public:
			handle handler;
			bool isObjectPtr;
		public:
			function()
			{

			}
			template<typename Type>
			function(const Type& target)
			{
				isObjectPtr = function_traits<Type>::BindType(target, handler);
			}
			template<typename Type>
			function& operator=(const Type& target)
			{
				isObjectPtr = function_traits<Type>::BindType(target, handler);
			}
			function(const function& target)
			{
				handler = target.handler;
				isObjectPtr = target.isObjectPtr;
			}
			template<typename FirstType, typename... PushArgs>
			return_type operator()(const FirstType& firstArg, const PushArgs&... Args)
			{
				if(isObjectPtr == true)
				{
					return ((reinterpret_cast<typename ztl::traits::type_traits::if_else< ztl::traits::type_traits::is_class<FirstType>::value, FirstType, empty>::type*>((void*)*(int*)&firstArg))->*(reinterpret_cast<return_type(typename ztl::traits::type_traits::if_else< ztl::traits::type_traits::is_class<FirstType>::value, FirstType, empty>::type::*)(PushArgs...)>(handler.object_func_ptr)))(Args...);
				}
				else
				{
					return reinterpret_cast<func_ptr_type>(handler.normal_functor)(firstArg, Args...);
				}
			}
			return_type operator()()
			{
				return reinterpret_cast<func_ptr_type>(handler.normal_functor)();
			}
		};
		template<typename ReturnType, typename... FuncArgs>
		class function<ReturnType(*)(FuncArgs...)>
		{
		public:
			typedef ReturnType return_type;
			using func_ptr_type = ReturnType(*)(FuncArgs...);
		public:
			func_ptr_type handler;
		public:
			function()
			{

			}
			function(const func_ptr_type& target)
			{
				handler = target;
			}
			template<typename Type>
			function& operator=(const func_ptr_type& target)
			{
				handler = target;
			}
			function(const function& target)
			{
				handler = target.handler;
			}
			template<typename... PushArgs>
			return_type operator()(const PushArgs&... Args)
			{
				return handler(Args...);
			}
		};
		template<typename ReturnType, typename ClassType, typename... FuncArgs>
		class function<ReturnType(ClassType::*)(FuncArgs...)>
		{
		public:
			typedef ReturnType return_type;
			using func_ptr_type = ReturnType(ClassType::*)(FuncArgs...);
		public:
			func_ptr_type handler;
		public:
			function()
			{

			}
			function(const func_ptr_type& target)
			{
				handler = target;
			}
			template<typename Type>
			function& operator=(const func_ptr_type& target)
			{
				handler = target;
			}
			function(const function& target)
			{
				handler = target.handler;
			}
			template<typename FirstType, typename... PushArgs>
			return_type operator()(const FirstType& firstArg, const PushArgs&... Args)
			{
				return (firstArg->*handler)(Args...);
			}
		};
		/*template<typename ReturnType, typename ClassType, typename... FuncArgs>
		class function<ReturnType(__stdcall ClassType::*)(FuncArgs...)>
		{
		public:
			typedef ReturnType return_type;
			using func_ptr_type = ReturnType(ClassType::*)(FuncArgs...);
		public:
			func_ptr_type handler;
			const bool value = true;
		public:
			function()
			{

			}
			function(const func_ptr_type& target)
			{
				handler = target;
			}
			template<typename Type>
			function& operator=(const func_ptr_type& target)
			{
				handler = target;
			}
			function(const function& target)
			{
				handler = target.handler;
			}
			template<typename FirstType, typename... PushArgs>
			return_type operator()(const FirstType& firstArg, const PushArgs&... Args)
			{
				return (firstArg->*handler)(Args...);
			}
		};*/
	}
}