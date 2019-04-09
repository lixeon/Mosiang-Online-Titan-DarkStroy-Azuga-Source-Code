// EventMapMgr.cpp: implementation of the CEventMapMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EventMapMgr.h"
#include "ChannelSystem.h"
#include "Player.h"
#include "Network.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CEventMap::CEventMap()
{
	m_PlayerTable.Initialize( 1000 );
	m_nState = eEventState_None;
	m_nChannelNum = 0;
	m_dwTime = 0;

	m_dwWinnerIdx = m_dwWinnerPartyIdx = 0;
}

CEventMap::~CEventMap()
{
	Release();
}

void CEventMap::Release()
{
//	CObject* pData = NULL;
//	m_PlayerTable.SetPositionHead();
//	while( pData = m_PlayerTable.GetData() )
//		m_PlayerTable.Remove( pData->GetID() );
	m_PlayerTable.RemoveAll();
}

void CEventMap::AddMember( CObject* pObject )
{
	if( m_PlayerTable.GetData( pObject->GetID() ) )
		return;

	m_PlayerTable.Add( pObject, pObject->GetID() );
}

void CEventMap::RemoveMember( CObject* pObject )
{	
	CObject* pData = m_PlayerTable.GetData( pObject->GetID() );
	if( pData )
		m_PlayerTable.Remove( pData->GetID() );	
}

BOOL CEventMap::IsEnemy( CObject* pOper, CObject* pTarget )
{
	if( m_nState != eEventState_Start )
		return FALSE;

	CObject* pData = m_PlayerTable.GetData( pOper->GetID() );
	if( !pData )
		return FALSE;
	pData = m_PlayerTable.GetData( pTarget->GetID() );
	if( !pData )
		return FALSE;

	if( pOper->GetObjectKind() != eObjectKind_Player )
		return FALSE;
	if( pTarget->GetObjectKind() != eObjectKind_Player )
		return FALSE;

	CPlayer* p1 = (CPlayer*)pOper;
	CPlayer* p2 = (CPlayer*)pTarget;

	if( p2->GetUserLevel() == eUSERLEVEL_GM )
		return FALSE;
	if( p1->GetPartyIdx() == 0 )
	{
		if( p1->GetID() != p2->GetID() )
			return TRUE;
	}
	if( p1->GetPartyIdx() != p2->GetPartyIdx() )
		return TRUE;

	return FALSE;
}

void CEventMap::Process()
{
	DWORD dwRemainTime = gCurTime - m_dwTime;

	switch( m_nState )
	{
	case eEventState_None:
		{
		}
		break;
	case eEventState_Ready:
		{
			if( dwRemainTime > 10000 )
				Start();
		}
		break;
	case eEventState_Start:
		{
			if( dwRemainTime > 600000 )
				JudgeFromOverTime();
			else
				Judge();
		}
		break;
	case eEventState_End:
		{
			End();
		}
		break;
	case eEventState_UserOut:
		{
			if( dwRemainTime > 10000 )
				UserOut();
		}
		break;
	}
}

void CEventMap::Judge()
{
	DWORD dwTeam = 0;
	m_dwWinnerIdx = m_dwWinnerPartyIdx = 0;

	CPlayer* pPlayer = NULL;
	m_PlayerTable.SetPositionHead();
	while( pPlayer = (CPlayer*)m_PlayerTable.GetData() )
	{
		if( pPlayer->GetUserLevel() == eUSERLEVEL_GM )
			continue;

		if( pPlayer->GetState() != eObjectState_Die )
		{
			if( pPlayer->GetPartyIdx() != 0 )
			{
				if( m_dwWinnerPartyIdx != pPlayer->GetPartyIdx() )
				{
					++dwTeam;
					m_dwWinnerPartyIdx = pPlayer->GetPartyIdx();
				}
				if( dwTeam > 1 )
					break;
			}
			else
			{
				++dwTeam;
				m_dwWinnerIdx = pPlayer->GetID();
				if( dwTeam > 1 )
					break;
			}
		}
	}

	if( dwTeam == 1 || dwTeam == 0 )
		m_nState = eEventState_End;
}

