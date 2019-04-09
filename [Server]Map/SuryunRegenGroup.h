// SuryunRegenGroup.h: interface for the CSuryunRegenGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SURYUNREGENGROUP_H__3F4380CE_CABA_4884_8CF5_654167AD8613__INCLUDED_)
#define AFX_SURYUNREGENGROUP_H__3F4380CE_CABA_4884_8CF5_654167AD8613__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSuryunRegenGroup  
{
	BOOL m_bExecute;
	int m_AliveMonsterCount;
	
public:
	CSuryunRegenGroup();
	~CSuryunRegenGroup();
	void SetExecute(int MonNum) { m_bExecute = TRUE; m_AliveMonsterCount = MonNum;}
	BOOL IsExecuted() { return m_bExecute;	}
	int GetAliveMonsterNum() { return m_AliveMonsterCount; }
	void SetAliveMonsterNum(int num) { m_AliveMonsterCount = num;	}
	int DecreaseAliveMonsterNum();
};

#endif // !defined(AFX_SURYUNREGENGROUP_H__3F4380CE_CABA_4884_8CF5_654167AD8613__INCLUDED_)
