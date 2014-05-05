#pragma once
namespace ztl
{
	namespace traits
	{
		namespace iterator_traits
		{
			struct input_iterator
			{

			};
			struct output_iterator
			{

			};
			struct forward_iterator
			{

			};
			struct bidirecttional_iterator
			{

			};
			struct random_acess_iterator
			{

			};
			template<typename Iterator>
			class iterator_traits
			{
				typedef Iterator iterator_type;
				typedef typename iterator_type::value_type value_type;
				typedef typename iterator_type::pointer_type pointer_type;
				typedef typename iterator_type::reference_type reference_type;
				typedef typename iterator_type::different_type different_type;
				typedef typename iterator_type::iterator_category iterator_category;
			};
			template<typename Iterator>
			class iterator_traits<Iterator*>
			{
				typedef Iterator iterator_type;
				typedef Iterator value_type;
				typedef Iterator* pointer_type;
				typedef Iterator& reference_type;
				typedef ptrdiff_t different_type;
				typedef random_acess_iterator iterator_category;
			};
			template<typename Iterator>
			class iterator_traits<const Iterator*>
			{
				typedef Iterator iterator_type;
				typedef Iterator value_type;
				typedef const Iterator* pointer_type;
				typedef const Iterator& reference_type;
				typedef ptrdiff_t different_type;
				typedef random_acess_iterator iterator_category;
			};
		}
		namespace type_traits
		{
			/*类型常量*/
			template<typename ValueType, ValueType Val>
			struct integral_constant
			{
			public:
				typedef ValueType value_type;
				typedef integral_constant<value_type, Val> self_type;
			public:
				static const value_type value = Val;
				operator value_type()
				{
					return value;
				}
			};
			typedef integral_constant<bool, true> true_type;
			typedef integral_constant<bool, false> false_type;
			/*编译期工具*/
			//1 2转换成true,false
			class TrueType
			{
				char temp;
			};
			class FlaseType
			{
				char temp[2];
			};
			template<size_t i>
			struct sizeof_predicate;
			template<>
			struct sizeof_predicate<1> :true_type
			{

			};
			template<>
			struct sizeof_predicate<2> :false_type
			{

			};
			template<bool>
			struct bool_predicate
			{

			};
			template<>
			struct bool_predicate<false> :false_type
			{

			};
			template<>
			struct bool_predicate<true> :true_type
			{

			};
			template<bool Chose,typename TruePath,typename FalsePath>
			struct if_else
			{
			};
			template<typename TruePath,typename FalsePath>
			struct if_else<true, TruePath, FalsePath>
			{
				typedef TruePath type;
			};
			template<typename TruePath, typename FalsePath>
			struct if_else<false, TruePath, FalsePath>
			{
				typedef FalsePath type;
			};
			
			
			/*类型关系*/
			template<typename T1,typename T2>
			struct is_same :false_type
			{

			};
			template<typename T1>
			struct is_same<T1,T1> : true_type
			{

			};
//FromType可以转换为ToType则返回true
			
			/*template<typename FromType,typename ToType>
			struct is_convertible_impl
			{
				static TrueType test_convertible(ToType);
				static FlaseType test_convertible(...);
			};
			template<typename FromType, typename ToType>
			struct is_convertible : sizeof_predicate<sizeof(is_convertible_impl<FromType, ToType>::test_convertible(FromType()))>
			{
				
			};*/
			
			template<typename BaseType, typename DerivedType>
			struct is_base_of : bool_predicate<__is_base_of(BaseType, DerivedType)>
			{

			};
			

			/*类型属性*/
			template<typename Type>
			struct alignment_impl
			{
				Type One;
				char Two;
				Type Three;
				
			};
			
			template<typename Type>
			struct alignment_of :integral_constant<size_t, sizeof(alignment_impl<Type>) - 2*sizeof(Type)>
			{

			};
#define  IS_TYPE_CV_PROPERTY(property)\
			template<typename Type>\
			struct is_##property : false_type\
			{\
			};\
			template<typename Type>\
			struct is_##property<property Type> : true_type\
			{\
			};
			
