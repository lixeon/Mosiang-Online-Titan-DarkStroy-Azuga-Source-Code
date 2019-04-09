#include "stdafx.h"
//#include "basenetworkinterface.h"


class CSC_BaseNetworkFactory : public IClassFactory
{
public:

	CSC_BaseNetworkFactory();
	virtual ~CSC_BaseNetworkFactory();
    STDMETHODIMP			QueryInterface(REFIID refiid, PPVOID ppv);
	STDMETHODIMP_(ULONG)	AddRef(void);
    STDMETHODIMP_(ULONG)	Release(void);
    STDMETHODIMP			CreateInstance(IUnknown*, REFIID, PPVOID);
    STDMETHODIMP			LockServer(BOOL);

private:
	DWORD						m_dwRefCount;
};