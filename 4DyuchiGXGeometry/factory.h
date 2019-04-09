#pragma once

#include "../4DyuchiGRX_common/stdafx.h"

class CF4DyuchiGXGeometry : public IClassFactory
{
public:    
    STDMETHODIMP			QueryInterface(REFIID refiid, PPVOID ppv);
	STDMETHODIMP_(ULONG)	AddRef(void);
    STDMETHODIMP_(ULONG)	Release(void);
    STDMETHODIMP			CreateInstance(IUnknown*, REFIID, PPVOID);
    STDMETHODIMP			LockServer(BOOL);
	CF4DyuchiGXGeometry();
	~CF4DyuchiGXGeometry();
	
private:
	DWORD					m_dwRefCount;

};
