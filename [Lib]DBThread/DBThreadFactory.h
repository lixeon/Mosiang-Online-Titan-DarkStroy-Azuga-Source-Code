// DBThreadFactory.h: interface for the DBThreadFactory class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "stdafx.h"
#include "db.h"
//#include "dbthreadguid.h"

class DBThreadFactory : public IClassFactory  
{
public:
	DBThreadFactory();
	virtual ~DBThreadFactory();
    STDMETHODIMP			QueryInterface(REFIID refiid, PPVOID ppv);
	STDMETHODIMP_(ULONG)	AddRef(void);
    STDMETHODIMP_(ULONG)	Release(void);
    STDMETHODIMP			CreateInstance(IUnknown*, REFIID, PPVOID);
    STDMETHODIMP			LockServer(BOOL);
private:
	DWORD					m_dwRefCount;

};

