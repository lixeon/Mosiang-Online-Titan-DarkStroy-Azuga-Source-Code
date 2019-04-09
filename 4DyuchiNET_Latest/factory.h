#pragma once

#include "../4DyuchiNET_Common/stdafx.h"


class CF4DyuchiNET : public IClassFactory
{
public:    
    STDMETHODIMP			QueryInterface(REFIID refiid, PPVOID ppv);
	STDMETHODIMP_(ULONG)	AddRef(void);
    STDMETHODIMP_(ULONG)	Release(void);
    STDMETHODIMP			CreateInstance(IUnknown*, REFIID, PPVOID);
    STDMETHODIMP			LockServer(BOOL);
	CF4DyuchiNET();
	~CF4DyuchiNET();
	
private:
	DWORD					m_dwRefCount;

};
