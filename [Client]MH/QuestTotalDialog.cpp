// QuestTotalDialog.cpp: implementation of the CQuestTotalDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestTotalDialog.h"

#include "QuestDialog.h"
#include "JournalDialog.h"
#include "WantedDialog.h"
#include "QuestManager.h"
#include "GameIn.h"
#include "MainBarDialog.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestTotalDialog::CQuestTotalDialog()
{
	m_type = WT_QUESTTOTALDIALOG;
	m_pWantedDlg = NULL;
	m_pQuestDlg = NULL;
	m_pJournalDlg = NULL;
}

CQuestTotalDialog::~CQuestTotalDialog()
{

}
void CQuestTotalDialog::SetActive( BOOL val )
{
	cTabDialog::SetActive( val );
	
//--- main bar icon
	CMainBarDialog* pDlg = GAMEIN->GetMainInterfaceDialog();
	if( pDlg )
	{
		pDlg->SetPushBarIcon( OPT_QUESTDLGICON, m_bActive );
		if( val == TRUE )
		{
			pDlg->SetAlram( OPT_QUESTDLGICON, FALSE );
		}
	}
}

void CQuestTotalDialog::Add(cWindow * window)
{	
	if(window->GetType() == WT_WANTEDDIALOG)
		m_pWantedDlg = (CWantedDialog*)window;
	else if(window->GetType() == WT_JOURNALDIALOG)
		m_pJournalDlg = (CJournalDialog*)window;
	else if(window->GetType() == WT_QUESTDIALOG)
		m_pQuestDlg = (CQuestDialog*)window;
		

	if(window->GetType() == WT_PUSHUPBUTTON)
	{
		AddTabBtn(curIdx1++, (cPushupButton * )window);
		return;
	}
	else if(window->GetType() == WT_QUESTDIALOG ||
		window->GetType() == WT_WANTEDDIALOG ||
		window->GetType() == WT_JOURNALDIALOG)
	{
		AddTabSheet(curIdx2++, window);
		return;
	}
	cDialog::Add(window);
}

void CQuestTotalDialog::JournalItemAdd(JOURNALINFO* pItem)
{
	if(pItem->Kind == eJournal_Quest)
	{	
		int Key = 0;
		COMBINEKEY(pItem->Param, 0, Key);
		m_pQuestDlg->CompleteQuestDelete(QUESTMGR->GetQuestString(Key));
	}
	m_pJournalDlg->JournalItemAdd(pItem);
}

void CQuestTotalDialog::CompleteQuestDelete(CQuestString* pQuestString)
{
	m_pQuestDlg->CompleteQuestDelete(pQuestString);
}

void CQuestTotalDialog::ProcessQuestAdd(CQuestString* pQuestString)
{
	m_pQuestDlg->ProcessQuestAdd(pQuestString);
}

void CQuestTotalDialog::ProcessQuestDelete(CQuestString* pQuestString)
{
	m_pQuestDlg->ProcessQuestDelete(pQuestString);
}

void CQuestTotalDialog::QuestItemAdd(QUEST_ITEM_INFO* pQuestItemInfo, DWORD Count)
{
	m_pQuestDlg->QuestItemAdd(pQuestItemInfo, Count);
}

void CQuestTotalDialog::QuestItemDelete(DWORD ItemIdx)
{
	m_pQuestDlg->QuestItemDelete(ItemIdx);
}

DWORD CQuestTotalDialog::QuestItemUpdate(DWORD type, DWORD ItemIdx, DWORD data)
{
	return m_pQuestDlg->QuestItemUpdate(type, ItemIdx, data);
}

DWORD CQuestTotalDialog::GetSelectedQuestID()
{
	return m_pQuestDlg->GetSelectedQuestID();
}

void CQuestTotalDialog::CloseMsgBox()
{
	m_pQuestDlg->CloseMsgBox();
}

void CQuestTotalDialog::GiveupQuestDelete(DWORD QuestIdx)
{
	m_pQuestDlg->GiveupQuestDelete(QuestIdx);
}



void CQuestTotalDialog::QuestListView()
{
	m_pQuestDlg->QuestListReset();
}

void CQuestTotalDialog::JournalView()
{
	m_pJournalDlg->JournalListReset();
}

void CQuestTotalDialog::UpdateSubQuestData()
{
	m_pQuestDlg->QuestListReset();	
}