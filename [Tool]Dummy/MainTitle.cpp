// MainTitle.cpp: implementation of the CMainTitle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainTitle.h"
#include "StateMng.h"

#include "Console.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern DWORD g_ConnectDist;
extern DWORD g_DisconnectDist;
extern DWORD g_TryToConnectAgent;
extern DWORD g_ConnectAgent;
extern DWORD g_DisconnectAgent;
extern DWORD g_ReceiveLoginAck;

char g_Version[256];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMainTitle::CMainTitle()
{
	m_bConnectingToAgent = FALSE;
	m_bConnectingToDist = FALSE;

	m_dwDistAuthKey = 0;

	m_bLoginAck = FALSE;
	m_bOverLogin = FALSE;

	m_bEnterAgent = FALSE;

	m_bDisconnectToDist = FALSE;

	m_ProcLevel = 0;

	m_bLoginTime = FALSE;

	strcpy( g_Version, "VER_Dummy");
}
	
CMainTitle::~CMainTitle()
{
}

BOOL CMainTitle::Init( int id, CStateMng* pMgr, CDummyNetwork* pNet, CDummyObjMng* pDOMgr )
{	
	CGameState::Init( id, pMgr, pNet, pDOMgr );
	
	return TRUE;
}

void CMainTitle::Open( void* pInitParam )
{
	m_pNetwork->SetCurState( this );
		
	m_bConnectingToDist = TRUE;
	m_bConnectingToAgent = FALSE;
	m_bDisconnectToDist = FALSE;
	m_bOverLogin = FALSE;
	m_bLoginTime = FALSE;

//	IPPORT* Dis = RESMNG->GetDistribute();		
//	m_pNetwork->ConnectToServer( Dis->Ip, Dis->Port );
}

void CMainTitle::Close()
{
//	m_bConnectingToDist = FALSE;
//	m_bConnectingToAgent = FALSE;	
}

void CMainTitle::Exit()
{
//	m_bConnectingToDist = FALSE;
//	m_bConnectingToAgent = FALSE;	
}

void CMainTitle::Process()
{
	if( m_bOverLogin )
	{
		++m_dwLogin;
		if( m_dwLogin > 10 )
		{
//			SendIDPass();
			m_bOverLogin = FALSE;

			m_pStateMng->ChangeState( eGAMESTATE_END, "ReLogin", 0 );
		}
	}

//	if( m_bEnterAgent )//&& m_pStateMng->m_bEnterAgent )
//	{
//		SendIDPass();

//		m_bEnterAgent = FALSE;
//	}
	
	if( m_bLoginTime == FALSE )
	{		
		--m_pStateMng->m_dwLoginTime;
		if( m_pStateMng->m_dwLoginTime <=  0 )
		{
			IPPORT* Dis = RESMNG->GetDistribute();		
			m_pNetwork->ConnectToServer( Dis->Ip, Dis->Port );

			m_bLoginTime = TRUE;
		}
	}
}

