#include "factory.h"
#include "coGeometry.h"


CF4DyuchiGXGeometry::CF4DyuchiGXGeometry()
{
//	MessageBox(NULL,"created factory","hoho",NULL);
	m_dwRefCount = 0;
}
	

STDMETHODIMP CF4DyuchiGXGeometry::QueryInterface(REFIID refiid, PPVOID ppv)
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

STDMETHODIMP_(ULONG) CF4DyuchiGXGeometry::AddRef(void)
{
	m_dwRefCount++;
	return 0;
}
    
STDMETHODIMP_(ULONG) CF4DyuchiGXGeometry::Release(void)
{
	m_dwRefCount--;
	if (!m_dwRefCount)
	{
		delete this;
	}
	return 0;
}
STDMETHODIMP CF4DyuchiGXGeometry::CreateInstance(IUnknown* pUnknown, REFIID refiid, PPVOID ppv)
{ 
	HRESULT hr = E_FAIL;

	CoGeometry* pGeometry = NULL;
	*ppv = NULL;

	
	if (NULL != pUnknown && refiid != IID_IUnknown)
		hr = CLASS_E_NOAGGREGATION;
	else
	{
		hr = E_OUTOFMEMORY;
		pGeometry = new CoGeometry;
		if (pGeometry)
		{
			hr = pGeometry->QueryInterface(refiid,ppv);
			if (FAILED(hr))
			{
				delete pGeometry;
				pGeometry = NULL;
			}
		}
	}
	return hr;
}
STDMETHODIMP CF4DyuchiGXGeometry::LockServer(BOOL)
{
	return 0;
}
CF4DyuchiGXGeometry::~CF4DyuchiGXGeometry()
{
	
//	MessageBox(NULL,"destoryed factory","hoho",NULL);
}