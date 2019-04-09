#include "stdafx.h"
#include "RMNetwork.h"

BOOL bReceiving = FALSE;		// °Ë»ç¿ë... ¸Þ¼¼Áö¸¦ ¹Þ´Â Áß¿¡ m_pCurStateÀÇ º¯È­°¡ ÀÏ¾î³ª¸é ¾ÈµÈ´Ù.
GLOBALTON(CRMNetwork)
CRMNetwork* gNetwork = NULL;	//????????????????????
HMODULE g_hBaseNetworkHandle = NULL;

CRMNetwork::CRMNetwork()
{
	m_ConnectionIndex = NULL;
	m_CheckSum = 0;

	gNetwork = this;


	
#ifdef _DEBUG
	memset(m_MsgIn,0,sizeof(DWORD)*MP_MAX);
	memset(m_MsgOut,0,sizeof(DWORD)*MP_MAX);
#endif

}

CRMNetwork::~CRMNetwork()
{
	Release();
}

void CRMNetwork::Release()
{
	if(m_pBaseNetwork)
	{
		Disconnect();
	}
	
	SAFE_RELEASE(m_pBaseNetwork);

	if(g_hBaseNetworkHandle)
	{
		FreeLibrary( g_hBaseNetworkHandle );
		g_hBaseNetworkHandle = NULL;
	}
}


BOOL CRMNetwork::Init(HWND hWnd)
{
	m_hMainWnd = hWnd;
	
	HRESULT hr;
	
	//»ý¼º
	g_hBaseNetworkHandle = LoadLibrary("./Data/BaseNetwork.dll");

	DllGetClassObject_BaseNetwork	pNetFunc;
	pNetFunc = (DllGetClassObject_BaseNetwork)GetProcAddress(g_hBaseNetworkHandle,"DllGetClassObject");
	hr = pNetFunc(CLSID_SC_BASENETWORK_DLL, IID_SC_BASENETWORK_DLL, (void**)&m_pBaseNetwork);
		
	if (hr != S_OK)
	{
		MessageBox( NULL,"There is no 'BASENETWORK.DLL'!!","Error",MB_OK);
		return FALSE;
	}
	
	//Network ÃÊ±âÈ­ 
	DESC_BASENETWORK Desc;	memset(&Desc,0,sizeof(Desc));
	Desc.wSockEventWinMsgID = WM_SOCKEVENTMSG;	
	Desc.OnConnect			= OnCon;
	Desc.OnDisconnect		= OnDisCon;
	Desc.ReceivedMsg		= OnRecv;
	
	m_pBaseNetwork->InitNetwork(m_hMainWnd,5,&Desc);

	if(m_pBaseNetwork == NULL)
		return FALSE;
	
	return TRUE;
}

BOOL CRMNetwork::ConnectToServer(char* ip,WORD port)
{	
	m_ConnectionIndex = m_pBaseNetwork->ConnectToServer(ip,port);
	if(!m_ConnectionIndex)
		return FALSE;
	
	return TRUE;
}

void CRMNetwork::Disconnect()
{
	if(m_ConnectionIndex)
	{
		m_pBaseNetwork->CompulsiveDisconnect(m_ConnectionIndex);
		m_ConnectionIndex = 0;		
	}
}

void CRMNetwork::SetCurState(CGameState* pState)
{
	m_pCurState = pState;
	
	//////////////////////////////////////////////////////////////////////////
	// for test
	m_RecvCount = 0;
	m_RecvedDataSize = 0;
}

void CRMNetwork::Send(MSGROOT* pMsg,int MsgLen)
{
	ASSERT(pMsg->Category < MP_MAX);
#ifdef _DEBUG
	AddMsgIn(pMsg->Category,MsgLen);
#endif
#ifdef _CRYPTCHECK_ 
/*/	int ssize = sizeof( MSGROOT );
	if( !m_crypt.Encrypt( ( char * )pMsg + ssize, MsgLen - ssize ) )
	{
		ASSERTMSG(0,"Encrypt Error");
		return;
	}
	pMsg->Code = m_crypt.GetEnCRCConvertChar();
	*/
#endif	

	pMsg->CheckSum = m_CheckSum++;
	if(m_ConnectionIndex)
	m_pBaseNetwork->Send(m_ConnectionIndex,(char*)pMsg,MsgLen);
}

//////////////////////////////////////////////////////////////////////////

void OnCon(DWORD dwConIndex)
{
	int a=0;
}

void OnDisCon(DWORD dwConIndex)
{
//	gNetwork->m_pCurState->OnDisconnect();

#ifdef _RMTOOL_
	if( AfxGetMainWnd()->ContinueModal() )
	{
		AfxGetMainWnd()->EndModalLoop(5000);
		AfxGetMainWnd()->EndModalState();
		return;
	}
#endif //_RMTOOL_

	MessageBox( NULL, "Disconnect To MAS!\nTry To Reconnect!!", "Error!!", MB_OK );
	SendMessage( gNetwork->m_hMainWnd, WM_CLOSE, 0, 0 );
}

//#define PRTOCOL_LOG
void OnRecv(DWORD dwConIndex,char* pMsg,DWORD msglen)
{
	bReceiving = TRUE;
//	ASSERT(gNetwork->m_pCurState);

	MSGROOT* pTemp = (MSGROOT*)pMsg;
#ifdef _CRYPTCHECK_ 
/*	int headerSize = sizeof( MSGROOT );
	if( !gNetwork->m_crypt.Decrypt( ( char * )pTemp + headerSize, msglen - headerSize ) )
	{
		ASSERTMSG(0,"Decrypt Error");
		return;
	}
	if( pTemp->Code != gNetwork->m_crypt.GetDeCRCConvertChar() )
	{
		ASSERTMSG(0,"Decrypt CRC Error");
		return;
	}
*/
#endif	
#ifdef _DEBUG
//	gNetwork->AddMsgOut(pTemp->Category,msglen);
#endif

	NetworkMsgParse( pTemp->Category, pTemp->Protocol, pMsg );
//	if( g_ServerToolView )
//		g_ServerToolView->NetworkMsgParse( pTemp->Category, pTemp->Protocol, pMsg );
	
	//////////////////////////////////////////////////////////////////////////
	// for test	
	++gNetwork->m_RecvCount;
	gNetwork->m_RecvedDataSize += msglen;
	
#ifdef PRTOCOL_LOG
	FILE* fp = fopen("log_protocol.txt","a+");
#define PLOGSTART	if(0)	{
#define PLOG(cat,pro)	} else if(pTemp->Category == cat && pTemp->Protocol == pro)	{
#define PLOGEND		}


	PLOGSTART
		PLOG(MP_ATTACK,MP_ATTACK_ACK)
			M2C_ATTACK* pmsg = (M2C_ATTACK*)pTemp;
			fprintf(fp,"MP_ATTACK_ACK    A.ID : %d    T.ID : %d \n",pmsg->dwAttackerID,pmsg->dwTargetObjectID);
		PLOG(MP_USERCONN,MP_USERCONN_MONSTER_ADD)
			SEND_MONSTER_TOTALINFO* pmsg = (SEND_MONSTER_TOTALINFO*)pMsg;
			fprintf(fp,"MP_USERCONN_MONSTER_ADD       ID : %d \n",pmsg->BaseObjectInfo.dwObjectID);
		PLOG(MP_USERCONN,MP_USERCONN_OBJECT_REMOVE)
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			fprintf(fp,"MP_USERCONN_MONSTER_REMOVE       ID : %d \n",pmsg->dwData);
	PLOGEND

	fclose(fp);
#endif

	bReceiving = FALSE;
}

