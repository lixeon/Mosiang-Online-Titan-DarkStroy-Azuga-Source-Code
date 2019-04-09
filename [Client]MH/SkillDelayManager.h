// SkillDelayManager.h: interface for the CSkillDelayManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLDELAYMANAGER_H__70522BD6_27D5_47F5_BAD8_6A81882AAC5B__INCLUDED_)
#define AFX_SKILLDELAYMANAGER_H__70522BD6_27D5_47F5_BAD8_6A81882AAC5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct PRIMERESKILL
{
	DWORD dwSkillIndex;
	DWORD dwDelay;
};


#define SKILLDELAYMGR USINGTON(CSkillDelayManager)

class CSkillDelayManager  
{
protected:

	CYHHashTable<PRIMERESKILL> m_htPrimereSkill;
	
//	DWORD	m_dwCharacterID;
//	DWORD	m_dwSkillStartTime;
//	DWORD	m_dwSkillDelay;

public:
	CSkillDelayManager();
	virtual ~CSkillDelayManager();

	void LoadSkillUseInfo();

//	BOOL CanExcuteSkill( DWORD dwSkillIndex );
	void AddSkillDelay( DWORD dwSkillIndex );
	void ContinueSkillDelay( DWORD dwDelayTime, DWORD dwRemainDelay );

};

EXTERNGLOBALTON(CSkillDelayManager);

#endif // !defined(AFX_SKILLDELAYMANAGER_H__70522BD6_27D5_47F5_BAD8_6A81882AAC5B__INCLUDED_)
