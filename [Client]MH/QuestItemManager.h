// QuestItemManager.h: interface for the CQuestItemManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTITEMMANAGER_H__8C0E130D_7E31_450F_BDD1_EAC967EE5918__INCLUDED_)
#define AFX_QUESTITEMMANAGER_H__8C0E130D_7E31_450F_BDD1_EAC967EE5918__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "..\[CC]Quest\QuestDefines.h"



class CQuestItemManager  
{
	CYHHashTable<QUEST_ITEM_INFO>		m_QuestItemList;	

public:
	CQuestItemManager();
	virtual ~CQuestItemManager();

	void	Release();
	void	QuestItemInfoLoad();
	QUEST_ITEM_INFO*	GetQuestItem(DWORD ItemIdx);

};


#endif // !defined(AFX_QUESTITEMMANAGER_H__8C0E130D_7E31_450F_BDD1_EAC967EE5918__INCLUDED_)
