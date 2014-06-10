#pragma once
#include "concept/ztl_base_concept.h"
namespace ztl
{
	template<typename FirstType, typename SecondType, typename ThirdType>
	class triple :public
		IAssignable<triple<FirstType, SecondType, ThirdType>>,
		IEquality<triple<FirstType, SecondType, ThirdType>>
	{
	public:
		typedef FirstType						first_type;
		typedef SecondType						second_type;
		typedef ThirdType						third_type;
		typedef triple<first_type, second_type, third_type>	self_type;

	public:
		first_type			first;
		second_type			second;
		third_type			third;
	public:	/*IAssignable*/
		triple() = default;
		triple(const self_type& target) :first(target.first), second(target.second), third(target.third)
		{
		}
		triple& operator=(const self_type& target)
		{
			first = target.first;
			second = target.second;
			third = target.third;
			return *this;
		}
		~triple() = default;
	public:/*IEquality*/
		bool operator== (const self_type& right)const
		{
			return this->first == right.first && this->second == right.second&& this->third == right.third;
		}
	public:

		triple(const first_type& left, const second_type& middle, const third_type right) :first(left), second(middle), third(right)
		{
		}
		template<typename T1, typename T2, typename T3>
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