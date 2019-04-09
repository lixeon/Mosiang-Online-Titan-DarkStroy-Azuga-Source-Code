#pragma once
#include <unknwn.h>


class CTConsoleFactory : public IClassFactory
{
public:
	CTConsoleFactory():m_cRef(1){}
	virtual ~CTConsoleFactory(){}

	virtual HRESULT __stdcall QueryInterface(const IID& iid, void ** ppv);
	virtual ULONG __stdcall AddRef();
	virtual ULONG __stdcall Release();
	
	virtual HRESULT __stdcall CreateInstance(IUnknown * pUnknownOuter, const IID & iid, void ** ppv);
	virtual HRESULT __stdcall LockServer(BOOL bLock);
private:
	LONG m_cRef;
};