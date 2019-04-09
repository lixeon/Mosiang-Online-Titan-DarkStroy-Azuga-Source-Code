// QuestUpdater.h: interface for the CQuestUpdater class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTUPDATER_H__4AA13D4F_B271_43EE_B11B_163762DD8EE3__INCLUDED_)
#define AFX_QUESTUPDATER_H__4AA13D4F_B271_43EE_B11B_163762DD8EE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\[CC]Quest\QuestDefines.h"


class CPlayer;
class CQuestString;

class CQuestUpdater  
{
public:
	CQuestUpdater();
	virtual ~CQuestUpdater();

	void	JournalItemAdd(JOURNALINFO* pItem);	

	void	CompleteQuestDelete(CQuestString* pQuestString);
	void	ProcessQuestAdd(CQuestString* pQuestString);
	void	ProcessQuestDelete(CQuestString* pQuestString);

	void	QuestItemAdd(QUEST_ITEM_INFO* pQuestItemInfo , DWORD Count);
	void	QuestItemDelete(DWORD ItemIdx);
	void	QuestItemUpdate(DWORD type, DWORD ItemIdx, DWORD Count);
	DWORD	GetQuestItemCount(DWORD ItemIdx);

	//
	DWORD	GetSelectedQuestID();
	void	CloseMsgBox();
	
	void	DeleteQuest(DWORD QuestIdx);
	
	void	UpdateSubQuestData();
};

#endif // !defined(AFX_QUESTUPDATER_H__4AA13D4F_B271_43EE_B11B_163762DD8EE3__INCLUDED_)
