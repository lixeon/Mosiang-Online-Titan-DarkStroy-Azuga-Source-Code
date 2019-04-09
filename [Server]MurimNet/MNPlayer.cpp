// MNPlayer.cpp: implementation of the MNPlayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MNPlayer.h"
#include "NetWork.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMNPlayer::CMNPlayer()
{
	m_Location.nKind	= eLOCATION_NULL;
	m_Location.dwIndex	= 0;

}

CMNPlayer::~CMNPlayer()
{

}

BOOL CMNPlayer::Init( DWORD dwPlayerID, DWORD dwAgentNum, DWORD UniqueIDinAgent, DWORD dwBackMapNum  )
{	
	memset( &m_BaseObjectInfo, 0, sizeof(BASEOBJECT_INFO) );
	memset( &m_CharacterInfo, 0, sizeof(CHARACTER_TOTALINFO) );
	memset( &m_HeroInfo, 0, sizeof(HERO_TOTALINFO) );
	
	m_AgentNum					= dwAgentNum;
	m_UnigueIDinAgent			= UniqueIDinAgent;
	m_BaseObjectInfo.dwObjectID = dwPlayerID;
	m_dwBackMapNum				= dwBackMapNum;
	
	m_bCaptain = FALSE;

	return TRUE;
}

void CMNPlayer::InitPlayerInfo(BASEOBJECT_INFO* pObjinfo, CHARACTER_TOTALINFO* pTotalinfo, HERO_TOTALINFO* pHeroinfo)
{
	memcpy(&m_BaseObjectInfo,pObjinfo,sizeof(BASEOBJECT_INFO));
	memcpy(&m_CharacterInfo,pTotalinfo,sizeof(CHARACTER_TOTALINFO));
	memcpy(&m_HeroInfo,pHeroinfo,sizeof(HERO_TOTALINFO));

}

void CMNPlayer::SendMsg( MSGBASE* pMsg, int MsgLen )
{
	pMsg->dwObjectID = GetID();
	g_Network.Send2Server( m_AgentNum, (char*)pMsg, MsgLen );
}