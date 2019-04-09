// MHNetwork.cpp: implementation of the CMHNetwork class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MHNetwork.h"
#include "mmsystem.h"
#include "ServerToolDoc.h"
#include "ServerToolView.h"

extern CServerToolView*	g_ServerToolView;

CMHNetwork* gNetwork = NULL;
HMODULE g_hBaseNetworkHandle = NULL;

GLOBALTON(CMHNetwork)

CMHNetwork::CMHNetwork()
{
	gNetwork = this;

	m_pBaseNetwork = NULL;
	memset( m_dwConnectionIndex, 0, sizeof(DWORD)*MAX_SERVERSET );
	memset( m_CheckSum, 0, sizeof(BYTE)*MAX_SERVERSET );

	m_hMainWnd = NULL;
}

CMHNetwork::~CMHNetwork()
{
	Release();
}

BOOL CMHNetwork::Init( HWND hWnd )
{
	m_hMainWnd = hWnd;
	
	HRESULT hr;
	
	char temp[256];
	strcpy( temp, GetDataDirectory() );
	strcat( temp, "BaseNetwork.dll" );
	//생성
	g_hBaseNetworkHandle = LoadLibrary( temp );

	DllGetClassObject_BaseNetwork	pNetFunc;
	pNetFunc = (DllGetClassObject_BaseNetwork)GetProcAddress(g_hBaseNetworkHandle,"DllGetClassObject");
	hr = pNetFunc(CLSID_SC_BASENETWORK_DLL, IID_SC_BASENETWORK_DLL, (void**)&m_pBaseNetwork);
		
	if( hr != S_OK )
	{
		MessageBox( NULL, "BASENETWORK.DLL Is Not Exist.", "Error", MB_OK );
		return FALSE;
	}
	
	//Network 초기화 
	DESC_BASENETWORK Desc;	memset(&Desc,0,sizeof(Desc));
	Desc.wSockEventWinMsgID	= WM_SOCKEVENTMSG;	
	Desc.OnConnect			= OnCon;
	Desc.OnDisconnect		= OnDisCon;
	Desc.ReceivedMsg		= OnRecv;
	
	m_pBaseNetwork->InitNetwork( m_hMainWnd, MAX_SERVERSET, &Desc );

	if( m_pBaseNetwork == NULL )
		return FALSE;
	
	return TRUE;
}

void CMHNetwork::Release()
{
	AllDisconnect();
	SAFE_RELEASE( m_pBaseNetwork );

	if( g_hBaseNetworkHandle )
	{
		FreeLibrary( g_hBaseNetworkHandle );
		g_hBaseNetworkHandle = NULL;
	}
}

BOOL CMHNetwork::ConnectToServer( int serverset, char* ip, WORD port )
{	
	m_dwConnectionIndex[serverset] = m_pBaseNetwork->ConnectToServer(ip,port);
	if( !m_dwConnectionIndex[serverset] )
		return FALSE;

	return TRUE;
}

void CMHNetwork::Disconnect( int serverset )
{
	if( m_dwConnectionIndex[serverset] )
	{
		m_pBaseNetwork->CompulsiveDisconnect( m_dwConnectionIndex[serverset] );
		m_dwConnectionIndex[serverset] = 0;		
	}
}

void CMHNetwork::AllDisconnect()
{
	if( m_pBaseNetwork )
	{
		for( int i = 0; i < MAX_SERVERSET; ++i )
			Disconnect( i );
	}
}

void CMHNetwork::Send( int serverset, MSGROOT* pMsg, int MsgLen )
{
	pMsg->CheckSum = m_CheckSum[serverset]++;
	if( m_dwConnectionIndex[serverset] )
		m_pBaseNetwork->Send( m_dwConnectionIndex[serverset], (char*)pMsg, MsgLen );
}

void CMHNetwork::SendAll( MSGROOT* pMsg, int MsgLen )
{
	for( int i = 0; i < MAX_SERVERSET; ++i )
		Send( i, pMsg, MsgLen );
}

//-------------------------------------------------------------------------------------------------
void OnCon( DWORD dwConIndex )
{	
}

void OnDisCon( DWORD dwConIndex )
{
	for( int i = 0; i < MAX_SERVERSET; ++i )
	{
		if( gNetwork->m_dwConnectionIndex[i] == dwConIndex )
		{
			gNetwork->m_dwConnectionIndex[i] = 0;
			if( g_ServerToolView )
				g_ServerToolView->Disconnect( i );			
			break;
		}
	}
}

void OnRecv( DWORD dwConIndex, char* pMsg, DWORD msglen )
{
	MSGROOT* pTemp = (MSGROOT*)pMsg;

	BOOL bRecv = FALSE;
	for( int i = 0; i < MAX_SERVERSET; ++i )
	{
		if( gNetwork->m_dwConnectionIndex[i] == dwConIndex )
		{
			bRecv = TRUE;
			break;
		}		
	}

	if( g_ServerToolView && bRecv )
		g_ServerToolView->NetworkMsgParse( i, pTemp->Category, pTemp->Protocol, pMsg );
}

