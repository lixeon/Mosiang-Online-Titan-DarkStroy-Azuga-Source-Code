// BattleTeam.cpp: implementation of the CBattleTeam class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleTeam.h"

#include "Player.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBattleTeam::CBattleTeam()
{
	m_TeamMemberTable.Initialize(32);
}

CBattleTeam::~CBattleTeam()
{
	Release();
}

void CBattleTeam::Release()
{
	DoRelease();
	
	CObject* pObject;
	m_TeamMemberTable.SetPositionHead();
	while(pObject = m_TeamMemberTable.GetData())
	{
		DoDeleteTeamMember(pObject);
	}
	m_TeamMemberTable.RemoveAll();
}

BOOL CBattleTeam::AddTeamMember(CObject* pObject)
{
	if(m_TeamMemberTable.GetData(pObject->GetID()) != NULL)
		return FALSE;

	m_TeamMemberTable.Add(pObject,pObject->GetID());
	DoAddTeamMember(pObject);
	return TRUE;
}

BOOL CBattleTeam::DeleteTeamMember(CObject* pObject)
{
	if(m_TeamMemberTable.GetData(pObject->GetID()) == NULL)
		return FALSE;

	m_TeamMemberTable.Remove(pObject->GetID());
	DoDeleteTeamMember(pObject);

	return TRUE;
}

DWORD CBattleTeam::GetMemberNum()
{
	return m_TeamMemberTable.GetDataNum();
}

YHTPOSITION CBattleTeam::GetPositionHead()
{
	return m_TeamMemberTable.GetPositionHead();
}

CObject* CBattleTeam::GetNextTeamMember(YHTPOSITION* ppList)
{
	return m_TeamMemberTable.GetDataPos(ppList);
}

void CBattleTeam::SendTeamMsg(MSGBASE* pmsg,int length)
{
#ifdef _MAPSERVER_
	CObject* pObject;
	m_TeamMemberTable.SetPositionHead();
	while(pObject = m_TeamMemberTable.GetData())
	{
		if(pObject->GetObjectKind() == eObjectKind_Player)
			((CPlayer*)pObject)->SendMsg(pmsg,length);
	}
#endif
}

BOOL CBattleTeam::IsTeamMember(CObject* pObject)
{
	if( m_TeamMemberTable.GetData( pObject->GetID() ) )
		return TRUE;

	return FALSE;
}