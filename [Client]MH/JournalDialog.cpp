// JournalDialog.cpp: implementation of the CJournalDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JournalDialog.h"
#include "ptrlist.h"
#include "QuestDialog.h"
#include "WindowIDEnum.h"
#include "ChatManager.h"
#include "QuestManager.h"
#include "ObjectManager.h"
#include "..\[CC]Quest\QuestString.h"
#include "./Interface/cListDialog.h"
#include "./Interface/cPushupButton.h"
#include "./Interface/cCheckBox.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJournalDialog::CJournalDialog()
{
	m_type	= WT_JOURNALDIALOG;
	m_BasePage		= 0;
	m_MaxPage		= 0;
	m_CurPage		= 0;

	m_bSavedJournal = FALSE;
	memset(m_bCheckItem, 0, sizeof(BOOL)*MAX_CHECKBOX_PERPAGE);
}

CJournalDialog::~CJournalDialog()
{
	JournalItem* pItem = NULL;

	PTRLISTSEARCHSTART(m_JournalList, JournalItem*, pItem);
		m_JournalList.Remove(pItem);
		delete pItem;
		pItem = NULL;
	PTRLISTSEARCHEND	

	PTRLISTSEARCHSTART(m_JournalSavedList, JournalItem*, pItem);
		m_JournalSavedList.Remove(pItem);
		delete pItem;
		pItem = NULL;
	PTRLISTSEARCHEND
}


void CJournalDialog::Linking()
{
	m_pMoveBtn[0] = (cButton*)GetWindowForID(QUE_JOURNALFRONTBTN);
	m_pMoveBtn[1] = (cButton*)GetWindowForID(QUE_JOURNALBACKBTN);
	m_pJournalBtn[0] = (cButton*)GetWindowForID(QUE_JOURNALSAVE);
	m_pJournalBtn[1] = (cButton*)GetWindowForID(QUE_JOURNALSAVEDLIST);
	
	for(int i=0; i<MAX_JOURNAL_PAGEBTN; ++i)
		m_pPageBtn[i] = (cPushupButton*)GetWindowForID(QUE_JOURNALPAGE1BTN+i);
	m_pPageBtn[0]->SetPush(TRUE);

	for(i=0; i<MAX_CHECKBOX_PERPAGE; i++)
		m_pCheckBox[i] = (cCheckBox*)GetWindowForID(QUE_JOURNALCHECKBOX1BTN+i);	

	m_pTextList = (cListDialog*)GetWindowForID(QUE_JOURNALLISTDLG);

	//
	SetPage(m_CurPage-m_BasePage);
}


void CJournalDialog::JournalItemAdd(JOURNALINFO* pJournalInfo)
{
	if(!pJournalInfo)			return;

	if(pJournalInfo->Kind == eJournal_Quest)
	{
		int Key = 0;
		COMBINEKEY( pJournalInfo->Param, 0, Key );
		CQuestString* pQString = QUESTMGR->GetQuestString(Key);

		/////////////////////////////////////////////////////////////////////
		// SubQuest는 기록하지 않는다.
/*		CQuestString* pSubQString = NULL;
		if(pJournalInfo->Param_2 > 0)
		{			
			COMBINEKEY( pJournalInfo->Param, pJournalInfo->Param_2, Key );
			pSubQString = QUESTMGR->GetQuestString(Key);
		}*/
//		JournalItemAdd(pJournalInfo->Index, pQString, pSubQString, pJournalInfo->RegDate, pJournalInfo->Param_2, pJournalInfo->Param_3, pJournalInfo->bSaved);
		JournalItemAdd(pJournalInfo->Index, pQString, NULL, pJournalInfo->RegDate, pJournalInfo->Param_2, pJournalInfo->Param_3, pJournalInfo->bSaved);
	}
	else if(pJournalInfo->Kind == eJournal_Wanted)
	{
		JournalItemAdd(pJournalInfo->Index, pJournalInfo->ParamName, pJournalInfo->Param, pJournalInfo->RegDate, pJournalInfo->bSaved);
	}
	else if(pJournalInfo->Kind == eJournal_Levelup)
	{
		JournalItemAdd(pJournalInfo->Index, pJournalInfo->Param, pJournalInfo->RegDate, pJournalInfo->bSaved);
	}
	
	JournalListReset();
}


