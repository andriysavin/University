//////////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////////
#include <stdexcept> 
#include <string> 
#include <wtypes.h> 
#include <comdef.h> 
//////////////////////////////////////////////////////////////////////////
class CW32Exception :public std::exception
{
private:
	static const std::string GetExceptionMessage(const std::string& sErrorDesc, HRESULT hr)
	{
		_com_error err(hr);
		return sErrorDesc + std::string(err.ErrorMessage());
	}
public:
	CW32Exception(const std::string& sErrorDesc, HRESULT hr) :
		std::exception(std::exception(GetExceptionMessage(sErrorDesc, hr).c_str()))
	{
	}
};
//////////////////////////////////////////////////////////////////////////
inline void TestResult(HRESULT hr, const std::string& sErrorDesc)
{
	if (hr != S_OK) throw(CW32Exception(sErrorDesc, hr));
}
//////////////////////////////////////////////////////////////////////////