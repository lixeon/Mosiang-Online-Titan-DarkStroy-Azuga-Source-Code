#pragma once

#include "../4DyuchiGRX_common/stdafx.h"

class CF4DyuchiGXRenderer : public IClassFactory
{
public:    
    STDMETHODIMP			QueryInterface(REFIID refiid, PPVOID ppv);
	STDMETHODIMP_(ULONG)	AddRef(void);
    STDMETHODIMP_(ULONG)	Release(void);
    STDMETHODIMP			CreateInstance(IUnknown*, REFIID, PPVOID);
    STDMETHODIMP			LockServer(BOOL);
	CF4DyuchiGXRenderer();
	~CF4DyuchiGXRenderer();
	
private:
	DWORD					m_dwRefCount;

};