			IS_TYPE_CV_PROPERTY(const);
			IS_TYPE_CV_PROPERTY(volatile);
#undef IS_TYPE_CV_PROPERTY

			
			/*类型转化*/

			//移除类型的cv属性
#define REMOVE_CV(cv) template<typename Type>\
			struct remove_##cv\
			{\
				typedef Type type;\
			};\
			template<typename Type>\
			struct remove_##cv<cv Type>\
			{\
				typedef Type type;\
			};\
			template<typename Type>\
			struct remove_##cv<cv Type[]>\
			{\
				typedef Type type[];\
			};\
			template<typename Type,unsigned int Number>\
			struct remove_##cv<cv Type[Number]>\
			{\
			typedef Type type[Number]; \
			}; 
			
			REMOVE_CV(const);
			REMOVE_CV(volatile);
#undef REMOVE_CV
			template<typename Type>
			struct remove_cv
			{
				typedef typename remove_volatile<typename remove_const<Type>::type>::type type;
			};
//添加类型的cv属性
			template<typename Type>
			struct add_const
			{
				typedef const Type type;
			};
			template<typename Type>
			struct add_volatile
			{
				typedef volatile Type type;
			};
			template<typename Type>
			struct add_cv
			{
				typedef const volatile Type type;
			};
//添加引用
			template<typename Type>
			struct add_reference
			{
				typedef Type& type;
			};
			template<typename Type>
			struct add_lvalue_reference
			{
				typedef typename add_reference<Type>::type type;
			};
			template<typename Type>
			struct add_rvalue_reference
			{
				typedef Type&& type;
			};
//添加指针
			template<typename Type>
			struct add_pointer
			{
				typedef Type* type;
			};
			

//移除数组最顶层的[]符号
			template<typename Type>
			struct remove_bound
			{
				typedef Type type;
			};
			template<typename Type>
			struct remove_bound<Type[]>
			{
				typedef Type type;
			};
			template<typename Type,unsigned int Index>
			struct remove_bound<Type[Index]>
			{
				typedef Type type;
			};
			//移除数组全部[]符号
			template<typename Type>
			struct remove_all_bound
			{
				typedef Type type;
			};
			template<typename Type>
			struct remove_all_bound<Type[]>
			{
				typedef Type type;
			};
			template<typename Type,unsigned int Index>
			struct remove_all_bound<Type[Index]>
			{
				typedef typename remove_bound<Type>::type type;
			};
//移除最顶层指针
			template<typename Type>
			struct remove_pointer
			{
				typedef Type type;
			};
			template<typename Type>
			struct remove_pointer<Type*>
			{
				typedef Type type;
			};
//移除全部的指针*
			template<typename Type>
			struct remove_all_pointer
			{
				typedef Type type;
			};
			template<typename Type>
			struct remove_all_pointer<Type*>
			{
				typedef typename remove_pointer<Type>::type type;
			};
//移除引用
			template<typename Type>
			struct remove_reference
			{
				typedef Type type;
			};
			template<typename Type>
			struct remove_reference<Type&>
			{
				typedef Type type;
			};
			template<typename Type>
			struct remove_reference<Type&&>
			{
				typedef Type type;
			};

