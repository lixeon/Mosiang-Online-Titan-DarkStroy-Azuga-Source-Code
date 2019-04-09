#include "BaseFontObject.h"

STDMETHODIMP CBaseFontObject::QueryInterface(REFIID refiid, PPVOID ppv)
{
	*ppv = NULL;
	return E_NOINTERFACE;
}
STDMETHODIMP_(ULONG) CBaseFontObject::AddRef(void)
{
	m_dwRefCount++;
	return m_dwRefCount;
}
STDMETHODIMP_(ULONG) CBaseFontObject::Release(void)
{
	m_dwRefCount--;
	if (!m_dwRefCount)
		delete this;

	return 0;

}

CBaseFontObject::~CBaseFontObject()
{
}