void CMainTitle::NetworkMsgParse( BYTE Category, BYTE Protocol, void* pMsg )
{
	CGameState::CommonNetworkMsgParse( Category, Protocol, pMsg );
	
	switch( Category ) 
	{
	case MP_USERCONN:
		{
			switch( Protocol ) 
			{
			case MP_USERCONN_DIST_CONNECTSUCCESS:
				{
					MSGBASE* pmsg = (MSGBASE*)pMsg;
					m_dwDistAuthKey = pmsg->dwObjectID;
					m_ProcLevel = PL_ReceiveDCS;

					m_bEnterAgent = TRUE;

					SendIDPass();

					g_ConnectDist++;				
					
				}
				return;

			case MP_USERCONN_LOGIN_ACK:
				{					
					m_ProcLevel = PL_ReceiveLoginAck;

					m_bDisconnectToDist = TRUE;

					MSG_LOGIN_ACK* pmsg = (MSG_LOGIN_ACK*)pMsg;
					strcpy( m_pStateMng->m_AgentIpPort.Ip, pmsg->agentip );
					m_pStateMng->m_AgentIpPort.Port = pmsg->agentport;
					m_pStateMng->m_dwUserIDx = pmsg->userIdx;
					if( pmsg->userIdx == 0 )
						g_Console.Log( eLogDisplay, 4, "!!!!!!!!!!MP_USERCONN_LOGIN_ACK!!!!!!!!!!!!유저 IDX==0");

					m_bLoginAck = TRUE;
					MSGBASE msg;
					msg.Category = MP_USERCONN;
					msg.Protocol = MP_USERCONN_REQUEST_DISTOUT;
					msg.dwObjectID = pmsg->userIdx;
					m_pNetwork->Send(&msg, sizeof(msg));

					g_ReceiveLoginAck++;

				}
				return;				
			case MP_USERCONN_LOGIN_NACK:
				{
					m_bDisconnectToDist = FALSE;					
//					m_pNetwork->Disconnect();

					MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
					LoginError( pmsg->dwData );		

				}
				return;
				
			case MP_USERCONN_AGENT_CONNECTSUCCESS:
				{
					if( m_bLoginAck )
					{
						MSGCRYPTDATA* pmsg = (MSGCRYPTDATA*)pMsg;
//						m_pNetwork->SetCheckSum( pmsg->CheckSum );
						
						MSG_DWORD2 msg;
						msg.Category = MP_USERCONN;
						msg.Protocol = MP_USERCONN_CHARACTERLIST_SYN;
						msg.dwData1 = m_pStateMng->m_dwUserIDx;
						if( msg.dwData1 == 0 )
							g_Console.Log( eLogDisplay, 4, "Dummy%d State : !!!!!!!MP_USERCONN_AGENT_CONNECTSUCCESS!!!!유저 IDX==0", m_nID );
						else
						{
							msg.dwData2 = m_dwDistAuthKey;
							m_pNetwork->Send( &msg,sizeof(msg) );
						}				
						g_ConnectAgent++;						
					}	
				}
				return;

			case MP_USERCONN_CHARACTERLIST_ACK:
				{
					m_pStateMng->ChangeState( eGAMESTATE_CHARSELECT, (void*)pMsg, sizeof(SEND_CHARSELECT_INFO) );
				}
				return;
			case MP_USERCONN_CHARACTERLIST_NACK:
				{
					m_bDisconnectToDist = FALSE;					
					m_pNetwork->Disconnect();

					m_pStateMng->ChangeState( eGAMESTATE_END, "MainTitle-MP_USERCONN_CHARACTERLIST_NACK", 0 );

					g_DisconnectAgent++;
				}
				return;

			case MP_USERCONN_CHECKSUMERROR:
				{					
					m_bDisconnectToDist = FALSE;					
					m_pNetwork->Disconnect();

					m_pStateMng->ChangeState( eGAMESTATE_END, "MP_USERCONN_CHECKSUMERROR", 0 );

					g_DisconnectAgent++;
				}
				break;
			}
		}
		return;
	}
}

void CMainTitle::OnDisconnect()
{
	if( m_bDisconnectToDist )
	{
		m_bConnectingToAgent = TRUE;
		m_bDisconnectToDist = FALSE;
		
		m_pNetwork->ConnectToServer( m_pStateMng->m_AgentIpPort.Ip, m_pStateMng->m_AgentIpPort.Port );
		g_TryToConnectAgent++;
	}
	else if( m_bConnectingToAgent )
	{
		m_pStateMng->ChangeState( eGAMESTATE_END, "CMainTitle-OnDisconnect  Agent()", 0 );
		g_DisconnectAgent++;
	}
	else
	{

		m_pStateMng->ChangeState( eGAMESTATE_END, "CMainTitle-OnDisconnect    Dist()", 0 );
		g_DisconnectDist++;

		g_Console.Log( eLogDisplay, 4, "Dummy%d(UserIDx:%d) ProcLevel : %d", m_nID, m_pStateMng->m_dwUserIDx, m_ProcLevel );	
	}
	
}

