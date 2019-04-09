// QuestUpdater.cpp: implementation of the CQuestUpdater class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestUpdater.h"
#include "GameIn.h"
#include "QuestTotalDialog.h"
#include "Player.h"
#include "Quest.h"
#include "..\[CC]Quest\QuestInfo.h"
#include "..\[CC]Quest\SubQuestInfo.h"
#include "..\[CC]Quest\QuestString.h"
#include "QuestManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestUpdater::CQuestUpdater()
{

}

CQuestUpdater::~CQuestUpdater()
{

}


void CQuestUpdater::JournalItemAdd(JOURNALINFO* pItem)
{
	GAMEIN->GetQuestTotalDialog()->JournalItemAdd(pItem);
}

void CQuestUpdater::CompleteQuestDelete(CQuestString* pQuestString)
{
	GAMEIN->GetQuestTotalDialog()->CompleteQuestDelete(pQuestString);
}

void CQuestUpdater::ProcessQuestAdd(CQuestString* pQuestString)
{	
	GAMEIN->GetQuestTotalDialog()->ProcessQuestAdd(pQuestString);
}

void CQuestUpdater::ProcessQuestDelete(CQuestString* pQuestString)
{
	GAMEIN->GetQuestTotalDialog()->ProcessQuestDelete(pQuestString);
}

void  CQuestUpdater::QuestItemAdd(QUEST_ITEM_INFO* pQuestItemInfo, DWORD Count)
{
	GAMEIN->GetQuestTotalDialog()->QuestItemAdd(pQuestItemInfo, Count);
}

void CQuestUpdater::QuestItemDelete(DWORD ItemIdx)
{
	GAMEIN->GetQuestTotalDialog()->QuestItemDelete(ItemIdx);
}

void CQuestUpdater::QuestItemUpdate(DWORD type, DWORD ItemIdx, DWORD Count)
{
	GAMEIN->GetQuestTotalDialog()->QuestItemUpdate(type, ItemIdx, Count);
}

DWORD CQuestUpdater::GetQuestItemCount(DWORD ItemIdx)
{
	return GAMEIN->GetQuestTotalDialog()->QuestItemUpdate(eQItem_GetCount, ItemIdx, 0);
}

void CQuestUpdater::DeleteQuest(DWORD QuestIdx)
{
	GAMEIN->GetQuestTotalDialog()->GiveupQuestDelete(QuestIdx);
}

DWORD CQuestUpdater::GetSelectedQuestID()
{
	return GAMEIN->GetQuestTotalDialog()->GetSelectedQuestID();
}

void CQuestUpdater::CloseMsgBox()
{
	GAMEIN->GetQuestTotalDialog()->CloseMsgBox();
}

void CQuestUpdater::UpdateSubQuestData()
{
	GAMEIN->GetQuestTotalDialog()->UpdateSubQuestData();
}