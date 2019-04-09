#pragma once

#include "../4DyuchiGRX_common/stdafx.h"

class CF4DyuchiFileStorage : public IClassFactory
{
public:    
    STDMETHODIMP			QueryInterface(REFIID refiid, PPVOID ppv);
	STDMETHODIMP_(ULONG)	AddRef(void);
    STDMETHODIMP_(ULONG)	Release(void);
    STDMETHODIMP			CreateInstance(IUnknown*, REFIID, PPVOID);
    STDMETHODIMP			LockServer(BOOL);
	CF4DyuchiFileStorage();
	~CF4DyuchiFileStorage();
	
private:
	DWORD					m_dwRefCount;

};
