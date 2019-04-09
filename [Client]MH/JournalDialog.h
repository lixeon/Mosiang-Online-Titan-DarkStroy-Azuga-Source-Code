// JournalDialog.h: interface for the CJournalDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JOURNALDIALOG_H__676849EE_96BF_4124_8790_CBC965F84E81__INCLUDED_)
#define AFX_JOURNALDIALOG_H__676849EE_96BF_4124_8790_CBC965F84E81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INTERFACE\cDialog.h"
#include "..\[CC]Quest\QuestDefines.h"


class cButton;
class cPushupButton;
class cCheckBox;
class cListDialog;
class CQuestString;


struct JournalItem
{
	DWORD			type;
	DWORD			JournalDBIndex;
	int				ViewIndex;						// 화면에 보이는 Index ( 0 ~ 4 까지,   -1 = 안보인다. )
	CQuestString*	pQuestString;
	CQuestString*	pSubQuestString;
	char			Name[MAX_NAME_LENGTH+1];
	char			RegDate[11];					// 날자
	DWORD			Param_1;
	DWORD			Param_2;
	DWORD			bSaved;

	JournalItem()
	{
		JournalDBIndex = 0;
		ViewIndex = -1;
		pQuestString = NULL;
		pSubQuestString = NULL;
		memset(Name, 0, MAX_NAME_LENGTH+1);
		memset(RegDate, 0, 11);
		Param_1 = 0;
		Param_2 = 0;
		bSaved = 0;
	}
};


class CJournalDialog : public cDialog  
{
	cButton*			m_pMoveBtn[2];
	cButton*			m_pJournalBtn[2];
	cPushupButton*		m_pPageBtn[MAX_JOURNAL_PAGEBTN];
	cCheckBox*			m_pCheckBox[MAX_CHECKBOX_PERPAGE];
	cListDialog*		m_pTextList;

	int				m_BasePage;
	int				m_MaxPage;
	int				m_CurPage;
	cPtrList		m_JournalList;
	cPtrList		m_JournalSavedList;
	BOOL			m_bSavedJournal;
	BOOL			m_bCheckItem[MAX_CHECKBOX_PERPAGE];


public:
	CJournalDialog();
	virtual ~CJournalDialog();

	void	Linking();
	void	JournalReset();

	void	JournalItemAdd(JOURNALINFO* pJournalInfo);	
	void	JournalListReset();	

	void	SetBasePage(BOOL bNext);
	void	SetPage(int Index);

	BOOL	IsSavedJournal()				{	return m_bSavedJournal;		}
	void	SetItemCheck(int Index);
	void	SelectedJournalSave();
	void	SelectedJournalDelete();
	void	ViewJournalListToggle();

protected:
	void	JournalItemAdd(DWORD Index, CQuestString* pQuestString, CQuestString* pSubQString, char* Time, DWORD SubQuestIndex, BOOL bCompleted, BOOL bSaved);
	void	JournalItemAdd(DWORD Index, char* sName, DWORD Param, char* Time, BOOL bSaved);
	void	JournalItemAdd(DWORD Index, int Level, char* Time, BOOL bSaved);
	void	AddList(JournalItem* pItem, BOOL bSaved);

	void	JournalListQuestAdd(JournalItem* pItem);
	void	JournalListWantedAdd(JournalItem* pItem);
	void	JournalListLevelupAdd(JournalItem* pItem);

	void	SendNetMsg(DWORD Index, DWORD type);
};

#endif // !defined(AFX_JOURNALDIALOG_H__676849EE_96BF_4124_8790_CBC965F84E81__INCLUDED_)