void CJournalDialog::JournalItemAdd(DWORD Index, CQuestString* pQString, CQuestString* pSubQString, char* Time, DWORD SubQuestIndex, BOOL bCompleted, BOOL bSaved)
{
	if(!pQString || !Time)			return;

	JournalItem* pItem = new JournalItem;
	pItem->type = eJournal_Quest;
	pItem->JournalDBIndex = Index;
	pItem->pQuestString = pQString;
	/////////////////////////////////////////////////////////////////////
	// SubQuest는 기록하지 않는다.
//	pItem->pSubQuestString = pSubQString;
	pItem->pSubQuestString = NULL;

	pItem->Param_1 = bCompleted;
	pItem->Param_2 = SubQuestIndex;
	SafeStrCpy(pItem->RegDate, Time, 11);
	AddList(pItem, bSaved);
}


void CJournalDialog::JournalItemAdd(DWORD Index, char* sName, DWORD Param, char* Time, BOOL bSaved)
{
	if(!sName || !Time)				return;

	JournalItem* pItem = new JournalItem;
	pItem->type = eJournal_Wanted;
	pItem->JournalDBIndex = Index;
	strncpy(pItem->Name, sName, MAX_NAME_LENGTH);
	pItem->Param_1 = Param;
	SafeStrCpy(pItem->RegDate, Time, 11);
	AddList(pItem, bSaved);
}



void CJournalDialog::JournalItemAdd(DWORD Index, int Level, char* Time, BOOL bSaved)
{
	if(!Time)			return;

	JournalItem* pItem = new JournalItem;
	pItem->type = eJournal_Levelup;
	pItem->JournalDBIndex = Index;
	pItem->Param_1 = Level;
	SafeStrCpy(pItem->RegDate, Time, 11);
	AddList(pItem, bSaved);
}


void CJournalDialog::AddList(JournalItem* pItem, BOOL bSaved)
{
	m_JournalList.AddTail(pItem);

	if(bSaved)
	{
		JournalItem* pSavedItem = new JournalItem;
		memcpy(pSavedItem, pItem, sizeof(JournalItem));
		m_JournalSavedList.AddTail(pSavedItem);
	}
}

void CJournalDialog::JournalReset()
{
	m_pTextList->RemoveAll();
	m_BasePage		= 0;
	m_MaxPage		= 0;
	m_CurPage		= 0;
	m_bSavedJournal = FALSE;
	memset(m_bCheckItem, 0, sizeof(BOOL)*MAX_CHECKBOX_PERPAGE);

	JournalItem* pItem = NULL;

	PTRLISTSEARCHSTART(m_JournalList, JournalItem*, pItem);
		m_JournalList.Remove(pItem);
		delete pItem;
		pItem = NULL;
	PTRLISTSEARCHEND	

	PTRLISTSEARCHSTART(m_JournalSavedList, JournalItem*, pItem);
		m_JournalSavedList.Remove(pItem);
		delete pItem;
		pItem = NULL;
	PTRLISTSEARCHEND

	m_JournalList.RemoveAll();
	m_JournalSavedList.RemoveAll();

	m_pJournalBtn[0]->SetText(CHATMGR->GetChatMsg(631), RGB(255,255,255));
	m_pJournalBtn[1]->SetText(CHATMGR->GetChatMsg(633), RGB(255,255,255));
}