void CMainTitle::OnConnectSuccessed()
{
	if( m_bConnectingToAgent )
	{
//		g_Console.Log( eLogDisplay, 4, "Dummy%d(UserIDx:%d) State : Connect Success to Agent Server!!", m_nID, m_pStateMng->m_dwUserIDx );
//		g_ConnectAgent++;
	}
	else if( m_bConnectingToDist )
	{
//		g_Console.Log( eLogDisplay, 4, "Dummy%d(UserIDx:%d) State : Connect Success to Distribute Server!!", m_nID, m_pStateMng->m_dwUserIDx );
//		g_ConnectDist++;
		m_ProcLevel = PL_ConDist;
	}
	else
		ASSERT(0);
}
	
void CMainTitle::OnConnectFailed()
{
	if( m_bConnectingToAgent )
	{
		g_Console.Log( eLogDisplay, 4, "Dummy%d(UserIDx:%d) State : Connect Failed to Agent Server!!", m_nID, m_pStateMng->m_dwUserIDx );
		m_pStateMng->ChangeState( eGAMESTATE_END, "Connect Failed to Agent Server!!", 0 );
//		g_DisconnectAgent++;
	}
	else if(m_bConnectingToDist)
	{
		g_Console.Log( eLogDisplay, 4, "Dummy%d(UserIDx:%d) State : Connect Failed to Distribute Server!!", m_nID, m_pStateMng->m_dwUserIDx );
		m_pStateMng->ChangeState( eGAMESTATE_END, "Connect Failed to Distribute Server!!", 0 );
	}
	else
		ASSERT(0);	
}

void CMainTitle::LoginError( int code )
{
	char temp[256];
	switch( code )
	{
	case LOGIN_ERROR_INVALID_VERSION:	sprintf( temp, "LOGIN_ERROR_INVALID_VERSION" );			break;
	case LOGIN_ERROR_NOAGENTSERVER:		sprintf( temp, "LOGIN_ERROR_NOAGENTSERVER" );			break;
	case LOGIN_ERROR_NODISTRIBUTESERVER:	sprintf( temp, "LOGIN_ERROR_NODISTRIBUTESERVER" );	break;
	case LOGIN_ERROR_INVALIDUSERLEVEL:	sprintf( temp, "LOGIN_ERROR_INVALIDUSERLEVEL" );		break;
	case LOGIN_ERROR_WRONGIDPW:			sprintf( temp, "LOGIN_ERROR_WRONGIDPW" );				break;
	case LOGIN_ERROR_INVALIDDISTAUTHKEY:	sprintf( temp, "LOGIN_ERROR_INVALIDDISTAUTHKEY" );	break;
	case LOGIN_ERROR_DISTSERVERISBUSY:	sprintf( temp, "LOGIN_ERROR_DISTSERVERISBUSY" );		break;
	case LOGIN_ERROR_AGENTSERVERISBUSY:	sprintf( temp, "LOGIN_ERROR_AGENTSERVERISBUSY" );		break;
	case LOGIN_ERROR_AGENTMAXCONNECTION:	sprintf( temp, "LOGIN_ERROR_AGENTMAXCONNECTION" );	break;
	case LOGIN_ERROR_OVERLAPPEDLOGIN:
		{
			g_Console.Log( eLogDisplay, 4, "Dummy%d(UserIDx:%d) State : LOGIN_ERROR_OVERLAPPEDLOGIN", m_nID, m_pStateMng->m_dwUserIDx );

//			sprintf( temp, "LOGIN_ERROR_OVERLAPPEDLOGIN" );
			MSGBASE msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_FORCE_DISCONNECT_OVERLAPLOGIN;
			m_pNetwork->Send(&msg,sizeof(msg));

			m_bOverLogin = TRUE;
			m_dwLogin = 0;
		}
		return;
//		break;
	}

	m_pStateMng->ChangeState( eGAMESTATE_END, temp, 0 );
}

void CMainTitle::SendIDPass()
{
	// id & pass send
	char tempID[MAX_NAME_LENGTH];
	char tempPW[MAX_NAME_LENGTH];
	
	MSG_LOGIN_SYN msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_LOGIN_SYN;
	sprintf( tempID, "%d", m_nID );
	sprintf( tempPW, "ejal!" );		// 더미!
	strcpy( msg.id, tempID );
	strcpy( msg.pw, tempPW );
	strcpy( msg.Version, g_Version );
	msg.AuthKey = m_dwDistAuthKey;
	m_pNetwork->Send( &msg, sizeof(msg) );
	//
}
