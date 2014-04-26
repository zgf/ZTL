#pragma once
#include "Z_Exception.h"
namespace ztl
{

	class Contract
	{
	public:
		template<typename ExceptionType>
		static void Assert(const bool Expression, String::Pointer Message)
		{
			if(!Expression)
			{
				throw ExceptionType(Message);
			}
		}
		template<typename ExceptionType>
		static void Assert(const bool Expression, const ExceptionType& tException)
		{
			if(!Expression)
			{
				throw tException;
			}
		}
		static void AssertAbort(const bool Expression)
		{
			if(!Expression)
			{
				abort();
			}
		}

	};
}