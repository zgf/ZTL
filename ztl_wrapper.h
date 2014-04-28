#pragma once
#include "ztl_concept_base.h"
namespace ztl
{
	namespace wrapper
	{
		namespace tuples
		{
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