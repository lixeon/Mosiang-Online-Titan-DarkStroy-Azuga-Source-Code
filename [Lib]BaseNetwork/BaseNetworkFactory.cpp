// DBThreadFactory.cpp: implementation of the DBThreadFactory class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BaseNetworkFactory.h"
#include "basenetwork.h"

//#include "db.h"
//#include "dbclass.h"
static long g_ComponentCount = 0;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
STDAPI DllCanUnloadNow(void)
{
	if (g_ComponentCount)
		return S_FALSE;

	return S_OK;
}

CSC_BaseNetworkFactory::CSC_BaseNetworkFactory()
{
	m_dwRefCount = 0;
}

CSC_BaseNetworkFactory::~CSC_BaseNetworkFactory()
{
	g_ComponentCount--;
}


STDMETHODIMP CSC_BaseNetworkFactory::QueryInterface(REFIID refiid, PPVOID ppv)
{
	if (IID_IUnknown == refiid || IID_IClassFactory == refiid)
	{
		*ppv = this;
		((IUnknown*)(*ppv))->AddRef();
		return NOERROR;
	}
	
	*ppv = NULL;
	return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) CSC_BaseNetworkFactory::AddRef(void)
{
	InterlockedIncrement((LPLONG)&m_dwRefCount);
	return m_dwRefCount;
}
    
STDMETHODIMP_(ULONG) CSC_BaseNetworkFactory::Release(void)
{
	InterlockedDecrement((LPLONG)&m_dwRefCount);
	if (!m_dwRefCount)
	{
		delete this;
	}
	return 0;
}
STDMETHODIMP CSC_BaseNetworkFactory::CreateInstance(IUnknown* pUnknown, REFIID refiid, PPVOID ppv)
{ 
	HRESULT hr = E_FAIL;

	CSC_BaseNetwork* pCode = NULL;
	*ppv = NULL;

	
	if (NULL != pUnknown && refiid != IID_IUnknown)
		hr = CLASS_E_NOAGGREGATION;
	else
	{
		hr = E_OUTOFMEMORY;
		pCode = new CSC_BaseNetwork;
		if (pCode)
		{
			hr = pCode->QueryInterface(refiid,ppv);
			if (FAILED(hr))
			{
				delete pCode;
				pCode = NULL;
			}
			else g_ComponentCount++;
		}
	}
	return hr;
}
STDMETHODIMP CSC_BaseNetworkFactory::LockServer(BOOL)
{
	return 0;
}
