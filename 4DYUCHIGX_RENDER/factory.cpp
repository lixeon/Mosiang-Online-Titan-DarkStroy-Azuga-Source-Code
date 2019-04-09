#include "factory.h"
#include "CoD3DDevice.h"


CF4DyuchiGXRenderer::CF4DyuchiGXRenderer()
{
//	MessageBox(NULL,"created factory","hoho",NULL);
	m_dwRefCount = 0;
}
	

STDMETHODIMP CF4DyuchiGXRenderer::QueryInterface(REFIID refiid, PPVOID ppv)
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

STDMETHODIMP_(ULONG) CF4DyuchiGXRenderer::AddRef(void)
{
	m_dwRefCount++;
	return m_dwRefCount;
}
    
STDMETHODIMP_(ULONG) CF4DyuchiGXRenderer::Release(void)
{
	m_dwRefCount--;
	if (!m_dwRefCount)
	{
		delete this;
	}
	return 0;
}
STDMETHODIMP CF4DyuchiGXRenderer::CreateInstance(IUnknown* pUnknown, REFIID refiid, PPVOID ppv)
{ 
	HRESULT hr = E_FAIL;

	CoD3DDevice* pRenderer = NULL;
	*ppv = NULL;

	
	if (NULL != pUnknown && refiid != IID_IUnknown)
		hr = CLASS_E_NOAGGREGATION;
	else
	{
		hr = E_OUTOFMEMORY;
		pRenderer = new CoD3DDevice;
		if (pRenderer)
		{
			hr = pRenderer->QueryInterface(refiid,ppv);
			if (FAILED(hr))
			{
				delete pRenderer;
				pRenderer = NULL;
			}
		}
	}
	return hr;
}
STDMETHODIMP CF4DyuchiGXRenderer::LockServer(BOOL)
{
	return 0;
}
CF4DyuchiGXRenderer::~CF4DyuchiGXRenderer()
{
	
//	MessageBox(NULL,"destoryed factory","hoho",NULL);
}