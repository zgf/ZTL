#pragma once
#include "concept/ztl_base_concept.h"
namespace ztl
{
	template<typename FirstType, typename SecondType>
	class pair :
		virtual public IAssignable<pair<FirstType, SecondType>>,
		virtual public IEquality<pair<FirstType, SecondType>>
	{
	public:
		typedef FirstType						first_type;
		typedef SecondType						second_type;
		typedef pair<first_type, second_type>	self_type;
	public:
		first_type			first;
		second_type			second;
	public:/*IEquality*/
		bool operator== (const self_type& right) const
		{
			return first == right.first && second == right.second;
		}
	public:	/*IAssignable*/
		pair() = default;
		pair(const self_type& target) :first(target.first), second(target.second)
		{
		}
		pair& operator=(const self_type& target)
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
}