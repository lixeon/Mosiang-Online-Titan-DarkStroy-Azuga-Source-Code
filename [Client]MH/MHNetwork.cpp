// MHNetwork.cpp: implementation of the CMHNetwork class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MHNetwork.h"
#include "GameState.h"
#include "mmsystem.h"

#include "MainGame.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL bReceiving = FALSE;		// 八荤侩... 皋技瘤甫 罐绰 吝俊 m_pCurState狼 函拳啊 老绢唱搁 救等促.
GLOBALTON(CMHNetwork)
CMHNetwork* gNetwork = NULL;	//????????????????????
HMODULE g_hBaseNetworkHandle = NULL;

CMHNetwork::CMHNetwork()
{
	m_ConnectionIndex = NULL;
	m_CheckSum = 0;

	gNetwork = this;

	m_bDisconnecting = FALSE;


	
#ifdef _DEBUG
	memset(m_MsgIn,0,sizeof(DWORD)*MP_MAX);
	memset(m_MsgOut,0,sizeof(DWORD)*MP_MAX);
#endif

}

CMHNetwork::~CMHNetwork()
{
	Release();
}

void CMHNetwork::Release()
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

#define WM_SOCKEVENTMSG WM_USER+155

BOOL CMHNetwork::Init(HWND hWnd)
{
	m_hMainWnd = hWnd;
	
	HRESULT hr;
	
	//积己
	g_hBaseNetworkHandle = LoadLibrary("BaseNetwork.dll");

	DllGetClassObject_BaseNetwork	pNetFunc;
	pNetFunc = (DllGetClassObject_BaseNetwork)GetProcAddress(g_hBaseNetworkHandle,"DllGetClassObject");
	hr = pNetFunc(CLSID_SC_BASENETWORK_DLL, IID_SC_BASENETWORK_DLL, (void**)&m_pBaseNetwork);
		
	if (hr != S_OK)
	{
		MessageBox(NULL,"BASENETWORK.DLL is Not Exist.","Error",MB_OK);
		return FALSE;
	}
	
	//Network 檬扁拳 
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

BOOL CMHNetwork::ConnectToServer(char* ip,WORD port)
{	
	char hostname[256];
	strcpy(hostname,ip);
	// 检测输入的是域名还是ip地址，如是域名就去获取其对应的ip
	if (isalpha(ip[0]))
	{   
		
		// 获得主机名.
		//int res = gethostname(hostname, sizeof(hostname));
		//if (res != 0) {
		//	//printf("Error: %u\n", WSAGetLastError());
		//	return FALSE;
		//}
		//printf("hostname=%s\n", hostname);
		
		// 根据主机名获取主机信息. 
		hostent* pHostent = gethostbyname(hostname);
		if (pHostent==NULL) {
			//printf("Error: %u\n", WSAGetLastError());
			return FALSE;
		}
		
		// 解析返回的hostent信息.
		hostent& he = *pHostent;
		//printf("name=%s\naliases=%s\naddrtype=%d\nlength=%d\n",
		//	he.h_name, he.h_aliases, he.h_addrtype, he.h_length);

		sockaddr_in sa;
		for (int nAdapter=0; he.h_addr_list[nAdapter]; nAdapter++) {
			memcpy ( &sa.sin_addr.s_addr, he.h_addr_list[nAdapter],he.h_length);
			// 输出机器的IP地址.
			sprintf(hostname,"%s", inet_ntoa(sa.sin_addr)); // 显示地址串
		}

		
	}

	m_ConnectionIndex = m_pBaseNetwork->ConnectToServer(hostname,port);
	if(!m_ConnectionIndex)
		return FALSE;
	
	return TRUE;
}

void CMHNetwork::Disconnect()
{
	if(m_ConnectionIndex)
	{
		m_pBaseNetwork->CompulsiveDisconnect(m_ConnectionIndex);
		m_ConnectionIndex = 0;	
		m_bDisconnecting = TRUE;
	}
}

void CMHNetwork::SetCurState(CGameState* pState)
{
	m_pCurState = pState;
	
	//////////////////////////////////////////////////////////////////////////
	// for test
	m_RecvCount = 0;
	m_RecvedDataSize = 0;
}

void CMHNetwork::Send(MSGROOT* pMsg,int MsgLen)
{
	ASSERT(pMsg->Category < MP_MAX);
#ifdef _DEBUG
	AddMsgIn(pMsg->Category,MsgLen);
#endif
#ifdef _CRYPTCHECK_ 
	int ssize = sizeof( MSGROOT );
	if( !m_crypt.Encrypt( ( char * )pMsg + ssize, MsgLen - ssize ) )
	{
		ASSERTMSG(0,"Encrypt Error");
		return;
	}
	pMsg->Code = m_crypt.GetEnCRCConvertChar();
#endif	

	pMsg->CheckSum = m_CheckSum++;
	if(m_ConnectionIndex)
	m_pBaseNetwork->Send(m_ConnectionIndex,(char*)pMsg,MsgLen);
}



//////////////////////////////////////////////////////////////////////////

void OnCon(DWORD dwConIndex)
{
//	int a=0;
}

void OnDisCon(DWORD dwConIndex)
{
	if(gNetwork->m_bDisconnecting == FALSE)
	{
		gNetwork->m_pCurState->OnDisconnect();
	}
	else
	{
		gNetwork->m_bDisconnecting = FALSE;
	}
}

//#define PRTOCOL_LOG
void OnRecv(DWORD dwConIndex,char* pMsg,DWORD msglen)
{	
	bReceiving = TRUE;
	ASSERT(gNetwork->m_pCurState);

	MSGROOT* pTemp = (MSGROOT*)pMsg;

/*	
	//////////////////////////////////////////////////////////////////////////
	// test
	if(pTemp->Category != MP_MOVE && 1)
	{
		FILE* fp = fopen("recv.txt","wb");
		if(fp)
		{
			fwrite(pMsg,1,msglen,fp);
			char temp[64];
			sprintf(temp,"####%s",GetCurTimeToString());
			fwrite(temp,1,strlen(temp),fp);
			fclose(fp);
		}
	}
	//////////////////////////////////////////////////////////////////////////
*/	

#ifdef _CRYPTCHECK_ 
	int headerSize = sizeof( MSGROOT );
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

#endif	
#ifdef _DEBUG
	gNetwork->AddMsgOut(pTemp->Category,msglen);
#endif
	if(CGameState::CommonNetworkParser(pTemp->Category,pTemp->Protocol,pMsg) == FALSE)
		gNetwork->m_pCurState->NetworkMsgParse(pTemp->Category,pTemp->Protocol,pMsg);
	
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