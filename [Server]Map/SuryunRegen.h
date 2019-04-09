// SuryunRegen.h: interface for the CSuryunRegen class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SURYUNREGEN_H__6D4DBD39_2F64_4E4F_B710_F5C9B6BF1BA1__INCLUDED_)
#define AFX_SURYUNREGEN_H__6D4DBD39_2F64_4E4F_B710_F5C9B6BF1BA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define MAX_REGENMONSTER_KIND 5
//#include "cLinkedList.h"

class CSuryunRegenInfo;
class CSuryunRegenGroup;
class CPlayer;

class CSuryunRegen  
{
	CPlayer* m_pPlayer;
	DWORD m_StartTime;
	WORD m_MonsterKindInfo[MAX_REGENMONSTER_KIND];
	
	CSuryunRegenInfo* m_RegenInfo;
 	CSuryunRegenGroup * m_RegenGroupArray;
public:
	CSuryunRegen();
	~CSuryunRegen();
	void Init(CSuryunRegenInfo* pRegenInfo, WORD* MonsterKindInfo);
	void StartProcess();
	void Process();
	void Release();

	void DecreaseMonster(int GroupID);

	void SetPlayer(CPlayer* pPlayer)	{	m_pPlayer = pPlayer;	}
};

#endif // !defined(AFX_SURYUNREGEN_H__6D4DBD39_2F64_4E4F_B710_F5C9B6BF1BA1__INCLUDED_)
