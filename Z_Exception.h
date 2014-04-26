#pragma once;
#include "Z_String.h"
namespace ztl
{
	//以后替换成自己的String实现
	/*返回的异常没想catch,只是想返回错误消息而已*/
	typedef ztl::String::Pointer Pointer;
	typedef ztl::String::ValueType ValueType;
	class Exception
	{
	public:
		Exception()
		{

		}

		Exception(const Pointer tMessage) :Message(tMessage)
		{

		}
		Exception(const String& tMessage) :Message(tMessage)
		{

		}
		Exception(Exception& tTarget) :Message(tTarget.Message)
		{

		}
		Exception& operator=( const Exception& tTarget )
		{
			this->Message = tTarget.What();
		}
		Exception& operator=( const String& tMessage )
		{
			this->Message = tMessage;
		}
		Exception& operator=( const Pointer tMessage )
		{
			this->Message = tMessage;
		}
		virtual ~Exception()
		{

		}
		virtual const String& What() const
		{
			return Message;
		}
	private:
		String Message;
	};

	class ArgumentException : public Exception
	{
	public:
		ArgumentException()
		{

		}
		ArgumentException(const String& tMessage, 
			const String& tFunctionName = String(), const String& tParameterName = String())
			:Exception(tMessage), 
			FunctionName(tFunctionName), ParameterName(tParameterName)
		{
			
		}
		ArgumentException(const Pointer tMessage,
			const Pointer tFunctionName = ValueType(), const Pointer tParameterName = ValueType())
			:Exception(tMessage),
			FunctionName(tFunctionName), ParameterName(tParameterName)
		{

		}
		ArgumentException(const ArgumentException& Target)
		{
			this->FunctionName = Target.FunctionName;
			this->ParameterName = Target.ParameterName;
		}
		virtual ~ArgumentException() 
		{

		}
		ArgumentException& operator=( const ArgumentException& Target )
		{
			this->FunctionName = Target.FunctionName;
			this->ParameterName = Target.ParameterName;
		}
	public:
		const String& GetFunctionName() const
		{
			return FunctionName;
		}
		const String& GetParameterName() const
		{
			return ParameterName;
		}
		const String& What() const override
		{
			return Exception::What();
		}
	protected:
		String FunctionName;
		String ParameterName;
	};

}