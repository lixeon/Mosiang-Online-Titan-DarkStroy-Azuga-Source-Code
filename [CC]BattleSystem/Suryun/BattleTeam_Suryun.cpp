// BattleTeam_Suryun.cpp: implementation of the CBattleTeam_Suryun class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleTeam_Suryun.h"

#include "Player.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBattleTeam_Suryun_Player::CBattleTeam_Suryun_Player()
{

}

CBattleTeam_Suryun_Player::~CBattleTeam_Suryun_Player()
{

}

BOOL CBattleTeam_Suryun_Player::IsAddableTeamMember( CObject* pObject )
{
	if(pObject->GetObjectKind() == eObjectKind_Player)
	{
		ASSERT(GetMemberNum() == 0);
		return TRUE;
	}

	return FALSE;
}

CBattleTeam_Suryun_Monster::CBattleTeam_Suryun_Monster()
{

}

CBattleTeam_Suryun_Monster::~CBattleTeam_Suryun_Monster()
{

}

BOOL CBattleTeam_Suryun_Monster::IsAddableTeamMember( CObject* pObject )
{
	BYTE objectKind = pObject->GetObjectKind();
	if( (objectKind == eObjectKind_Monster) || (objectKind == eObjectKind_TitanMonster) )
		return TRUE;

	return FALSE;
}
