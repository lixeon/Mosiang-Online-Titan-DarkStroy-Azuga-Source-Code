#pragma once

#include "stdafx.h"
#include "basenetworkinterface.h"
#include "network.h"
#include "struct.h"

class CSC_BaseNetwork : public ISC_BaseNetwork
{
public:
    STDMETHODIMP						QueryInterface(REFIID, PPVOID);
	STDMETHODIMP_(ULONG)				AddRef(void);
    STDMETHODIMP_(ULONG)				Release(void);
	void	__stdcall					CloseAllConnection();
	void	__stdcall					CloseConnection(DWORD dwIndex);
	BOOL	__stdcall					Send(DWORD dwConnectionIndex,char* msg,DWORD length);
	DWORD	__stdcall					ConnectToServer(char* szIP,WORD port);
	BOOL	__stdcall					StartServer(char* ip,WORD port);
	void	__stdcall					CompulsiveDisconnect(DWORD dwIndex);
	DWORD	__stdcall					InitNetwork(HWND hwnd, DWORD dwMaxConnect,DESC_BASENETWORK* pDesc );
	void  __stdcall						SocketEventProc();

	CSC_BaseNetwork();
	virtual ~CSC_BaseNetwork();

private:
	DWORD						m_dwRefCount;
};


