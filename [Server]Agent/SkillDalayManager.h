// SkillDalayManager.h: interface for the CSkillDalayManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLDALAYMANAGER_H__4458B32A_934C_4817_A100_0F8D7286ACE7__INCLUDED_)
#define AFX_SKILLDALAYMANAGER_H__4458B32A_934C_4817_A100_0F8D7286ACE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SKILLDELAYMGR	USINGTON(CSkillDelayManager)

struct PRIMERESKILL
{
	DWORD dwSkillIndex;
	DWORD dwDelay;
};

struct SKILLUSE
{
	DWORD dwCharacterID;
	DWORD dwSkillIndex;
	DWORD dwDelay;
	DWORD dwStartTime;
};



class CSkillDelayManager  
{
protected:
	CMemoryPoolTempl<SKILLUSE>*	m_mpSkillUse;
	CYHHashTable<SKILLUSE>		m_htSkillUse;

	CYHHashTable<PRIMERESKILL> m_htPrimereSkill;

public:
	CSkillDelayManager();
	virtual ~CSkillDelayManager();

	void LoadSkillUseInfo();

	BOOL IsPrimereSkill( DWORD dwSkillIndex );

	BOOL AddSkillUse( DWORD dwCharacterID, DWORD dwSkillIndex, BOOL bForce = FALSE );
//	void RemoveSkillUse( DWORD dwCharacterID );

	void SendMsgToAgentServer( DWORD dwCharacterID, DWORD dwSkillIndex );
	void SendSkillDelayMsgToClient( DWORD dwCharacterID );
};


EXTERNGLOBALTON(CSkillDelayManager);

#endif // !defined(AFX_SKILLDALAYMANAGER_H__4458B32A_934C_4817_A100_0F8D7286ACE7__INCLUDED_)
