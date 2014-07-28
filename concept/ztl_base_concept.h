#pragma once
namespace ztl
{
	class INocopyable
	{
		INocopyable& operator=(const INocopyable&);
		INocopyable(const INocopyable&);
	};
	template<typename DerivedType>
	class IAssignable
	{
	public:
		DerivedType& operator=(const DerivedType& target);
	};

	template<typename DerivedType>
	class IEquality
	{
	public:
		bool operator== (const DerivedType& right) const;
	public:
		friend bool operator!= (const DerivedType& left, const DerivedType& right)
		{
			return !(left == right);
		}
	};
	template<typename DerivedType>
	class IOrdering
	{
	public:
		bool operator<  (const DerivedType& right)const;
	public:
		friend bool operator<= (const DerivedType& left, const DerivedType& right)
		{
			return !(left < right);
		}
		friend bool operator>(const DerivedType& left, const DerivedType& right)
		{
			return right < left;
		}
		friend bool operator>= (const DerivedType& left, const DerivedType& right)
		{
			return !(right < left);
		}
	};
}