void CEventMap::JudgeFromOverTime()
{
	m_dwWinnerIdx = m_dwWinnerPartyIdx = 0;

	m_nState = eEventState_End;
}

void CEventMap::Ready()
{
	MSGBASE Msg;
	Msg.Category = MP_USERCONN;
	Msg.Protocol = MP_USERCONN_EVENT_READY;
	
	CPlayer* pPlayer = NULL;
	m_PlayerTable.SetPositionHead();
	while( pPlayer = (CPlayer*)m_PlayerTable.GetData() )
	{
		pPlayer->SendMsg( &Msg, sizeof(Msg) );
	}	

	m_nState = eEventState_Ready;

	m_dwTime = gCurTime;
}

void CEventMap::Start()
{
	MSGBASE Msg;
	Msg.Category = MP_USERCONN;
	Msg.Protocol = MP_USERCONN_EVENT_START;
	
	CPlayer* pPlayer = NULL;
	m_PlayerTable.SetPositionHead();
	while( pPlayer = (CPlayer*)m_PlayerTable.GetData() )
	{
		pPlayer->SendMsg( &Msg, sizeof(Msg) );
	}	

	m_nState = eEventState_Start;

	m_dwTime = gCurTime;
}

void CEventMap::End()
{
	MSG_DWORD3 Msg;
	Msg.Category = MP_USERCONN;
	Msg.Protocol = MP_USERCONN_EVENT_END;

	MSG_NAME_WORD GMMsg;
	memset( &GMMsg, 0, sizeof(MSG_NAME_WORD) );
	GMMsg.Category = MP_CHEAT;
	GMMsg.Protocol = MP_CHEAT_EVENTMAP_RESULT;
	GMMsg.wData = m_nChannelNum;

	if( m_dwWinnerIdx != 0 )
	{
		Msg.dwData1 = 0;	// win kind
		CPlayer* pPlayer = NULL;
		m_PlayerTable.SetPositionHead();
		while( pPlayer = (CPlayer*)m_PlayerTable.GetData() )
		{
			if( pPlayer->GetUserLevel() == eUSERLEVEL_GM )
				continue;
			if( pPlayer->GetID() == m_dwWinnerIdx )
			{
				Msg.dwData2 = m_dwWinnerIdx;
				Msg.dwData3 = 1;				// win

				if( strcmp( GMMsg.Name, "" ) == 0 )
					SafeStrCpy( GMMsg.Name, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1 );				
			}
			else
			{
				Msg.dwData2 = m_dwWinnerIdx;
				Msg.dwData3 = 0;				// lose
			}

			pPlayer->SendMsg( &Msg, sizeof(Msg) );
		}

		GMMsg.dwObjectID = 0;		
	}	
	else if( m_dwWinnerPartyIdx != 0 )
	{
		Msg.dwData1 = 1;	// win kind
		CPlayer* pPlayer = NULL;
		m_PlayerTable.SetPositionHead();
		while( pPlayer = (CPlayer*)m_PlayerTable.GetData() )
		{
			if( pPlayer->GetUserLevel() == eUSERLEVEL_GM )
				continue;
			if( pPlayer->GetPartyIdx() == m_dwWinnerPartyIdx )
			{
				Msg.dwData2 = m_dwWinnerPartyIdx;
				Msg.dwData3 = 1;				// win

				if( strcmp( GMMsg.Name, "" ) == 0 )
					SafeStrCpy( GMMsg.Name, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1 );
			}
			else
			{
				Msg.dwData2 = m_dwWinnerPartyIdx;
				Msg.dwData3 = 0;				// lose
			}

			pPlayer->SendMsg( &Msg, sizeof(Msg) );
		}

		GMMsg.dwObjectID = 0;
	}
	else
	{
		Msg.dwData1 = 2;	// win kind
		CPlayer* pPlayer = NULL;
		m_PlayerTable.SetPositionHead();
		while( pPlayer = (CPlayer*)m_PlayerTable.GetData() )
		{
			if( pPlayer->GetUserLevel() == eUSERLEVEL_GM )
				continue;
			Msg.dwData2 = 0;
			Msg.dwData3 = 2;				// draw
			pPlayer->SendMsg( &Msg, sizeof(Msg) );
		}

		GMMsg.dwObjectID = 2;
	}

	g_Network.Send2AgentServer( (char*)&GMMsg, sizeof(GMMsg) );

	m_nState = eEventState_UserOut;

	m_dwTime = gCurTime;
}

