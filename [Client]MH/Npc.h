// Npc.h: interface for the CNpc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPC_H__0E71DC08_2E3F_440C_8EB6_7A9D6D0F8CF3__INCLUDED_)
#define AFX_NPC_H__0E71DC08_2E3F_440C_8EB6_7A9D6D0F8CF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Object.h"
#include "GameResourceStruct.h"


#define MAX_NPCQUEST_LIST 50




class CNpc : public CObject  
{
	NPC_TOTALINFO m_NpcInfo;
	int		m_QuestList[MAX_NPCQUEST_LIST];
	int		m_QuestCount;
	
	NPC_LIST * m_pSInfo;

	void InitNpc(NPC_TOTALINFO* pTotalInfo);
public:
	CNpc();
	virtual ~CNpc();
	virtual void Die(CObject* pAttacker,BOOL bFatalDamage,BOOL bCritical, BOOL bDecisive);
	NPC_LIST * GetSInfo() { return m_pSInfo; }

	void Process();
	WORD GetNpcJob(){return m_NpcInfo.NpcJob;}
	friend class CObjectManager;

	WORD GetNpcKind()		{	return m_NpcInfo.NpcKind;	}
	WORD GetNpcUniqueIdx()		{	return m_NpcInfo.NpcUniqueIdx;	}
	void AddQuestIndex(int Idx);
	int  GetQuestIdx(int Idx)		{	return m_QuestList[Idx];	}
	int  GetQuestCount()			{	return m_QuestCount;	}

	void OnClicked();
	
	virtual void SetMotionInState(BYTE State);
};

#endif // !defined(AFX_NPC_H__0E71DC08_2E3F_440C_8EB6_7A9D6D0F8CF3__INCLUDED_)
