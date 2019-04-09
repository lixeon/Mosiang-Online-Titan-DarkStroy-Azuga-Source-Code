#include "stdafx.h"
#include "basenetwork.h"
#include "network.h"
#include "basenetworkinterface.h"
#include "msgproc.h"

CSC_BaseNetwork::CSC_BaseNetwork()
{
	m_dwRefCount			=	0;
	g_pNetwork				=	NULL;

}

CSC_BaseNetwork::~CSC_BaseNetwork()
{
	if (g_pNetwork)
		delete g_pNetwork;
	g_pNetwork = NULL;
}

STDMETHODIMP CSC_BaseNetwork::QueryInterface(REFIID refiid, PPVOID ppv)
{
	if (IID_IUnknown == refiid)
		*ppv = this;
	else if (IID_SC_BASENETWORK_DLL == refiid)
		*ppv = (ISC_BaseNetwork*)this;
	else 
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	((IUnknown*)(*ppv))->AddRef();
	return NOERROR;
	
}
STDMETHODIMP_(ULONG) CSC_BaseNetwork::AddRef(void)
{
	InterlockedIncrement((LPLONG)&m_dwRefCount);
	return m_dwRefCount;
}

STDMETHODIMP_(ULONG) CSC_BaseNetwork::Release(void)
{

	InterlockedDecrement((LPLONG)&m_dwRefCount);
	if (!m_dwRefCount)
		delete this;
	return 0;
}

DWORD __stdcall CSC_BaseNetwork::InitNetwork(HWND hwnd, DWORD dwMaxConnect,DESC_BASENETWORK* pDesc )
{
	if (g_pNetwork) return 0;
	g_pNetwork = new CNetwork( hwnd, dwMaxConnect, pDesc->wSockEventWinMsgID);
	if (!g_pNetwork) return 0;

	g_pNetwork->ReceivedMsg = pDesc->ReceivedMsg;
	g_pNetwork->OnDisconnect = pDesc->OnDisconnect;
	g_pNetwork->OnConnect = pDesc->OnConnect;

	return 1;
}

BOOL __stdcall CSC_BaseNetwork::Send(DWORD dwConnectionIndex,char* msg,DWORD length)
{
	return g_pNetwork->Send(dwConnectionIndex,msg,length);
}
void CSC_BaseNetwork::CompulsiveDisconnect(DWORD dwIndex)
{
	g_pNetwork->CompulsiveDisconnect(dwIndex);
}
BOOL __stdcall CSC_BaseNetwork::StartServer(char* ip,WORD port)
{
	return g_pNetwork->StartServer(ip,port);
}

void __stdcall CSC_BaseNetwork::CloseAllConnection()
{
	g_pNetwork->CloseAllConnection();
}


void __stdcall CSC_BaseNetwork::CloseConnection(DWORD dwIndex)
{
	g_pNetwork->CloseConnection(dwIndex);
}

DWORD __stdcall CSC_BaseNetwork::ConnectToServer(char* szIP,WORD port)
{
	return g_pNetwork->ConnectToServer(szIP,port);
}

void  __stdcall CSC_BaseNetwork::SocketEventProc()
{
	SocketEventProc1();
}
