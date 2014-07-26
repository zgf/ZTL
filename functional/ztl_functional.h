#pragma once
#include "../type/ztl_type_traits.h"
#include "../concept/ztl_base_concept.h"
#include "../tuple/ztl_tuple.h"
#include "../type/ztl_type_list.h"
namespace ztl
{
	class empty
	{
	};
	enum class pointer_category :bool
	{
		normal_function_ptr,
		object_function_ptr,
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
	template<typename ReturnType>
	class function_traits : public function_traits<decltype(&Type::operator())>
	{
	public:
		typedef function_traits<decltype(&Type::operator())> base_type;
		typedef typename  base_type::decay_ptr_type func_ptr_type;
		template<typename Type>
		static pointer_category BindType(Type&& target, handle& temp)
		{
			//bind lambda����Ӻܶడ- -
			//		func_ptr_type p = target;ֻ��static����ת��
			//bool a = ztl::is_pointer<Type>::value;
			//static����lambda��ʽת��
			temp.normal_functor = reinterpret_cast<typename handle::func_type>(static_cast<func_ptr_type>(ztl::forward<Type&&>(target)));
			return pointer_category::normal_function_ptr;
		}
	};
	//�����const �汾��ר��Ϊ��bind lambda... ��֧��const ��׺�ĺ���
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
			temp.member_funtor.object_func_ptr = reinterpret_cast<typename handle::object_type>(ztl::forward<Type&&>(target));
			return pointer_category::object_function_ptr;
		}
	};

	//�����const �汾��ר��Ϊ��bind lambda... ��֧��const ��׺�ĺ���
	template<typename ReturnType, typename ClassType, typename... FuncArgs>
	class function_traits<ReturnType(__thiscall ClassType::*)(FuncArgs...)volatile>
	{
	public:
		using func_ptr_type = ReturnType(__thiscall ClassType::*)(FuncArgs...);
		using decay_ptr_type = ReturnType(__cdecl*)(FuncArgs...);
		template<typename Type>
		static pointer_category BindType(Type&& target, handle& temp)
		{
			temp.member_funtor.object_ptr = nullptr;
			temp.member_funtor.object_func_ptr = reinterpret_cast<typename handle::object_type>(ztl::forward<Type&&>(target));
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
			temp.normal_functor = reinterpret_cast<typename handle::func_type>(ztl::forward<Type&&>(target));
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
			temp.member_funtor.object_func_ptr = reinterpret_cast<typename handle::object_type>(ztl::forward<Type&&>(target));
			return pointer_category::object_function_ptr;
		}
	};
	//�����Ƿº���- - ���������Ǹ���ָͨ���Ҳֻ�зº�����,��Ϊ�º���Ĭ�ϳ�Ա����ʽoperator()���Բ��ô���
	template<typename ReturnType>
	class function_traits<ReturnType*>
	{
	public:
		typedef ReturnType functor_type;

		template<typename Type>
		static pointer_category BindType(Type&& target, handle& temp)
		{
			temp.member_funtor.object_ptr = (void*)(new ReturnType(*target));
			return pointer_category::object_function_ptr;
		}
	};
	//��������Ͳ�����ͨ����ָ�벻�ǳ�Ա������ָ�벻��R()��ʽ����,�Ǿ���lambda
	template<typename Type>
	class function;
	//�º���
	template<typename Type>
	class function<Type*> :public IEquality<function<Type*>>
	{
	private:
		handle handler;
		pointer_category isObjectPtr;
	public:/*IEquality*/
		bool operator== (const function& right) const
		{
			return handler.normal_functor == handler.normal_functor;
		}
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
			isObjectPtr = function_traits<typename ztl::remove_reference<Type>::type>::BindType(ztl::forward<Type&&>(target), handler);
		}
		function& operator=(nullptr_t)
		{
			if(handler.normal_functor != nullptr)
			{
				delete handler.member_funtor.object_ptr;
			}
			isObjectPtr = pointer_category::normal_function_ptr;
			handler.normal_functor = nullptr;
		}
		~function()
		{
			delete handler.member_funtor.object_ptr;
		}
		bool empty()const
		{
			return handler.normal_functor == nullptr;
		}
		template<typename Type>
		function& operator=(Type&& target)
		{
			if(handler.member_funtor.object_ptr != nullptr)
			{
				delete handler.member_funtor.object_ptr;
				handler.member_funtor.object_ptr = nullptr;
			}
			isObjectPtr = function_traits<typename remove_reference<Type>::type>::BindType(ztl::forward<Type&&>(target), handler);
			return *this;
		}
		function(const function& target)
		{
			isObjectPtr = function_traits<Type*>::BindType((Type*)target.handler.member_funtor.object_ptr, handler);
		}
		function(function&& target)
		{
			isObjectPtr = function_traits<Type*>::BindType((Type*)target.handler.member_funtor.object_ptr, handler);
		}
		//ͨ��decltype��auto��Ͻ�����������������ý������
		template< typename... PushArgs>
		auto operator()(PushArgs&&... Args)->decltype((*static_cast<Type*>(handler.member_funtor.object_ptr))(Args...))
		{
			return (*static_cast<Type*>(handler.member_funtor.object_ptr))(ztl::forward<PushArgs&&>(Args)...);
		}
	};
	//��ӶԷº���֧��.���� function<dectyple(&functor)>(&functor)
	template<typename ReturnType, typename... FuncArgs>
	class function<ReturnType(FuncArgs...)> :public IEquality<function<ReturnType(FuncArgs...)>>
	{
	public:
		typedef ReturnType return_type;
		using func_ptr_type = ReturnType(*)(FuncArgs...);
	private:
		handle handler;
		pointer_category isObjectPtr;
	public:/*IEquality*/
		bool operator== (const function& right) const
		{
			return handler.normal_functor == handler.normal_functor;
		}
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
			//���ﲻ����Type&&
			isObjectPtr = function_traits<typename remove_reference<Type>::type>::BindType(ztl::forward<Type&&>(target), handler);
		}
		function& operator=(nullptr_t)
		{
			isObjectPtr = pointer_category::normal_function_ptr;
			handler.normal_functor = nullptr;
		}
		template<typename Type>
		function& operator=(Type&& target)
		{
			isObjectPtr = function_traits<typename remove_reference<Type>::type>::BindType(ztl::forward<Type&&>(target), handler);
			return *this;
		}

		function(const function& target) :handler(target.handler), isObjectPtr(target.isObjectPtr)
		{
		}
		function(function&& target) :handler(target.handler), isObjectPtr(target.isObjectPtr)
		{
		}
		bool empty()const
		{
			return handler.normal_functor == nullptr;
		}
		template<typename FirstType, typename... PushArgs>
		return_type operator()(FirstType&& firstArg, PushArgs&&... Args)
		{
			if(isObjectPtr == pointer_category::object_function_ptr)
			{
				if(handler.member_funtor.object_ptr == nullptr)
				{
					////˵����һ�������ǵ��õ�class
					//����д����fastcall��ɷ����.ֻ������thiscall����
					//��������Կ��������ʵ�����͸�����Ӱ�캯������,ֻҪ�������һ����ת��Ϊvoid*���͵����ͼ���
					//return ((reinterpret_cast<typename if_else< is_class<FirstType>::value, FirstType, empty>::type*>((void*)*(int*)&firstArg))->*(reinterpret_cast<return_type(typename if_else< is_class<FirstType>::value, FirstType, empty>::type::*)(PushArgs...)>(handler.member_funtor.object_func_ptr)))(Args...);
					//thiscall��fastcall��֧��fastcall��
					using functor_type = ReturnType(__thiscall*)(FuncArgs...);
					return union_cast<functor_type>(handler.member_funtor.object_func_ptr)(forward<FirstType&&>(firstArg), ztl::forward<PushArgs&&>(Args)...);
				}
				else
				{
					//˵����һ���������ǵ�����
					using functor_type = ReturnType(__thiscall*)(void*, FuncArgs...);
					return union_cast<functor_type>(handler.member_funtor.object_func_ptr)(handler.member_funtor.object_ptr, forward<FirstType&&>(firstArg), ztl::forward<PushArgs&&>(Args)...);
				}
			}
			else
			{
				using functor_type = ReturnType(__cdecl*)(FuncArgs...);
				return reinterpret_cast<functor_type>(handler.normal_functor)(forward<FirstType&&>(firstArg), ztl::forward<PushArgs&&>(Args)...);
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
	class function<ReturnType(__cdecl *)(FuncArgs...)> : public IEquality<function<ReturnType(__cdecl *)(FuncArgs...)>>
	{
	public:
		typedef ReturnType return_type;
		using func_ptr_type = ReturnType(__cdecl *)(FuncArgs...);
	private:
		handle handler;
		pointer_category isObjectPtr;
	public:/*IEquality*/
		bool operator== (const function& right) const
		{
			return handler.normal_functor == handler.normal_functor;
		}
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
		function& operator=(nullptr_t)
		{
			isObjectPtr = pointer_category::normal_function_ptr;
			handler.normal_functor = nullptr;
		}
		function& operator=(const func_ptr_type& target)
		{
			isObjectPtr = function_traits<func_ptr_type>::BindType(target, handler);
			return*this;
		}
		function(const function& target) :handler(target.handler), isObjectPtr(target.isObjectPtr)
		{
		}
		function(function&& target) :handler(target.handler), isObjectPtr(target.isObjectPtr)
		{
		}
		bool empty()const
		{
			return handler.normal_functor == nullptr;
		}
		template<typename... PushArgs>
		return_type operator()(PushArgs&&... Args)
		{
			//push������Ĭ������,���Ǻ�����Ҫ������.
			using functor_type = return_type(__cdecl*)(FuncArgs...);
			return reinterpret_cast<functor_type>(handler.normal_functor)(ztl::forward<PushArgs&&>(Args)...);
		}
	};
	template<typename ReturnType, typename ClassType, typename... FuncArgs>
	class function<ReturnType(__thiscall ClassType::*)(FuncArgs...)>
		:public IEquality<function<ReturnType(__thiscall ClassType::*)(FuncArgs...)>>
	{
	public:
		typedef ReturnType return_type;
		using func_ptr_type = ReturnType(__thiscall ClassType::*)(FuncArgs...);
	private:
		handle handler;
		pointer_category isObjectPtr;
	public:/*IEquality*/
		bool operator== (const function& right) const
		{
			return handler.normal_functor == handler.normal_functor;
		}
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
		function& operator=(nullptr_t)
		{
			isObjectPtr = pointer_category::normal_function_ptr;
			handler.normal_functor = nullptr;
		}
		function& operator=(const func_ptr_type& target)
		{
			isObjectPtr = function_traits<func_ptr_type>::BindType(target, handler);
			return*this;
		}
		function(const function& target) :handler(target.handler), isObjectPtr(target.isObjectPtr)
		{
		}
		function(function&& target) :handler(target.handler), isObjectPtr(target.isObjectPtr)
		{
		}
		bool empty()const
		{
			return handler.normal_functor == nullptr;
		}
		template<typename FirstType, typename... PushArgs>
		return_type operator()(FirstType&& firstArg, PushArgs&&... Args)
		{
			using functor_type = ReturnType(__thiscall*)(void*, FuncArgs...);
			return union_cast<functor_type>(handler.member_funtor.object_func_ptr)(firstArg, ztl::forward<PushArgs&&>(Args)...);
		}
	};
	template<typename ReturnType, typename ClassType, typename... FuncArgs>
	class function<ReturnType(__thiscall ClassType::*)(FuncArgs...)const>
		:public IEquality<function<ReturnType(__thiscall ClassType::*)(FuncArgs...)>>
	{
	public:
		typedef ReturnType return_type;
		using func_ptr_type = ReturnType(__thiscall ClassType::*)(FuncArgs...)const;
	private:
		handle handler;
		pointer_category isObjectPtr;
	public:/*IEquality*/
		bool operator== (const function& right) const
		{
			return handler.normal_functor == handler.normal_functor;
		}
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
		function& operator=(nullptr_t)
		{
			isObjectPtr = pointer_category::normal_function_ptr;
			handler.normal_functor = nullptr;
		}
		function& operator=(const func_ptr_type& target)
		{
			isObjectPtr = function_traits<func_ptr_type>::BindType(target, handler);
			return*this;
		}
		function(const function& target) :handler(target.handler), isObjectPtr(target.isObjectPtr)
		{
		}
		function(function&& target) :handler(target.handler), isObjectPtr(target.isObjectPtr)
		{
		}
		bool empty()const
		{
			return handler.normal_functor == nullptr;
		}
		template<typename FirstType, typename... PushArgs>
		return_type operator()(FirstType&& firstArg, PushArgs&&... Args)
		{
			using functor_type = ReturnType(__thiscall*)(const void*, FuncArgs...);
			return union_cast<functor_type>(handler.member_funtor.object_func_ptr)(firstArg, ztl::forward<PushArgs&&>(Args)...);
		}
	};

	template<typename ReturnType, typename ClassType, typename... FuncArgs>
	class function<ReturnType(__thiscall ClassType::*)(FuncArgs...)volatile>
		:public IEquality<function<ReturnType(__thiscall ClassType::*)(FuncArgs...)>>
	{
	public:
		typedef ReturnType return_type;
		using func_ptr_type = ReturnType(__thiscall ClassType::*)(FuncArgs...)volatile;
	private:
		handle handler;
		pointer_category isObjectPtr;
	public:/*IEquality*/
		bool operator== (const function& right) const
		{
			return handler.normal_functor == handler.normal_functor;
		}
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
		function& operator=(nullptr_t)
		{
			isObjectPtr = pointer_category::normal_function_ptr;
			handler.normal_functor = nullptr;
		}
		function& operator=(const func_ptr_type& target)
		{
			isObjectPtr = function_traits<func_ptr_type>::BindType(target, handler);
			return*this;
		}
		function(const function& target) :handler(target.handler), isObjectPtr(target.isObjectPtr)
		{
		}
		function(function&& target) :handler(target.handler), isObjectPtr(target.isObjectPtr)
		{
		}
		bool empty()const
		{
			return handler.normal_functor == nullptr;
		}
		template<typename FirstType, typename... PushArgs>
		return_type operator()(FirstType&& firstArg, PushArgs&&... Args)
		{
			using functor_type = ReturnType(__thiscall*)(volatile void*, FuncArgs...);
			return union_cast<functor_type>(handler.member_funtor.object_func_ptr)(firstArg, ztl::forward<PushArgs&&>(Args)...);
		}
	};

	template<typename PointerType>
	function<typename ztl::remove_reference<PointerType>::type> make_function(PointerType&& ptr)
	{
		//������Ҫfunctionд���ƶ����캯��,��Ϊ��д��const function&�Ĺ��캯��,���Ա�����û���Զ������ƶ�����汾��.
		return ztl::move(function<typename ztl::remove_reference<PointerType>::type>(ptr));
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

	//ռλ���жϺ���
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

	//����ʱ��ȡtuple��size
	template<typename... TupleArgs>
	struct get_tuple_size;
	template<typename... TupleArgs>
	struct get_tuple_size<tuple<TupleArgs...>>
	{
		static const size_t tuple_size = sizeof...(TupleArgs);
	};
	//��������������
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

	template<size_t n, typename BindType, typename CallType>
	typename tuple_element_type<n, CallType>::type select_value(BindType& bind_list, CallType&& call_list, const true_type&)
	{
		return call_list.get<n>();
	}
	template<size_t n, typename BindType, typename CallType>
	typename tuple_element_type<n, BindType>::type select_value(BindType& bind_list, CallType&& call_list, const false_type&)
	{
		return bind_list.get<n>();
	}
	template<size_t index, typename BindType, typename CallType>
	typename if_else<
		is_placeholder<typename tuple_element_type<index,
		BindType>::type>::value,
		typename tuple_element_type<
		is_placeholder<typename tuple_element_type<index
		, BindType>::type>::arg_index - 1,
		CallType>::type,
		typename tuple_element_type<index, BindType>::type>::type  select_index_value(BindType& bind_list, CallType&& call_list)
	{
			return select_value<if_else_value<is_placeholder<typename tuple_element_type<index
				, BindType>::type>::value, is_placeholder<typename tuple_element_type<index
				, BindType>::type>::arg_index - 1, index>::value>(bind_list, ztl::forward<CallType&&>(call_list), integral_constant<bool, is_placeholder<typename tuple_element_type<index, BindType>::type>::value>());
	}

	template<typename FuncType, typename... BindFuncArgs>
	class bind_object :public IEquality<bind_object<FuncType, BindFuncArgs...>>
	{
	public:
		typedef FuncType func_type;
		typedef function<FuncType> functor_type;
		typedef tuple<BindFuncArgs...> bind_list_type;
	private:
		functor_type functor_ptr;
		bind_list_type bind_list;
	public:
		bool operator==(const bind_object& target)const
		{
			return functor_ptr == target.functor_ptr;
		}
	public:
		bind_object()
		{
		}
		bind_object(FuncType&& func_ptr, BindFuncArgs&&... Args) :functor_ptr(ztl::forward<FuncType&&>(func_ptr)), bind_list(ztl::forward<BindFuncArgs&&>(Args)...)
		{
		}
		template<typename... PushFuncArgs>
		auto operator()(PushFuncArgs&&... Args)->decltype(Invoke(bind_list, tuple<PushFuncArgs...>(ztl::forward<PushFuncArgs&&>(Args)...), generator<sizeof... (BindFuncArgs)>::type()))
		{
			return Invoke(bind_list, ztl::forward<tuple<PushFuncArgs...>&&>(tuple<PushFuncArgs...>(ztl::forward<PushFuncArgs&&>(Args)...)), generator<sizeof... (BindFuncArgs)>::type());
		}
		template<typename BindType, typename CallType, size_t... Number>
		auto Invoke(BindType& bind_list, CallType&& call_list, const sequence_wrapper<Number...>&)->decltype(functor_ptr(select_index_value<Number>(bind_list, ztl::forward<CallType&&>(call_list))...))
		{
			return functor_ptr(select_index_value<Number>(bind_list, ztl::forward<CallType&&>(call_list))...);
		}
	};

	////bind��ʵ��
	template<typename FuncType, typename... BindArgs>
	bind_object<FuncType, BindArgs...> bind(FuncType&& func_ptr, BindArgs&&... Args)
	{
		return ztl::move(bind_object<FuncType, BindArgs...>(ztl::forward<FuncType&&>(func_ptr), ztl::forward<BindArgs&&>(Args)...));
	}
}