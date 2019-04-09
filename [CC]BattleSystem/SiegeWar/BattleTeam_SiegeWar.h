// BattleTeam_SiegeWar.h: interface for the CBattleTeam_SiegeWar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BATTLETEAM_SIEGEWAR_H__416891AD_E4AC_46F9_89C6_2F5AC5ADFB63__INCLUDED_)
#define AFX_BATTLETEAM_SIEGEWAR_H__416891AD_E4AC_46F9_89C6_2F5AC5ADFB63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BattleTeam.h"


struct BATTLE_INFO_SIEGEWAR : public BATTLE_INFO_BASE
{
	DWORD		DefenceCount;
	DWORD		AttackCount;
	DWORD		GuildList[SIEGEWAR_MAXGUILDCOUNT_PERTEAM*2];

	BATTLE_INFO_SIEGEWAR()
	{
		DefenceCount = 0;
		AttackCount = 0;
		memset( GuildList, 0, sizeof(DWORD)*SIEGEWAR_MAXGUILDCOUNT_PERTEAM*2 );
	}
	int GetSize()
	{
		return (sizeof(BATTLE_INFO_SIEGEWAR) - (sizeof(DWORD)*(SIEGEWAR_MAXGUILDCOUNT_PERTEAM*2-(DefenceCount+AttackCount))) );
	}
};


class CBattleTeam_SiegeWar : public CBattleTeam
{
	DWORD		m_GuildCount;
	DWORD		m_GuildList[SIEGEWAR_MAXGUILDCOUNT_PERTEAM];

public:
	CBattleTeam_SiegeWar();
	virtual ~CBattleTeam_SiegeWar();

	void Init( DWORD Count, DWORD* GuildList );
	virtual BOOL IsAddableTeamMember( CObject* pObject );

	void SetTeamInfo( DWORD Count, DWORD* pGuildList);
	void SendBattlInfoToPlayer( BATTLE_INFO_SIEGEWAR* pInfo, DWORD Team );
	void ReturnToMap();
};

#endif // !defined(AFX_BATTLETEAM_SIEGEWAR_H__416891AD_E4AC_46F9_89C6_2F5AC5ADFB63__INCLUDED_)