void CJournalDialog::JournalListReset()
{
	m_pTextList->RemoveAll();
	int ViewStartIdx = JOURNALVIEW_PER_PAGE*m_CurPage;
	int count = 0;
	int CheckBoxCount = 0;
	bool bAdd = false;

	// CheckBox 초기화
	for(int i=0; i<MAX_CHECKBOX_PERPAGE; i++)
	{
 		if(m_bCheckItem[i])		m_pCheckBox[i]->SetChecked(TRUE);
 		else					m_pCheckBox[i]->SetChecked(FALSE);
		m_pCheckBox[i]->SetActive(FALSE);
	}

	// MaxPage 설정
	JournalItem* pItem = NULL;
	PTRLISTPOS pos = NULL;
	int ItemCount = 0;
	if(m_bSavedJournal)
	{
		pos = m_JournalSavedList.GetHeadPosition();
		ItemCount = m_JournalSavedList.GetCount();
	}
	else
	{
		pos = m_JournalList.GetHeadPosition();
		ItemCount = m_JournalList.GetCount();
	}
	
	m_MaxPage = ItemCount/JOURNALVIEW_PER_PAGE-1;
	if(ItemCount%JOURNALVIEW_PER_PAGE)
		++m_MaxPage;

	// 화면에 보여야 할 페이지의 Item만 Add
	int viewindex = 0;
	while(pos)
	{
		if(m_bSavedJournal)			pItem = (JournalItem*)m_JournalSavedList.GetNext(pos);
		else						pItem = (JournalItem*)m_JournalList.GetNext(pos);

		if(pItem && count >= ViewStartIdx && count < ViewStartIdx+JOURNALVIEW_PER_PAGE)
		{
			// ViewIndex 설정
			pItem->ViewIndex = viewindex;
			++viewindex;

			if(pItem->type == eJournal_Quest)
				JournalListQuestAdd(pItem);
			else if(pItem->type == eJournal_Wanted)
				JournalListWantedAdd(pItem);
			else if(pItem->type == eJournal_Levelup)
				JournalListLevelupAdd(pItem);
							
			m_pCheckBox[CheckBoxCount]->SetActive(TRUE);
			++CheckBoxCount;
		}
		else
			pItem->ViewIndex = -1;

		++count;
	}

	m_pTextList->SetTopListItemIdx(0);
	SetPage(m_CurPage-m_BasePage);
}


void CJournalDialog::JournalListQuestAdd(JournalItem* pItem)
{
	if(!pItem)			return;	
	char buf[128] = { 0, };
	QString* pBuf = NULL;

	m_pTextList->AddItem(pItem->RegDate, SUBQUEST_TITLE_COLOR_SEL);

	// MainQuest Title
	pBuf = (QString*)pItem->pQuestString->GetTitle()->GetHead();
	if(pBuf)	
		m_pTextList->AddItem(pBuf->Str, QUEST_DESC_COLOR);

	/////////////////////////////////////////////////////////////////////
	// SubQuest는 기록하지 않는다.
	// SubQuest Title
/*	if(pItem->Param_2 && pItem->pSubQuestString)
	{
		pBuf = (QString*)pItem->pSubQuestString->GetTitle()->GetHead();
		if(pBuf)
			m_pTextList->AddItem(pBuf->Str, QUEST_DESC_COLOR);
		else
			m_pTextList->AddItem(" ", 0);
	}*/

	// Compeleted
	if(pItem->Param_1)
		m_pTextList->AddItem(CHATMGR->GetChatMsg(600), QUEST_DESC_HIGHLIGHT);
	else
		m_pTextList->AddItem(CHATMGR->GetChatMsg(599), QUEST_DESC_HIGHLIGHT);

	/////////////////////////////////////////////////////////////////////
	// SubQuest는 기록하지 않는다.
/*	if(!pItem->Param_2 || !pItem->pSubQuestString)
		m_pTextList->AddItem(" ", 0);*/

	m_pTextList->AddItem(" ", 0);
}

