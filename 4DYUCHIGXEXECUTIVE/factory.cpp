#include "factory.h"
#include "executive.h"


CF4DyuchiGXExecutive::CF4DyuchiGXExecutive()
{
//	MessageBox(NULL,"created factory","hoho",NULL);
	m_dwRefCount = 0;
}
	

STDMETHODIMP CF4DyuchiGXExecutive::QueryInterface(REFIID refiid, PPVOID ppv)
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

STDMETHODIMP_(ULONG) CF4DyuchiGXExecutive::AddRef(void)
{
	m_dwRefCount++;
	return m_dwRefCount;
}
    
STDMETHODIMP_(ULONG) CF4DyuchiGXExecutive::Release(void)
{
	m_dwRefCount--;
	if (!m_dwRefCount)
	{
		delete this;
	}
	return 0;
}
STDMETHODIMP CF4DyuchiGXExecutive::CreateInstance(IUnknown* pUnknown, REFIID refiid, PPVOID ppv)
{ 
	HRESULT hr = E_FAIL;

	CoExecutive* pExecutive = NULL;
	*ppv = NULL;

	
	if (NULL != pUnknown && refiid != IID_IUnknown)
		hr = CLASS_E_NOAGGREGATION;
	else
	{
		hr = E_OUTOFMEMORY;
		pExecutive = new CoExecutive;
		if (pExecutive)
		{
			hr = pExecutive->QueryInterface(refiid,ppv);
			if (FAILED(hr))
			{
				delete pExecutive;
				pExecutive = NULL;
			}
		}
	}
	return hr;
}
STDMETHODIMP CF4DyuchiGXExecutive::LockServer(BOOL)
{
	return 0;
}
CF4DyuchiGXExecutive::~CF4DyuchiGXExecutive()
{
	
//	MessageBox(NULL,"destoryed factory","hoho",NULL);
}