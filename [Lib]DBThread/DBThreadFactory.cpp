// DBThreadFactory.cpp: implementation of the DBThreadFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "db.h"
#include "dbclass.h"
#include "DBThreadFactory.h"
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

DBThreadFactory::DBThreadFactory()
{
	m_dwRefCount = 0;
}

DBThreadFactory::~DBThreadFactory()
{
	g_ComponentCount--;
}


STDMETHODIMP DBThreadFactory::QueryInterface(REFIID refiid, PPVOID ppv)
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

STDMETHODIMP_(ULONG) DBThreadFactory::AddRef(void)
{
	InterlockedIncrement((LPLONG)&m_dwRefCount);
	return m_dwRefCount;
}
    
STDMETHODIMP_(ULONG) DBThreadFactory::Release(void)
{
	InterlockedDecrement((LPLONG)&m_dwRefCount);
	if (!m_dwRefCount)
	{
		delete this;
	}
	return 0;
}
STDMETHODIMP DBThreadFactory::CreateInstance(IUnknown* pUnknown, REFIID refiid, PPVOID ppv)
{ 
	HRESULT hr = E_FAIL;

	CODB* pCode = NULL;
	*ppv = NULL;

	
	if (NULL != pUnknown && refiid != IID_IUnknown)
		hr = CLASS_E_NOAGGREGATION;
	else
	{
		hr = E_OUTOFMEMORY;
		pCode = new CODB;
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
STDMETHODIMP DBThreadFactory::LockServer(BOOL)
{
	return 0;
}
