#include "TConsoleFactory.h"
#include "TConsole.h"

HRESULT __stdcall CTConsoleFactory::QueryInterface(const IID& iid, void ** ppv)
{
	if(iid == IID_IClassFactory || iid == IID_IUnknown)
	{
		*ppv = static_cast<IClassFactory*>(this);
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}

ULONG __stdcall CTConsoleFactory::AddRef()
{
	return InterlockedIncrement(&m_cRef);
}

ULONG __stdcall CTConsoleFactory::Release()
{
	if(InterlockedDecrement(&m_cRef) == 0)
	{
		delete this;
		return 0;
	}
	return m_cRef;
}

HRESULT __stdcall CTConsoleFactory::CreateInstance(IUnknown * pUnknownOuter, const IID & iid, void ** ppv)
{
	if(pUnknownOuter != NULL)
	{
		return CLASS_E_NOAGGREGATION;
	}

	CTConsole * pTConsole = new CTConsole;
	if(!pTConsole) return E_OUTOFMEMORY;

	HRESULT hr = pTConsole->QueryInterface(iid, ppv);
	pTConsole->Release();

	return hr;
}

HRESULT __stdcall CTConsoleFactory::LockServer(BOOL bLock)
{
	if(bLock)
	{
		//InterlockedIncrement(&);
	}
	else
	{
		//InterlockedDecrement(&);
	}
	return S_OK;
}