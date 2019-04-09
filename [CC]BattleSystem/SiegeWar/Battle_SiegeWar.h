// Battle_SiegeWar.h: interface for the CBattle_SiegeWar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BATTLE_SIEGEWAR_H__B56EC219_0938_4490_962A_484336CA2BF3__INCLUDED_)
#define AFX_BATTLE_SIEGEWAR_H__B56EC219_0938_4490_962A_484336CA2BF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Battle.h"
#include "BattleTeam_SiegeWar.h"

#define MAX_SWTEAM_USERCOUNT		1000



class CBattle_SiegeWar : public CBattle
{
	BATTLE_INFO_SIEGEWAR		m_BInfo;
	CBattleTeam_SiegeWar		m_Observer;
	
	VECTOR3				m_Team1Pos;
	VECTOR3				m_Team2Pos;

public:
	CBattle_SiegeWar();
	virtual ~CBattle_SiegeWar();

	virtual void Initialize(BATTLE_INFO_BASE* pCreateInfo, CBattleTeam* pTeam1, CBattleTeam* pTeam2);
	virtual void OnCreate(BATTLE_INFO_BASE* pCreateInfo, CBattleTeam* pTeam1, CBattleTeam* pTeam2);
	virtual void OnDestroy();
	virtual void OnTeamMemberAdd(int Team,DWORD MemberID,char* Name);
	virtual BOOL OnTeamMemberDie(int Team,DWORD VictimMemberID,DWORD KillerID);

#ifdef _MAPSERVER_
	virtual void GetBattleInfo(char* pInfo,WORD* size);
	virtual BOOL AddObjectToBattle(CObject* pObject);
	virtual BOOL DeleteObjectFromBattle(CObject* pObject);
	virtual DWORD GetTeamMemberNum(int i);
	BOOL AddObserverToBattle(CObject* pObject);
	void ReleaseAllObject();
	void SetBattleInfo( BATTLE_INFO_SIEGEWAR* pInfo );
	void BattleTeamChange();
	void SendBattleInfoToPlayer();
	void BattleTeamPositionChange();
	void ReturnToMapAllPlayer();
#else
	void CreateWaitPlace();
	void DestroyWaitPlace();
	virtual void Render();
	BOOL IsObserver( CObject* pObject );
	void SetBattleInfo( SEND_SW_BTGUILDLIST* pInfo );
	BOOL IsDefenceGuild( DWORD GuildIdx );
	void SetSiegeWarName( CObject* pObject, DWORD Team, DWORD GuildIdx );
#endif
	
	virtual void OnFightStart();
	virtual BOOL IsEnemy(CObject* pOperator,CObject* pTarget);
	virtual BOOL IsFriend(CObject* pOperator,CObject* pTarget);
	virtual DWORD GetBattleTeamID( CObject* pObject );	
	
	void StartBattle();
	void EndBattle();
};

#endif // !defined(AFX_BATTLE_SIEGEWAR_H__B56EC219_0938_4490_962A_484336CA2BF3__INCLUDED_)
