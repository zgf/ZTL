#pragma once
namespace ztl
{
	namespace functional
	{
		class empty
		{
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
			typedef typename  base_type::func_ptr_type func_ptr_type;
			template<typename Type>
			static bool BindType(const Type& target, handle& temp)
			{
				//bind lambda这里坑很多啊- - 
				//		func_ptr_type p = target;只有static可以转换
				//bool a = std::is_pointer<Type>::value;
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
				temp.member_funtor.object_func_ptr = reinterpret_cast<typename handle::object_type>(target);
				return true;
			}
		};
		template<typename ReturnType, typename... FuncArgs>
		class function_traits<ReturnType(*)(FuncArgs...)>
		{

		public:
			using func_ptr_type =  ReturnType(*)(FuncArgs...);
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
			using func_ptr_type = ReturnType(*)(FuncArgs...);
			typedef ClassType class_type;
			template<typename Type>
			static bool BindType(const Type& target, handle& temp)
			{
				temp.member_funtor.object_func_ptr = reinterpret_cast<typename handle::object_type>(target);
				return true;
			}
		};
		//这种是仿函数- - 传入类型是个普通指针的也只有仿函数了,因为仿函数默认成员函数式operator()所以不用传了
		template<typename ReturnType>
		class function_traits<ReturnType*>
		{
		public:
			typedef ReturnType functor_type;
			//using func_ptr_type = ReturnType(*)(FuncArgs...);
			//typedef ClassType class_type;
			template<typename Type>
			static bool BindType(const Type& target, handle& temp)
			{
				temp.member_funtor.object_ptr = target;
				temp.member_funtor.object_func_ptr = reinterpret_cast<typename handle::object_type>(&functor_type::operator());
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
				handler.member_funtor.object_ptr = nullptr;
				isObjectPtr = function_traits<Type>::BindType(target, handler);
			}
			template<typename Type>
			function& operator=(const Type& target)
			{
				handler.member_funtor.object_ptr = nullptr;
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
					if(handler.member_funtor.object_ptr == nullptr)
					{
						////说明第一个参数是调用的class
						//return ((reinterpret_cast<typename ztl::traits::type_traits::if_else< ztl::traits::type_traits::is_class<FirstType>::value, FirstType, empty>::type*>((void*)*(int*)&firstArg))->*(reinterpret_cast<return_type(typename ztl::traits::type_traits::if_else< ztl::traits::type_traits::is_class<FirstType>::value, FirstType, empty>::type::*)(PushArgs...)>(handler.member_funtor.object_func_ptr)))(Args...);
						using functor_type = ReturnType(__thiscall*)(void*,PushArgs...);
						return union_cast<functor_type>(handler.member_funtor.object_func_ptr)((void*)*(int*)&firstArg, Args...);
					}
					else
					{
						//说明第一个参数不是调用者
						using functor_type = ReturnType(__thiscall*)(void*,FuncArgs...);
						return union_cast<functor_type>(handler.member_funtor.object_func_ptr)(handler.member_funtor.object_ptr, firstArg, Args...);
					}
					

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
		class function<ReturnType(__cdecl *)(FuncArgs...)>
		{
		public:
			typedef ReturnType return_type;
			using func_ptr_type = ReturnType(__cdecl *)(FuncArgs...);
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
		class function<ReturnType(__thiscall ClassType::*)(FuncArgs...)>
		{
		public:
			typedef ReturnType return_type;
			using func_ptr_type = ReturnType(__thiscall ClassType::*)(FuncArgs...);
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
	}
}