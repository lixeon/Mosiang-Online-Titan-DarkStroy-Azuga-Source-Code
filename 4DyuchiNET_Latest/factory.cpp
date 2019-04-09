#include "factory.h"
#include "conetwork.h"


CF4DyuchiNET::CF4DyuchiNET()
{
//	MessageBox(NULL,"created factory","hoho",NULL);
	m_dwRefCount = 0;
}
	

STDMETHODIMP CF4DyuchiNET::QueryInterface(REFIID refiid, PPVOID ppv)
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

STDMETHODIMP_(ULONG) CF4DyuchiNET::AddRef(void)
{
	m_dwRefCount++;
	return m_dwRefCount;
}
    
STDMETHODIMP_(ULONG) CF4DyuchiNET::Release(void)
{
	m_dwRefCount--;
	if (!m_dwRefCount)
	{
		delete this;
	}
	return 0;
}
STDMETHODIMP CF4DyuchiNET::CreateInstance(IUnknown* pUnknown, REFIID refiid, PPVOID ppv)
{ 
	HRESULT hr = E_FAIL;

	Co4DyuchiNET* pNet = NULL;
	*ppv = NULL;

	
	if (NULL != pUnknown && refiid != IID_IUnknown)
		hr = CLASS_E_NOAGGREGATION;
	else
	{
		hr = E_OUTOFMEMORY;
		pNet = new Co4DyuchiNET;
		if (pNet)
		{
			hr = pNet->QueryInterface(refiid,ppv);
			if (FAILED(hr))
			{
				delete pNet;
				pNet = NULL;
			}
		}
	}
	return hr;
}
STDMETHODIMP CF4DyuchiNET::LockServer(BOOL)
{
	return 0;
}
CF4DyuchiNET::~CF4DyuchiNET()
{
	
//	MessageBox(NULL,"destoryed factory","hoho",NULL);
}