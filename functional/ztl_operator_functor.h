#pragma once
#include "../type/ztl_type_traits.h"
namespace ztl
{

#define Product_Binary_Operator_Functor(functor_name,operator_name)\
	template<class Type = void> \
	struct functor_name\
	{	\
	Type operator()(const Type& left, const Type& right) const\
	{	\
	return (left operator_name right);\
	}\
	};\
	template<>\
	struct functor_name<void>\
	{	\
	template<class LeftType,class RightType>\
	auto operator()(LeftType&& left, RightType&& right) const\
	-> decltype(ztl::forward<LeftType>(left) operator_name ztl::forward<RightType>(right))\
	{	\
	return ztl::forward<LeftType>(left) operator_name ztl::forward<RightType>(right);\
	}\
	};

	Product_Binary_Operator_Functor(plus, +);
	Product_Binary_Operator_Functor(minus, -);
	Product_Binary_Operator_Functor(multiplies, *);
	Product_Binary_Operator_Functor(divides, / );
	Product_Binary_Operator_Functor(modulus, %);
	Product_Binary_Operator_Functor(equal_to, == );
	Product_Binary_Operator_Functor(not_equal_to, != );
	Product_Binary_Operator_Functor(greater, > );
	Product_Binary_Operator_Functor(less, < );
	Product_Binary_Operator_Functor(greater_equal, >= );
	Product_Binary_Operator_Functor(less_equal, <= );
	Product_Binary_Operator_Functor(logical_and, &&);
	Product_Binary_Operator_Functor(logical_or, || );
	Product_Binary_Operator_Functor(bit_and, &);
	Product_Binary_Operator_Functor(bit_or, | );
	Product_Binary_Operator_Functor(bit_xor, ^);
#undef Product_Binary_Operator_Functor

#define Product_Unary_Operator_Functor(functor_name,operator_name)\
	template<class Type = void> \
	struct functor_name\
	{	\
	Type operator()(const Type& value) const\
	{	\
	return (operator_name value);\
	}\
	};\
	template<>\
	struct functor_name<void>\
	{	\
	template<class LeftType>\
	auto operator()(LeftType&& value) const\
	-> decltype(operator_name ztl::forward<LeftType>(value))\
	{	\
	return operator_name ztl::forward<RightType>(value);\
	}\
	};
	Product_Unary_Operator_Functor(logical_not, !);
	Product_Unary_Operator_Functor(bit_not, ~);
	Product_Unary_Operator_Functor(negate, -);

#undef Product_Unary_Operator_Functor
	//一元谓词否定
	template<typename UnaryPredicateType>
	struct unary_negate
	{
		UnaryPredicateType pred;
		unary_negate(const UnaryPredicateType& tpred) :pred(tpred)
		{

		}
		unary_negate(UnaryPredicateType&& tpred) :pred(ztl::move(tpred))
		{

		}
		template<typename Type>
		bool operator()(Type&& arg) const
		{
			return !pred(ztl::forward<Type>(arg));
		}
	};
	//二元否定
	template<typename BinaryPredicateType>
	struct binary_negate
	{
		BinaryPredicateType pred;
		binary_negate() = delete;
		binary_negate(const BinaryPredicateType& tpred) :pred(tpred)
		{

		}
		binary_negate(BinaryPredicateType&& tpred) :pred(ztl::move(tpred))
		{

		}
		template<typename LeftType,typename RightType>
		bool operator()(LeftType&& left,RightType&& right) /*const*/
		{
			return !pred(ztl::forward<LeftType>(left),ztl::forward<RightType>(right));
		}
	};
	//生成仿函数的交换参数的仿函数版本
	template<typename Functor>
	struct swap_argement
	{
		swap_argement() = delete;
		swap_argement(Functor&& tfunctor) : functor(tfunctor)
		{

		}
		swap_argement(const Functor& tfunctor) :functor(tfunctor)
		{

		}
		template<typename LeftType,typename RightType>
		auto operator()(LeftType&& left, RightType&& right)->decltype(functor(right, left))
		{
			return functor(right, left);
		}
		Functor functor;
	};
}