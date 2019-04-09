// QuestTotalDialog.h: interface for the CQuestTotalDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTTOTALDIALOG_H__55C4DC04_EA23_4325_88BD_4E476F4A1D10__INCLUDED_)
#define AFX_QUESTTOTALDIALOG_H__55C4DC04_EA23_4325_88BD_4E476F4A1D10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cTabDialog.h"
#include "..\[CC]Quest\QuestDefines.h"

class CWantedDialog;
class CQuestDialog;
class CJournalDialog;
class CQuestString;

class CQuestTotalDialog  : public cTabDialog
{
	CWantedDialog*	m_pWantedDlg;
	CQuestDialog*	m_pQuestDlg;
	CJournalDialog*	m_pJournalDlg;
	
public:
	CQuestTotalDialog();
	virtual ~CQuestTotalDialog();
	virtual void Add(cWindow * window);
	virtual void SetActive( BOOL val );
	
	void	JournalItemAdd(JOURNALINFO* pItem);
	void	CompleteQuestDelete(CQuestString* pQuestString);
	void	ProcessQuestAdd(CQuestString* pQuestString);
	void	ProcessQuestDelete(CQuestString* pQuestString);

	void	QuestItemAdd(QUEST_ITEM_INFO* pQuestItemInfo, DWORD Count);
	void	QuestItemDelete(DWORD ItemIdx);
	DWORD	QuestItemUpdate(DWORD type, DWORD ItemIdx, DWORD data);
	
	DWORD	GetSelectedQuestID();
	void	CloseMsgBox();
	void	GiveupQuestDelete(DWORD QuestIdx);
	
	//test
	void	QuestListView();
	void	JournalView();
	
	void	UpdateSubQuestData();
};

#endif // !defined(AFX_QUESTTOTALDIALOG_H__55C4DC04_EA23_4325_88BD_4E476F4A1D10__INCLUDED_)