void CJournalDialog::JournalListWantedAdd(JournalItem* pItem)
{
	if(!pItem)			return;
	char buf[128] = { 0, };

	m_pTextList->AddItem(pItem->RegDate, SUBQUEST_TITLE_COLOR_SEL);

	// 척살
	sprintf(buf, "%s", CHATMGR->GetChatMsg(614));
	m_pTextList->AddItem(buf, QUEST_DESC_COLOR);

	// xxx(을)를 척살에 성공 or 실패
	switch(pItem->Param_1)
	{
	case eJournal_Wanted_Succeed:
		sprintf(buf, CHATMGR->GetChatMsg(615), pItem->Name);
		break;		
	case eJournal_Wanted_FailbyOther:
		sprintf(buf, CHATMGR->GetChatMsg(616), pItem->Name);
		break;
	case eJournal_Wanted_FailbyDelChr:
		sprintf(buf, CHATMGR->GetChatMsg(606), pItem->Name);
		break;
	case eJournal_MurderedbyChr:
		sprintf(buf, CHATMGR->GetChatMsg(636), pItem->Name);
		break;
	case eJournal_Wanted_FailbyTime:
		sprintf(buf, CHATMGR->GetChatMsg(684), pItem->Name);
		break;
	}
	m_pTextList->AddItem(buf, QUEST_DESC_HIGHLIGHT);

	m_pTextList->AddItem(" ", 0);
}

void CJournalDialog::JournalListLevelupAdd(JournalItem* pItem)
{
	if(!pItem)			return;
	char buf[128] = { 0, };

	m_pTextList->AddItem(pItem->RegDate, SUBQUEST_TITLE_COLOR_SEL);

	m_pTextList->AddItem("Level Up ", QUEST_DESC_COLOR);

	//
	sprintf(buf, CHATMGR->GetChatMsg(635), pItem->Param_1);
	m_pTextList->AddItem(buf, QUEST_DESC_HIGHLIGHT);

	m_pTextList->AddItem(" ", 0);
}



void CJournalDialog::SetBasePage(BOOL bNext)
{
	int BasePageBackup = m_BasePage;

	if(bNext)
	{
		if(m_BasePage+MAX_JOURNAL_PAGEBTN <= m_MaxPage)
			m_BasePage += MAX_JOURNAL_PAGEBTN;
	}
	else
	{
		if(m_BasePage-MAX_JOURNAL_PAGEBTN >= 0)
			m_BasePage -= MAX_JOURNAL_PAGEBTN;
	}
	
	if(BasePageBackup != m_BasePage)	SetPage(0);
}

void CJournalDialog::SetPage(int Index)
{	
	int ShowPage = m_MaxPage-m_BasePage;
	if(ShowPage > 4)	ShowPage = 4;

	char buf[16] = { 0, };
	for(int i=0; i<MAX_JOURNAL_PAGEBTN; i++)
	{
		if(i>ShowPage+1 || m_BasePage+i>m_MaxPage)		m_pPageBtn[i]->SetActive(FALSE);
		else
		{
			sprintf(buf, "%d", m_BasePage+i+1);
			m_pPageBtn[i]->SetText(buf, QUEST_DESC_COLOR);
			m_pPageBtn[i]->SetActive(TRUE);
			m_pPageBtn[i]->SetPush(FALSE);
		}
	}
	m_pPageBtn[Index]->SetPush(TRUE);

	// Back
	if(m_BasePage >= MAX_JOURNAL_PAGEBTN)
		m_pMoveBtn[0]->SetActive(TRUE);
	else
		m_pMoveBtn[0]->SetActive(FALSE);
	// Front
	if((m_BasePage+MAX_JOURNAL_PAGEBTN) <= m_MaxPage)
		m_pMoveBtn[1]->SetActive(TRUE);
	else
		m_pMoveBtn[1]->SetActive(FALSE);

 	// Save CheckBox
 	for(i=0; i<MAX_CHECKBOX_PERPAGE; i++)
 		m_bCheckItem[i] = FALSE;	

	
	// 같은 페이지번호를 눌렀을때
	if((m_BasePage+Index) == m_CurPage)			return;

	m_CurPage = m_BasePage+Index;
	JournalListReset();
}


