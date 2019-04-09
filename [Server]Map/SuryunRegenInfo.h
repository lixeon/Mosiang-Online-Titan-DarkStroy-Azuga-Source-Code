// SuryunRegenInfo.h: interface for the CSuryunRegenInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SURYUNREGENINFO_H__ADC003E7_25C4_4316_9828_C958CE1A7404__INCLUDED_)
#define AFX_SURYUNREGENINFO_H__ADC003E7_25C4_4316_9828_C958CE1A7404__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CSuryunRegenGroupInfo;
class CSuryunRegenGroup;
class CSuryunRegenInfo  
{
	int m_MaxCount;
	CSuryunRegenGroupInfo* m_RegenGroupInfoArray;
	
public:
	CSuryunRegenInfo();
	~CSuryunRegenInfo();
	void Init(char* filename);
	void TimeRegen(CSuryunRegenGroup* pRegenGroup, int GroupID, DWORD elpsTime, CPlayer* pPlayer, WORD* MonsterKindInfo);
	
	void RatioRegen(CSuryunRegenGroup* pRegenGroupArray, int GroupID, int CurMonsterNum, CPlayer* pPlayer, WORD* MonsterKindInfo);
	int GetMaxCount() { return m_MaxCount;	}
	void Load(char* filename);
	int GetMonCountinGroup(int GroupID);
};

#endif // !defined(AFX_SURYUNREGENINFO_H__ADC003E7_25C4_4316_9828_C958CE1A7404__INCLUDED_)
