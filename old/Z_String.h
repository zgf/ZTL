#pragma once
#include <string>
namespace ztl
{

	template<typename StringType = std::string>
	class BaseString
	{
	public:
		typedef typename StringType::value_type ValueType;
		typedef typename StringType::pointer Pointer
			typedef typename StringType::reference Reference;
		typedef typename StringType::difference_type Distance;
		BaseString()
		{

		}
		BaseString(const char* tString) : Data(tString)
		{

		}
		BaseString(const string& tString) :Data(tString)
		{

		}
		BaseString(const BaseString& tString) :Data(tString.Data)
		{

		}
		~BaseString()
		{

		}
	public:
		BaseString& operator=( const string& tString )
		{
			Data = tString;
		}
		BaseString& operator=( const BaseString& tString )
		{
			*this = tString;
		}
		BaseString& operator=( const char* tString )
		{
			Data = tString;
		}

	private:
		StringType Data;
	};
	typedef BaseString<std::string> String;
}