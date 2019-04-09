// Battle_MunpaField.h: interface for the CBattle_MunpaField class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BATTLE_MUNPAFIELD_H__CCD48E03_0A1B_4A21_B5A0_EC801D163024__INCLUDED_)
#define AFX_BATTLE_MUNPAFIELD_H__CCD48E03_0A1B_4A21_B5A0_EC801D163024__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Battle.h"

class CBattleTeam_MunpaField;

class CBattle_MunpaField : public CBattle  
{
	BOOL JudgeOneTeamWinsOtherTeam(int TheTeam,int OtherTeam);

	CBattleTeam_MunpaField* GetTeam(int Team)
	{
		ASSERT(Team < eBattleTeam_Max);
		return (CBattleTeam_MunpaField*)m_TeamArray[Team];
	}
	int GetOtherTeamNum(int Team)
	{
		ASSERT(Team < eBattleTeam_Max);
		return Team == eBattleTeam1 ? eBattleTeam2 : eBattleTeam1;
	}


#ifdef _MAPSERVER_
	MAPTYPE m_RequestMapNum;		// ¹®ÆÄÀü »ý¼ºÀ» ¿äÃ»ÇÑ ¸Ê¼­¹öÀÇ Æ÷Æ®
#endif

public:
	CBattle_MunpaField();
	virtual ~CBattle_MunpaField();


	//////////////////////////////////////////////////////////////////////////
	// { start virtual func ¹Ýµå½Ã ¿À¹ö¶óÀÌµù ÇØ¾ßÇÔ

	// Battle Á¤º¸ °ü·Ã
#ifdef _MAPSERVER_
	virtual void GetBattleInfo(char* pInfo,WORD* size);
#endif

	// Àû,¾Æ±º ±¸º°
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

	virtual void OnTick();

	// ½ÂÆÐ ÆÇÁ¤
	virtual BOOL Judge();
	virtual void Victory(int WinnerTeamNum,int LoserTeamNum);
	virtual void Draw();

	
#ifdef _MHCLIENT_
	// Render		(Å¬¶óÀÌ¾ðÆ®¸¸ »ç¿ëÇÔ)
	virtual void Render();
#endif

	// } end virtual func
	//////////////////////////////////////////////////////////////////////////
};

#endif // !defined(AFX_BATTLE_MUNPAFIELD_H__CCD48E03_0A1B_4A21_B5A0_EC801D163024__INCLUDED_)
