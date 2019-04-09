// GameIn.cpp: implementation of the CGameIn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameIn.h"
#include "DummyObjMng.h"
#include "StateMng.h"
#include "MoveMng.h"

#include "Console.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern DWORD g_DisconInMap;
extern DWORD g_DisconIntoMap;
extern DWORD g_MapChangeAck;

CGameIn::CGameIn()
{
	m_bGameIn = FALSE;
}

CGameIn::~CGameIn()
{
}

BOOL CGameIn::Init( int id, CStateMng* pMgr, CDummyNetwork* pNet, CDummyObjMng* pDOMgr )
{
	CGameState::Init( id, pMgr, pNet, pDOMgr );

	return TRUE;
}

void CGameIn::Open( void* pInitParam )
{
	m_pNetwork->SetCurState( this );

	m_bGameIn = FALSE;

//	g_Console.Log( eLogDisplay, 4, "Dummy%d State : CGameIn", m_nID );

//	MSGBASE msg;
//	msg.Category = MP_USERCONN;
//	msg.Protocol = MP_USERCONN_GAMEIN_SYN;
//	m_pNetwork->Send( &msg, sizeof(msg) );	

	MSG_DWORD msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_GAMEIN_SYN;
	msg.dwData = m_pStateMng->m_dwChannel;
	m_pNetwork->Send( &msg, sizeof(msg) );
}

void CGameIn::Close()
{
}

void CGameIn::Exit()
{
}

void CGameIn::Process()
{
	if( m_bGameIn )
	{
		m_pDummyObjMgr->Process();
	}
}

void CGameIn::NetworkMsgParse( BYTE Category, BYTE Protocol, void* pMsg )
{
	CGameState::CommonNetworkMsgParse( Category, Protocol, pMsg );

	switch( Category )
	{
	case MP_USERCONN:
		{
			switch( Protocol )
			{
			case MP_USERCONN_GAMEIN_ACK:
				{	
					SEND_HERO_TOTALINFO* pmsg = (SEND_HERO_TOTALINFO*)pMsg;
					m_pDummyObjMgr->SetHeroTotalInfo( *pmsg );

					g_Console.Log( eLogDisplay, 4, "---------------------------- Dummy%d State : GameIn", m_nID );
				}
				return;
			case MP_USERCONN_GAMEIN_NACK:
				{
					m_pNetwork->Disconnect();

					m_pStateMng->ChangeState( eGAMESTATE_END, "MP_USERCONN_GAMEIN_NACK", 0 );
				}
				return;

			case MP_USERCONN_CHANGEMAP_ACK:
				{
					MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
					m_pDummyObjMgr->SetMapNum( pmsg->dwData );
		
					m_pStateMng->ChangeState( eGAMESTATE_MAPCHANGE, NULL, 0 );

					m_bGameIn = FALSE;

					g_Console.Log( eLogDisplay, 4, "Dummy%d State : Map Change to %d", m_nID, pmsg->dwData );
					
					++g_MapChangeAck;
				}
				return;

			case MP_USERCONN_CHANGEMAP_NACK:
				{
					m_pNetwork->Disconnect();

					m_bGameIn = FALSE;

					m_pStateMng->ChangeState( eGAMESTATE_END, "MP_USERCONN_CHANGEMAP_NACK", 0 );
				}
				return;

			case MP_USERCONN_MONSTER_ADD:
				{
//					SEND_MONSTER_TOTALINFO* pmsg = (SEND_MONSTER_TOTALINFO*)pMsg;
//					if( pmsg->BaseObjectInfo.dwObjectID >= 100000 )
//						MOVEMNG->AddMonster( *pmsg );
				}
				return;

			case MP_USERCONN_OBJECT_REMOVE:
				{
//					MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
//					MOVEMNG->RemoveMonster( pmsg->dwData );
				}
				return;
				
			case MP_USERCONN_CHARACTER_DIE:
				{
					MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
					m_pDummyObjMgr->SetDummyObjStateDie( pmsg->dwData2 );
				}
				return;

			case MP_USERCONN_GRIDINIT:
				m_bGameIn = TRUE;
				return;					
			}
		}
		return;

	case MP_MOVE:
		{
			MOVEMNG->NetworkMsgParse( Protocol, pMsg );
		}
		return;
		
/*	case MP_CHAR_REVIVE:
		{
			switch(Protocol)
			{				
			case MP_CHAR_REVIVE_PRESENTSPOT_ACK: 
				{
					MOVE_POS* pmsg = (MOVE_POS*)pMsg;
					VECTOR3 pos;
					pmsg->cpos.Decompress(&pos);
					m_pDummyObjMgr->SetDummyObjStateRevive( pmsg->dwMoverID, pos );
				}
				break;
			case MP_CHAR_REVIVE_LOGINSPOT_ACK:
				{
//					MOVE_POS* pmsg = (MOVE_POS*)pMsg;
//					VECTOR3 pos;
//					CObject* pObject = OBJECTMGR->GetObject(pmsg->dwMoverID);
//					pmsg->cpos.Decompress(&pos);
//					OBJECTACTIONMGR->Revive(pObject,&pos);
				}
				break;
			}
		}
		return;
		*/
	}
}

void CGameIn::OnDisconnect()
{
	if( m_bGameIn )
		++g_DisconInMap;
	else
		++g_DisconIntoMap;
	m_pStateMng->ChangeState( eGAMESTATE_END, "CGameIn-OnDisconnect()", 0 );
}