void CEventMap::UserOut()
{
	MSGBASE Msg;
	Msg.Category = MP_USERCONN;
	Msg.Protocol = MP_USERCONN_MAP_OUT_TO_EVENTBEFOREMAP;
	
	CPlayer* pPlayer = NULL;
	m_PlayerTable.SetPositionHead();
	while( pPlayer = (CPlayer*)m_PlayerTable.GetData() )
	{
		if( pPlayer->GetUserLevel() == eUSERLEVEL_GM )
			continue;
		pPlayer->SendMsg( &Msg, sizeof(Msg) );

		g_pServerSystem->RemovePlayer( pPlayer->GetID() );
	}	

	m_nState = eEventState_None;

	m_dwTime = gCurTime;	
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CEventMapMgr::CEventMapMgr()
{
	m_EventMapTable.Initialize( 100 );
	m_dwMapNum = 0;
}

CEventMapMgr::~CEventMapMgr()
{
	Remove();
}

void CEventMapMgr::Init( DWORD dwMapNum, int nChannelNum )
{
	if( nChannelNum < 1 )	return;

	m_dwMapNum = dwMapNum;

	CHANNELSYSTEM->CreateChannel( 0, nChannelNum );

	CEventMap* pEventMap = NULL;
	for( int i = 0; i < nChannelNum; ++i )
	{
		pEventMap = new CEventMap;
		pEventMap->Init( i + 1 );
		m_EventMapTable.Add( pEventMap, i );
	}
}

void CEventMapMgr::Remove()
{
	CEventMap* pEventMap = NULL;
	m_EventMapTable.SetPositionHead();
	while( pEventMap = m_EventMapTable.GetData() )
		delete pEventMap;
	m_EventMapTable.RemoveAll();
}

BOOL CEventMapMgr::IsEventMap()
{
	if( m_dwMapNum == EVENTMAPNUM )	return TRUE;
	else							return FALSE;
}

BOOL CEventMapMgr::IsEnemy( CObject* pOper, CObject* pTarget, DWORD dwChannel )
{
	CEventMap* pEventMap = m_EventMapTable.GetData( dwChannel - 1 );
	if( !pEventMap )	return FALSE;

	return pEventMap->IsEnemy( pOper, pTarget );
}

void CEventMapMgr::AddMember( CObject* pObject, int nTeam, int nChannel )
{
	CEventMap* pEventMap = m_EventMapTable.GetData( nChannel - 1 );
	if( pEventMap )
		pEventMap->AddMember( pObject );
}

void CEventMapMgr::RemoveMember( CObject* pObject, int nTeam, int nChannel )
{
	CEventMap* pEventMap = m_EventMapTable.GetData( nChannel - 1 );
	if( pEventMap )
		pEventMap->RemoveMember( pObject );
}

void CEventMapMgr::Process()
{
	CEventMap* pEventMap = NULL;
	m_EventMapTable.SetPositionHead();
	while( pEventMap = m_EventMapTable.GetData() )
		pEventMap->Process();
}

void CEventMapMgr::StartEvent( DWORD dwChannel )
{
	CEventMap* pEventMap = m_EventMapTable.GetData( dwChannel - 1 );
	if( !pEventMap )	return;
	
	pEventMap->Start();	
}

void CEventMapMgr::ReadyEvent( DWORD dwChannel )
{
	CEventMap* pEventMap = m_EventMapTable.GetData( dwChannel - 1 );
	if( !pEventMap )	return;
	
	pEventMap->Ready();	
}

void CEventMapMgr::NetworkMsgParse( BYTE protocol, MSGBASE* pmsg )
{
	
}
