#pragma once
namespace ztl
{
	namespace functional
	{
		
		template <typename Function>
		struct function_traits
			: public function_traits<decltype(&Function::operator())>
		{
		};

		template <typename ClassType, typename ReturnType, typename... Args>
		struct function_traits<ReturnType(ClassType::*)(Args...) const>
		{
			typedef ReturnType(*pointer)(Args...);
		};
		enum RealType :bool
		{
			FunctionPtr,
			MemberFunctionPtr
		};
		template<typename ReturnType>
		class func_ptr_traits
		{
		public:
			using func_type = typename function_traits<ReturnType>::pointer;
			
			template<typename Type>
			static int* get_addr(const Type& ptr)
			{
				func_type p = ptr;
				return std::move((int*)p);
			}
			static RealType get_tag()
			{
				return FunctionPtr;
			}
		};
	
		template<typename ReturnType,typename ClassType,typename... FuncArgs>
		class func_ptr_traits< ReturnType(ClassType::*)(FuncArgs...)>
		{
		public:
			using func_type = ReturnType(ClassType::*)(FuncArgs...);
			
			template<typename Type>
			static int* get_addr(const Type& ptr)
			{
				union
				{
					Type _f;
					int* _t;
				}ut;
				ut._f = ptr;
				
				return ut._t;;
			}
			static RealType get_tag()
			{
				return MemberFunctionPtr;
			}
		
		};
		template<typename ReturnType, typename... FuncArgs>
		class func_ptr_traits<ReturnType(*)(FuncArgs...)> 
		{
		public:
			using func_type = ReturnType(*)(FuncArgs...);
			static RealType get_tag()
			{
				return FunctionPtr;
			}
			template<typename Type>
			static int* get_addr(const Type& ptr)
			{
				return std::move((int*)ptr);
			}
		};
		

		template<typename ReturnType,typename... RestArgs>
		class function_impl;
		
		template<typename ReturnType,typename FirstType, typename... RestArgs>
		class function_impl<ReturnType,FirstType, RestArgs...>
		{
		public:
			typedef ReturnType return_type;
			function_impl() : addr(nullptr)
			{
				
			}
			function_impl(const function_impl& target)
			{
				if (addr !=nullptr)
				{
					delete addr;
				}
				addr = target.addr;
			}
			template<typename Type>
			function_impl& operator=(const Type& Ptr)
			{
				addr = func_ptr_traits<Type>::get_addr(Ptr);
				return *this;

			}
			function_impl& operator=(const function_impl& target)
			{
				addr = target.addr;
				return *this;

			}
			template<typename Type>
			function_impl(const Type& Ptr) :tag(func_ptr_traits<Type>::get_tag())
			{
				addr = func_ptr_traits<Type>::get_addr(Ptr);
			}
			template<typename... FuncArgs>
			return_type operator()(FirstType firstArg, FuncArgs... Args)
			{
				if (tag == FunctionPtr)
				{
					using funcType = return_type(*)(FirstType, FuncArgs...);
					return	((funcType)addr)(firstArg, Args...);
				}
				else
				{
					using funcType = return_type(__thiscall*)(void*, FuncArgs...);
					return	funcType(addr)((void*)*(int*)&firstArg, Args...);
				}
			}
			int* addr;
			RealType tag;
		};
		template<typename ReturnType>
		class function_impl<ReturnType>
		{
		public:
			typedef ReturnType return_type;
			function_impl() : addr(nullptr)
			{

			}
			function_impl(const function_impl& target)
			{
				if(addr != nullptr)
				{
					delete addr;
				}
				addr = target.addr;
			}
			template<typename Type>
			function_impl& operator=(const Type& Ptr)
			{
				addr = func_ptr_traits<Type>::get_addr(Ptr);
				return *this;

			}
			function_impl& operator=(const function_impl& target)
			{
				addr = target.addr;
				return *this;

			}
			template<typename Type>
			function_impl(const Type& Ptr) 
			{
				addr = func_ptr_traits<Type>::get_addr(Ptr);
			}
			return_type operator()()
			{
				return reinterpret_cast<return_type(*)()>(addr)();
			}
		public:
			int* addr;

		};
		template<typename Type>
		class function;
		template<typename ReturnType,typename... FuncArgs>
		class function<ReturnType(FuncArgs...)>
		{
		public:
			typedef ReturnType return_type;
			function_impl<return_type, FuncArgs...> Functor;
			
		public:
			function<ReturnType(FuncArgs...)>() : Functor()
			{

			}
			template<typename Type>
			function(const Type& ptr) : Functor(ptr)
			{

			}
			template<typename Type>
			function& operator= (const Type& ptr) 
			{
				Functor = ptr;
				return *this;

			}
			template<typename... FuncArgs>
			return_type operator()(FuncArgs... Args)
			{
				return Functor.operator()(Args...);
			}
		};
		
		template <typename typename ReturnType,typename ClassType,typename... FuncArg>
		class function<ReturnType(ClassType::*)(FuncArg...)>
		{
		public:
			typedef ClassType class_type;
			typedef ReturnType return_type;
			using ObjectPtr = return_type(class_type::*)(FuncArg...);
			typedef ObjectPtr func_ptr_type;
		public:
			ObjectPtr functor;

		public:
			function()
			{
				functor = nullptr;
			}
			function(nullptr_t)
			{
				functor = nullptr;
			}
			function(const ObjectPtr& funcPtr) :functor(funcPtr)
			{
		
			}
			function(const function& target) :/*obj(target.obj),*/ functor(target.functor)
			{
			}
			function& operator=(const function& target)
			{
				functor = target.functor;
				return *this;
			}
			function& operator=(const ObjectPtr& target)
			{
				functor = target;
				return *this;
			}
			function& operator=(nullptr_t)
			{
				functor = nullptr;
				return *this;
			}
			template<typename ClassObject,typename... PushFuncArgs>
			return_type operator()(ClassObject object,PushFuncArgs... Args)
			{
				return (object->*functor)(Args...);
			}
		};
		template <typename typename ReturnType,  typename... FuncArg>
		class function<ReturnType(*)(FuncArg...)>
		{
		public:
			typedef ReturnType return_type;
			using FuncPtr = return_type(*)(FuncArg...);
			typedef FuncPtr func_ptr_type;
		public:
			func_ptr_type functor;

		public:
			function()
			{
				functor = nullptr;
			}
			function(nullptr_t)
			{
				functor = nullptr;
			}
			function(const func_ptr_type& funcPtr) :functor(funcPtr)
			{

			}
			function(const function& target) : functor(target.functor)
			{
			}
			function& operator=(const function& target)
			{
				functor = target.functor;
				return *this;

			}
			function& operator=(const func_ptr_type& target)
			{
				functor = target;
				return *this;

			}
			function& operator=(nullptr_t)
			{
				functor = nullptr;
				return *this;

			}
			template< typename... PushFuncArgs>
			return_type operator()(PushFuncArgs... Args)
			{
				return functor(Args...);
			}
		};
	}
}