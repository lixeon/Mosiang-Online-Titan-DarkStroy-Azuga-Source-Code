// CharMake.cpp: implementation of the CCharMake class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CharMake.h"
#include "StateMng.h"

#include "Console.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern	BOOL	g_bDirectAgent;

extern DWORD g_ConnectDist;
extern DWORD g_DisconnectDist;
extern DWORD g_TryToConnectAgent;
extern DWORD g_ConnectAgent;
extern DWORD g_DisconnectAgent;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCharMake::CCharMake()
{
}

CCharMake::~CCharMake()
{
}

BOOL CCharMake::Init( int id, CStateMng* pMgr, CDummyNetwork* pNet, CDummyObjMng* pDOMgr )
{
	CGameState::Init( id, pMgr, pNet, pDOMgr );

	return TRUE;
}

void CCharMake::Open( void* pInitParam )
{
	m_pNetwork->SetCurState( this );

	g_Console.Log( eLogDisplay, 4, "Dummy%d State : CCharMake", m_nID );

	// character »ý¼º
	char tempName[MAX_NAME_LENGTH];
	CHARACTERMAKEINFO msg;
	memset( &msg, 0, sizeof(CHARACTERMAKEINFO) );
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHARACTER_MAKE_SYN;
	msg.SexType = rand()%2;
	msg.BodyType = 0;
	msg.HairType = 0;
	msg.FaceType = 0;
	msg.StartArea = 18;
	msg.WearedItemIdx[eWearedItem_Dress] = 0;
	msg.WearedItemIdx[eWearedItem_Shoes] = 0;
	msg.StandingArrayNum = -1;
	sprintf( tempName, "Dum%d", m_nID );
	strcpy( msg.Name, tempName );
	m_pNetwork->Send( &msg, sizeof(msg) );
	//	
}

void CCharMake::Close()
{
}

void CCharMake::Exit()
{
}

void CCharMake::Process()
{
}

void CCharMake::NetworkMsgParse( BYTE Category, BYTE Protocol, void* pMsg )
{
	CGameState::CommonNetworkMsgParse( Category, Protocol, pMsg );

	switch( Category ) 
	{
	case MP_USERCONN:
		{
			switch( Protocol )
			{
			case MP_USERCONN_CHARACTER_MAKE_ACK:
				{
					MSGBASE msg;
					msg.Category = MP_USERCONN;
					msg.Protocol = MP_USERCONN_DIRECTCHARACTERLIST_SYN;
					m_pNetwork->Send( &msg, sizeof(msg) );
				}
				return;
			case MP_USERCONN_CHARACTER_MAKE_NACK:
				{
					g_DisconnectAgent++;
					m_pStateMng->ChangeState( eGAMESTATE_END, "CharMake - MP_USERCONN_CHARACTER_MAKE_NACK", 0 );
				}
				return;				

			case MP_USERCONN_CHARACTERLIST_ACK:
				{	
					m_pStateMng->ChangeState( eGAMESTATE_CHARSELECT, (void*)pMsg, sizeof(SEND_CHARSELECT_INFO) );
				}
				return;
			}
		}
		return;
	}
}

void CCharMake::OnDisconnect()
{
	m_pStateMng->ChangeState( eGAMESTATE_END, "CCharMake-OnDisconnect()", 0 );	
	g_DisconnectAgent++;
}