			/*主类型分类*/
#define COMPLIER_COMPLETE(class_name)\
	template<typename Type>\
			struct class_name : bool_predicate<__##class_name(Type)>\
			{};
#define COMPLIER_COMPLETE_TwoArg(class_name,from_type,to_type)\
	template<typename from_type,typename to_type>\
			struct class_name : bool_predicate<__##class_name(from_type,to_type)>\
			{};
			COMPLIER_COMPLETE(has_nothrow_assign);
			COMPLIER_COMPLETE(has_nothrow_constructor);
			COMPLIER_COMPLETE(has_nothrow_copy);
			COMPLIER_COMPLETE(is_nothrow_destructible);
			COMPLIER_COMPLETE(is_destructible);
			COMPLIER_COMPLETE(has_trivial_assign);
			COMPLIER_COMPLETE(has_trivial_constructor);
			COMPLIER_COMPLETE(has_trivial_copy);
			COMPLIER_COMPLETE(has_trivial_destructor);
			COMPLIER_COMPLETE(has_virtual_destructor);
			COMPLIER_COMPLETE(is_polymorphic);
			COMPLIER_COMPLETE(is_enum);
			COMPLIER_COMPLETE(is_pod);
			COMPLIER_COMPLETE(is_class);
			COMPLIER_COMPLETE(is_abstract);
			COMPLIER_COMPLETE(is_union);
			COMPLIER_COMPLETE(is_empty);
			COMPLIER_COMPLETE(is_trivial);
			COMPLIER_COMPLETE(is_literal_type);
			COMPLIER_COMPLETE(is_standard_layout);
			
			COMPLIER_COMPLETE_TwoArg(is_nothrow_assignable, ToType, FromType);
			COMPLIER_COMPLETE_TwoArg(is_trivially_assignable, ToType, FromType);
			COMPLIER_COMPLETE_TwoArg(is_constructible, Type, ArgsPack);
			COMPLIER_COMPLETE_TwoArg(is_trivially_constructible, Type, ArgsPack);
			COMPLIER_COMPLETE_TwoArg(is_nothrow_constructible, Type, ArgsPack);
			COMPLIER_COMPLETE_TwoArg(is_convertible_to, FromType, ToType);
	//		COMPLIER_COMPLETE(is_assignable);
	//		COMPLIER_COMPLETE(is_constructible);
	//		COMPLIER_COMPLETE(is_copy_assignable);
	//		COMPLIER_COMPLETE(is_copy_constructible);
	//		COMPLIER_COMPLETE(is_destructible);
	//		COMPLIER_COMPLETE(is_default_constructible);
	//		COMPLIER_COMPLETE(is_move_assignable);
	//		COMPLIER_COMPLETE(is_move_constructible);
	//		COMPLIER_COMPLETE(is_trivially_assignable);
	//		COMPLIER_COMPLETE(is_trivially_constructible);
	//		COMPLIER_COMPLETE(is_trivially_copy_assignable);
	//		COMPLIER_COMPLETE(is_trivially_copy_constructible);
	//		COMPLIER_COMPLETE(is_trivially_destructible);
		//	COMPLIER_COMPLETE(is_trivially_default_constructible);
	//		COMPLIER_COMPLETE(is_trivially_move_assignable);
	//		COMPLIER_COMPLETE(is_trivially_move_constructible);
	//		COMPLIER_COMPLETE(is_nothrow_assignable);
		//	COMPLIER_COMPLETE(is_nothrow_constructible);
		//	COMPLIER_COMPLETE(is_nothrow_copy_assignable);
		//	COMPLIER_COMPLETE(is_nothrow_copy_constructible);
		//	COMPLIER_COMPLETE(is_nothrow_destructible);
		//	COMPLIER_COMPLETE(is_nothrow_default_constructible);
		//	COMPLIER_COMPLETE(is_nothrow_move_assignable);
		//	COMPLIER_COMPLETE(is_nothrow_move_constructible);
#undef COMPLIER_COMPLETE
			template<typename FromType, typename ToType>
			struct is_convertible :is_convertible_to<FromType, ToType>
			{

			};
			template<typename Type>
			struct is_float_impl : false_type
			{

			};
			template< >
			struct is_float_impl<double> : true_type
			{

			};
			template<>
			struct is_float_impl<float> : true_type
			{

			};
			template<>
			struct is_float_impl<long double> : true_type
			{

			};
			template<typename Type>
			struct is_float_point : is_float_impl<typename remove_cv<Type>::type>
			{

			};
			