void CJournalDialog::SendNetMsg(DWORD Index, DWORD type)
{	
	SEND_JOURNAL_DWORD Msg;
	Msg.Category = MP_JOURNAL;	

	switch(type)
	{
	case eJournal_Update:
		Msg.Protocol = MP_JOURNAL_UPDATE;
		break;
	case eJournal_Delete:
		Msg.Protocol = MP_JOURNAL_DELETE;
		break;
	}

	Msg.Param = Index;
	Msg.dwObjectID = HERO->GetID();

	NETWORK->Send(&Msg, sizeof(Msg));
}


void CJournalDialog::SetItemCheck(int Index)
{
	m_bCheckItem[Index] = !m_bCheckItem[Index];
}


void CJournalDialog::SelectedJournalSave()
{
	int ViewStartIdx = JOURNALVIEW_PER_PAGE*m_CurPage;
	JournalItem* pItem = NULL;

	for(int i=0; i<MAX_CHECKBOX_PERPAGE; i++)
	{
		if(m_bCheckItem[i])
		{
			// 저장목록은 50개까지
			if(m_JournalSavedList.GetCount() >= 50)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(671));
				return;
			}

			PTRLISTSEARCHSTART(m_JournalList, JournalItem*, pItem);
				if(pItem->ViewIndex == i)
				{
					if(pItem->bSaved)			break;
					
					JournalItem* pSavedItem = new JournalItem;
					memcpy(pSavedItem, pItem, sizeof(JournalItem));
					pItem->bSaved = TRUE;
					pSavedItem->bSaved = TRUE;
					SendNetMsg(pSavedItem->JournalDBIndex, eJournal_Update);
					m_JournalSavedList.AddTail(pSavedItem);
					break;
				}
			PTRLISTSEARCHEND
			m_bCheckItem[i] = FALSE;
		}
	}

	JournalListReset();
}


void CJournalDialog::SelectedJournalDelete()
{
	int ViewStartIdx = JOURNALVIEW_PER_PAGE*m_CurPage;
	JournalItem* pItem = NULL;

	for(int i=0; i<MAX_CHECKBOX_PERPAGE; i++)
	{
		if(m_bCheckItem[i])
		{
			PTRLISTSEARCHSTART(m_JournalSavedList, JournalItem*, pItem);		
				if(pItem->ViewIndex == i)
				{
					// ViewIndex와 비교해서 삭제
					SendNetMsg(pItem->JournalDBIndex, eJournal_Delete);					
					m_JournalSavedList.Remove(pItem);
					delete pItem;
					break;
				}
			PTRLISTSEARCHEND
			m_bCheckItem[i] = FALSE;
		}
	}

	JournalListReset();	
}


void CJournalDialog::ViewJournalListToggle()
{
	// TEST
	if(m_bSavedJournal)
	{
		m_pJournalBtn[0]->SetText(CHATMGR->GetChatMsg(631), RGB(255,255,255));
		m_pJournalBtn[1]->SetText(CHATMGR->GetChatMsg(633), RGB(255,255,255));
	}
	else
	{
		m_pJournalBtn[0]->SetText(CHATMGR->GetChatMsg(632), RGB(255,255,255));
		m_pJournalBtn[1]->SetText(CHATMGR->GetChatMsg(634), RGB(255,255,255));
	}

	m_bSavedJournal = !m_bSavedJournal;
	m_CurPage = 0;

 	// Save CheckBox
 	for(int i=0; i<MAX_CHECKBOX_PERPAGE; i++)
 	{
 		m_bCheckItem[i] = FALSE;
 		m_pCheckBox[i]->SetChecked(FALSE);
 	}

	JournalListReset();
}
