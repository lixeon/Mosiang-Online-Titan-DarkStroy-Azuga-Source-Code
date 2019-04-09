#include "factory.h"
#include "CoStorage.h"


CF4DyuchiFileStorage::CF4DyuchiFileStorage()
{
//	MessageBox(NULL,"created factory","hoho",NULL);
	m_dwRefCount = 0;
}
	

STDMETHODIMP CF4DyuchiFileStorage::QueryInterface(REFIID refiid, PPVOID ppv)
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

STDMETHODIMP_(ULONG) CF4DyuchiFileStorage::AddRef(void)
{
	m_dwRefCount++;
	return m_dwRefCount;
}
    
STDMETHODIMP_(ULONG) CF4DyuchiFileStorage::Release(void)
{
	m_dwRefCount--;
	if (!m_dwRefCount)
	{
		delete this;
	}
	return 0;
}
STDMETHODIMP CF4DyuchiFileStorage::CreateInstance(IUnknown* pUnknown, REFIID refiid, PPVOID ppv)
{ 
	HRESULT hr = E_FAIL;

	CoStorage* pStorage = NULL;
	*ppv = NULL;

	
	if (NULL != pUnknown && refiid != IID_IUnknown)
		hr = CLASS_E_NOAGGREGATION;
	else
	{
		hr = E_OUTOFMEMORY;
		pStorage = new CoStorage;
		if (pStorage)
		{
			hr = pStorage->QueryInterface(refiid,ppv);
			if (FAILED(hr))
			{
				delete pStorage;
				pStorage = NULL;
			}
		}
	}
	return hr;
}
STDMETHODIMP CF4DyuchiFileStorage::LockServer(BOOL)
{
	return 0;
}
CF4DyuchiFileStorage::~CF4DyuchiFileStorage()
{
	
//	MessageBox(NULL,"destoryed factory","hoho",NULL);
}