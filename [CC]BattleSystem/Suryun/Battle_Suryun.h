// Battle_Suryun.h: interface for the CBattle_Suryun class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BATTLE_SURYUN_H__1AD15919_719C_4DAE_BE91_9A819F9D5C1C__INCLUDED_)
#define AFX_BATTLE_SURYUN_H__1AD15919_719C_4DAE_BE91_9A819F9D5C1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Battle.h"
#include "BattleTeam_Suryun.h"

class CPlayer;
class CSuryunRegen;
struct SURYUNINFO;

#ifdef _MHCLIENT_	
#include "../ImageNumber.h"
#define MSGBOXTIME 4000
#endif

struct BATTLE_INFO_SURYUN : public BATTLE_INFO_BASE
{
	DWORD ReturnBattleID;
	DWORD ReturnGridID;
	VECTOR3 ReturnPosition;
	WORD MonsterKindInfo[5];
	WORD MugongIdx;
	BYTE CurrentSung;
};

class CBattle_Suryun : public CBattle  
{
	BOOL m_bDestroyed;
	BOOL m_bResultEffect;
	BATTLE_INFO_SURYUN m_BattleSuryunInfo;
	BOOL m_bResultFirst;
	CPlayer* m_pPlayer;
	CSuryunRegen* m_pRegen;
	SURYUNINFO* m_pSuryunInfo;
	
	CBattleTeam_Suryun_Player m_PlayerTeam;
	CBattleTeam_Suryun_Monster m_MonsterTeam;

	DWORD m_dwCreateTime;
	int m_KilledCount;
#ifdef _MHCLIENT_	
	BOOL m_bMsgBox;
	DWORD m_ResultStartTime;
	BOOL m_bSetResultTime;
#endif
	void RemoveAllMonster();
public:
	CBattle_Suryun();
	virtual ~CBattle_Suryun();

	void Initialize(BATTLE_INFO_BASE* pCreateInfo, CBattleTeam* pTeam1, CBattleTeam* pTeam2);
	
	//////////////////////////////////////////////////////////////////////////
	// { start virtual func 반드시 오버라이딩 해야함

	// Battle 정보 관련
#ifdef _MAPSERVER_
	virtual void GetBattleInfo(char* pInfo,WORD* size);
#endif

	// 적,아군 구별
	virtual BOOL IsEnemy(CObject* pOperator,CObject* pTarget);
	virtual BOOL IsFriend(CObject* pOperator,CObject* pTarget);
	
	// event func
	virtual void OnCreate(BATTLE_INFO_BASE* pCreateInfo, CBattleTeam* pTeam1, CBattleTeam* pTeam2);
	virtual void OnFightStart();
	virtual void OnDestroy();
	virtual void OnTeamMemberAdd(int Team,DWORD MemberID,char* Name);
	virtual BOOL OnTeamMemberDelete(int Team,DWORD MemberID,char* Name);
	virtual BOOL OnTeamMemberDie(int Team,DWORD VictimMemberID,DWORD KillerID);
	virtual void OnTeamMemberRevive(int Team,CObject* pReviver);
	virtual void OnTeamObjectCreate(int Team,CBattleObject* pBattleObject);
	virtual void OnTeamObjectDestroy(int Team,CBattleObject* pBattleObject);
	virtual void OnMonsterDistribute(CMonster* pMonster,CObject* pKiller);	

	virtual void OnTick();
	
	// 승패 판정
	virtual BOOL Judge();		// Judge에서 Victory나 Draw 함수를 호출해주어야 한다.
	virtual void Victory(int WinnerTeamNum,int LoserTeamNum);
	virtual void Draw();

	
#ifdef _MHCLIENT_
	// Render		(클라이언트만 사용함)
	virtual void Render();
#endif

	// } end virtual func
	//////////////////////////////////////////////////////////////////////////
	
};

#endif // !defined(AFX_BATTLE_SURYUN_H__1AD15919_719C_4DAE_BE91_9A819F9D5C1C__INCLUDED_)
