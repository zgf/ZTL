#pragma once
namespace ztl
{
	namespace functional
	{
		class empty
		{
		};

		//��handle��������ʱ��������
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
				//bind lambda����Ӻܶడ- - 
				//		func_ptr_type p = target;ֻ��static����ת��
				//bool a = std::is_pointer<Type>::value;
				temp.normal_functor = reinterpret_cast<typename handle::func_type>(static_cast<func_ptr_type>(target));
				return false;
			}
		};
		//�����const �汾��ר��Ϊ��bind lambda... ��֧��const ��׺�ĺ���
		template<typename ReturnType, typename ClassType, typename... FuncArgs>
		class function_traits<ReturnType(__thiscall ClassType::*)(FuncArgs...)const>
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
		class function_traits<ReturnType(__thiscall ClassType::*)(FuncArgs...)>
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
		//�����Ƿº���- - ���������Ǹ���ָͨ���Ҳֻ�зº�����,��Ϊ�º���Ĭ�ϳ�Ա����ʽoperator()���Բ��ô���
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
				temp.member_funtor.object_ptr = new ReturnType(*target);
				temp.member_funtor.object_func_ptr = reinterpret_cast<typename handle::object_type>(&functor_type::operator());
				return true;
			}
		};
		//��������Ͳ�����ͨ����ָ�벻�ǳ�Ա������ָ�벻��R()��ʽ����,�Ǿ���lambda
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
						////˵����һ�������ǵ��õ�class
						//����д����fastcall��ɷ����.ֻ������thiscall����
						//��������Կ��������ʵ�����͸�����Ӱ�캯������,ֻҪ�������һ����ת��Ϊvoid*���͵����ͼ���
						//return ((reinterpret_cast<typename ztl::traits::type_traits::if_else< ztl::traits::type_traits::is_class<FirstType>::value, FirstType, empty>::type*>((void*)*(int*)&firstArg))->*(reinterpret_cast<return_type(typename ztl::traits::type_traits::if_else< ztl::traits::type_traits::is_class<FirstType>::value, FirstType, empty>::type::*)(PushArgs...)>(handler.member_funtor.object_func_ptr)))(Args...);
						//thiscall��fastcall��֧��fastcall��
						using functor_type = ReturnType(__thiscall*)(void*, PushArgs...);
						return union_cast<functor_type>(handler.member_funtor.object_func_ptr)((void*)*(int*)&firstArg, Args...);
					}
					else
					{
						//˵����һ���������ǵ�����
						using functor_type = ReturnType(__thiscall*)(void*,FuncArgs...);
						return union_cast<functor_type>(handler.member_funtor.object_func_ptr)(handler.member_funtor.object_ptr, firstArg, Args...);
						//return ((reinterpret_cast<typename ztl::traits::type_traits::if_else< ztl::traits::type_traits::is_class<FirstType>::value, FirstType, empty>::type*>((void*)*(int*)&handler.member_funtor.object_ptr))->*(reinterpret_cast<return_type(typename ztl::traits::type_traits::if_else< ztl::traits::type_traits::is_class<FirstType>::value, FirstType, empty>::type::*)(FirstType, PushArgs...)>(handler.member_funtor.object_func_ptr)))(firstArg,Args...);
					}
				}
				else
				{
					return reinterpret_cast<func_ptr_type>(handler.normal_functor)(firstArg, Args...);
				}
			}
			return_type operator()()
			{
				if(handler.member_funtor.object_ptr == nullptr)
				{
					return reinterpret_cast<func_ptr_type>(handler.normal_functor)();
				}
				else
				{
					using functor_type = ReturnType(__thiscall*)(void*, FuncArgs...);
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

		template<int index>
		class placeholder
		{

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

		template<typename Type, typename... CallArgs>
		class bind_traits
		{
			
		};
		template<typename... CallArgs, typename... BindArgs>
		class bind_traits<ztl::wrapper::tuples::tuple<BindArgs...>, ztl::wrapper::tuples::tuple<CallArgs...>>
		{
		public:
			typedef ztl::wrapper::tuples::tuple<BindArgs...> bind_list_type;
			typedef ztl::wrapper::tuples::tuple<CallArgs...> call_list_type;
		public:

			bind_traits()
			{

			}
			/*template<typename ReturnType,typename PtrType>
			ReturnType Invoke(const PtrType& func_ptr,const bind_list_type& bind_list,const call_list_type& call_list)
			{
				func_ptr(arg_trait<BindArgs>(call_list)...)
				
			}*/
			//template<typename Type>

		};

		template<typename FuncType,typename... BindFuncArgs>
		class bind_object
		{
		public:
			typedef FuncType func_type;
			typedef function<FuncType> functor_type;
			typedef typename functor_type::return_type return_type;
			typedef ztl::wrapper::tuples::tuple<BindFuncArgs...> bind_list_type;
		public:
			functor_type functor_ptr;
			bind_list_type bind_list;
		public:
			bind_object()
			{

			}
			bind_object(const FuncType& func_ptr, const BindFuncArgs&... Args) :functor_ptr(func_ptr), bind_list(Args...)
			{

			}
			template<typename... PushFuncArgs>
			return_type operator()(const PushFuncArgs&... Args)
			{
				ztl::wrapper::tuples::tuple<PushFuncArgs...> call_list(Args...);
				bind_traits<bind_list_type, ztl::wrapper::tuples::tuple<PushFuncArgs...>>();
				//functor_ptr(Args...);
				return return_type();
			}
		};
		////bind��ʵ��
		template<typename FuncType, typename... BindArgs>
		bind_object<FuncType, BindArgs...> bind(const FuncType& func_ptr, const BindArgs&... Args)
		{
			return ztl::traits::type_traits::move(bind_object<FuncType, BindArgs...>(func_ptr, Args...));
		}
	}
}