			template<typename Type>
			struct is_void_impl :false_type
			{

			};
			template<>
			struct is_void_impl<void> : true_type
			{

			};
			template<typename Type>
			struct is_void : is_void_impl<typename remove_cv<Type>::type>
			{

			};
			template<typename Type>
			struct is_array :false_type
			{

			};
			template<typename Type>
			struct is_array<Type[]>:true_type
			{

			};
			template<typename Type,typename size_t Index>
			struct is_array<Type[Index]>:true_type
			{

			};
			/*template<typename ClassType>
			struct is_class:false_type
			{

			};
			template<typename ClassType,typename ValueType>
			struct is_class<ValueType ClassType::*> : true_type
			{

			};*/
			
			template<typename ClassType>
			struct is_member_function_pointer :false_type
			{

			};
			//折腾了好久...原来这么简单= =
			template<typename ClassType, typename ValueType,typename... FuncArg>
			struct is_member_function_pointer<ValueType(ClassType::*)(FuncArg...)> : true_type
			{

			};
			template<typename ClassType>
			struct is_member_obejct_pointer:false_type
			{

			};
			template<typename ClassType, typename ValueType>
			struct is_member_obejct_pointer<ValueType(ClassType::*)> : bool_predicate<!is_member_function_pointer<ValueType(ClassType::*)>::value>
			{

			};
			template<typename Type>
			struct is_pointer : false_type
			{

			};
			template<typename Type>
			struct is_pointer<Type*> : bool_predicate<!is_member_obejct_pointer<Type*>::value&&!is_member_function_pointer<Type*>::value>
			{

			};
			template<typename Type>
			struct is_reference : false_type
			{

			};
			template<typename Type>
			struct is_reference<Type&> : true_type
			{

			};
			template<typename ValueType>
			struct is_function_impl : false_type
			{

			};
			template<typename ValueType,typename... FuncArg>
			struct is_function_impl<ValueType(*)(FuncArg...)>:true_type
			{

			};
			template<typename ValueType>
			struct is_function : is_function_impl<typename remove_cv<ValueType>::type>
			{

			};

			template<typename Type>
			struct is_lvalue_reference :false_type
			{

			};
			template<typename Type>
			struct is_lvalue_reference<Type&> :true_type
			{

			};
			template<typename Type>
			struct is_rvalue_reference :false_type
			{

			};
			template<typename Type>
			struct is_rvalue_reference<Type&&> :true_type
			{

			};
			template<typename Type>
			struct is_integral:false_type
			{

			};
#define IS_INTEGRAL(int_type) \
			template<>\
			struct is_integral<int_type> : true_type\
			{};

			IS_INTEGRAL(bool);
			IS_INTEGRAL(char);
			IS_INTEGRAL(wchar_t);
			IS_INTEGRAL(signed char);
			IS_INTEGRAL(short int);
			IS_INTEGRAL(int);
			IS_INTEGRAL(long int);
			IS_INTEGRAL(long long int);
			IS_INTEGRAL(unsigned char);
			IS_INTEGRAL(unsigned short int);
			IS_INTEGRAL(unsigned int);
			IS_INTEGRAL(unsigned long int);
			IS_INTEGRAL(unsigned long long int);
#undef IS_INTEGRAL
			/*次类型分类*/
			template<typename Type>
			struct is_signed : bool_predicate <((typename remove_cv<Type>::type)(-1) > (typename remove_cv<Type>::type)(0)) >
			{

			};
			template<typename Type>
			struct is_unsigned : bool_predicate < ((typename remove_cv<Type>::type)(-1) < (typename remove_cv<Type>::type)(0))>
			{

			};
			template<typename Type>
			struct is_arithmetic : bool_predicate<is_integral<Type>::value || is_float_point<Type>::value>
			{
				
			};
			template<typename Type>
			struct is_fundamental : bool_predicate<is_arithmetic<Type>::value || is_void<Type>::value>
			{

			};
			template<typename Type>
			struct is_compound : bool_predicate<!is_fundamental<Type>::value>
			{

			};
			
			template<typename Type>
			struct is_member_pointer : bool_predicate<is_member_obejct_pointer<Type>::value || is_member_function_pointer<Type>::value>
			{

			};
			template<typename Type>
			struct is_object : bool_predicate<!(is_function<Type>::value || is_reference<Type>::value||is_void<Type>::value)>
			{

			};
			template<typename Type>
			struct is_scalar : bool_predicate<
				is_arithmetic<Type>::value 
				|| is_enum<Type>::value
				|| is_pointer<Type>::value
				|| is_member_pointer<Type>::value>
			{

			};

			/*类型退化*/
			template<typename Type>
			struct decay
			{
				typedef typename remove_cv<typename remove_reference<Type>::type>::type non_ref_type;
				typedef typename if_else<is_array<non_ref_type>::value,
					typename remove_bound<non_ref_type>::type*,
					typename if_else<is_function<non_ref_type>::value,
					typename add_pointer<non_ref_type>,
					typename remove_cv<non_ref_type>::type>::type>::type type;
			};
			/*类型替换*/
			template<typename Tc, typename Ts, typename Tr, bool>
			struct replace_type_impl;
			template<typename Tc, typename Ts, typename Tr>
			struct replace_type;

			template<typename Tc, typename Ts, typename Tr>
			struct replace_type_impl<Tc, Ts, Tr, true>
			{
				typedef Tr type;
			};
			template<typename Tc, typename Ts, typename Tr>
			struct replace_type_impl<Tc*, Ts, Tr, false>
			{
				typedef typename replace_type<Tc, Ts, Tr>::type* type;
			};
			template<typename Tc, typename Ts, typename Tr>
			struct replace_type_impl<const Tc, Ts, Tr, false>
			{
				typedef typename const replace_type<Tc, Ts, Tr>::type type;
			};
			template<typename Tc, typename Ts, typename Tr>
			struct replace_type_impl<volatile Tc, Ts, Tr, false>
			{
				typedef typename volatile replace_type<Tc, Ts, Tr>::type type;
			};
			template<typename Tc, typename Ts, typename Tr>
			struct replace_type_impl<Tc&, Ts, Tr, false>
			{
				typedef typename replace_type<Tc, Ts, Tr>::type& type;
			};
			template<typename Tc, typename Ts, typename Tr, typename Class>
			struct replace_type_impl<Tc Class::*, Ts, Tr, false>
			{
				typedef typename replace_type<Tc, Ts, Tr>::type  Class::* type;
			};
			template<typename Tc, typename Ts, typename Tr>
			struct replace_type_impl<Tc&&, Ts, Tr, false>
			{
				typedef typename replace_type<Tc, Ts, Tr>::type& type;
			};
			template<typename Tc, typename Ts, typename Tr>
			struct replace_type_impl< Tc[], Ts, Tr, false>
			{
				typedef typename replace_type<Tc, Ts, Tr>::type type[];
			};
			template<typename Tc, typename Ts, typename Tr, size_t Index>
			struct replace_type_impl<Tc[Index], Ts, Tr, false>
			{
				typedef typename replace_type<Tc, Ts, Tr>::type type[Index];
			};
			template<typename Tc, typename Ts, typename Tr>
			struct replace_type
			{
				typedef typename replace_type_impl<Tc, Ts, Tr, std::is_same<Tc, Ts>::value>::type type;
			};
			/*右值转发*/
			template<typename Type> inline
				typename remove_reference<Type>::type&&
				move(Type&& target)
			{
					return static_cast<typename remove_reference<Type>::type&&>(target);
			};

			template<class Type> inline
				Type&& forward(typename remove_reference<Type>::type& Arg)
			{
					return static_cast<Type&&>(Arg);
			};

			template<class Type> inline
				Type&& forward(typename remove_reference<Type>::type&& Arg)
			{
					static_assert(!is_lvalue_reference<Type>::value, "bad forward call");
					return (static_cast<Type&&>(Arg));
			};
			template<typename Type>
			void swap(Type&& left, Type&& right)
			{
				auto temp = move(left);
				right = move(left);
				left = move(temp);
			}
		}
	}
	
}