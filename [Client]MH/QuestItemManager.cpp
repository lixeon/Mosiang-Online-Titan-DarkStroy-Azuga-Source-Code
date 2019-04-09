// QuestItemManager.cpp: implementation of the CQuestItemManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestItemManager.h"
#include "mhFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CQuestItemManager::CQuestItemManager()
{
	m_QuestItemList.Initialize(200);
}

CQuestItemManager::~CQuestItemManager()
{	
	Release();
}

void CQuestItemManager::Release()
{
	QUEST_ITEM_INFO* pData = NULL;
	m_QuestItemList.SetPositionHead();
	while( pData = m_QuestItemList.GetData() )
	{
		delete pData;
		pData = NULL;
	}
	m_QuestItemList.RemoveAll();
}

void CQuestItemManager::QuestItemInfoLoad()
{
CMHFile file;
#ifdef _FILE_BIN_
	if(!file.Init("./Resource/QuestScript/QuestItemList.bin", "rb"))
		return;
#else
	if(!file.Init("./Resource/QuestScript/QuestItemList.txt", "rt"))
		return;
#endif
	/*
#ifdef _FILE_BIN_
	if(!file.Init("Quest/QuestItemList.bin", "rb"))
		return;
#else
	if(!file.Init("Quest/QuestItemList.txt", "rt"))
		return;
#endif
	*/

	int Key_1, Key_2;
	int Key = 0;
	QUEST_ITEM_INFO * pInfo = NULL;	

	int max = 0;
	while(1)
	{
		if(file.IsEOF())
			break;


		ASSERT(!pInfo);
		pInfo = new QUEST_ITEM_INFO;

		pInfo->ItemIdx = file.GetDword();

		//SafeStrCpy(pInfo->ItemName, file.GetString(), MAX_ITEMNAME_LENGTH+1);
		SafeStrCpyEmptyChange(pInfo->ItemName, file.GetString(), MAX_ITEMNAME_LENGTH+1);
		Key_1 = file.GetDword();
		Key_2 = file.GetDword();
		COMBINEKEY(Key_1, Key_2, Key);
		pInfo->QuestKey = Key;
		pInfo->Image2DNum = file.GetWord();
		pInfo->SellPrice = file.GetWord();
		//SafeStrCpy(pInfo->ItemDesc, file.GetString(), MAX_ITEMDESC_LENGTH+1);		
		SafeStrCpyEmptyChange(pInfo->ItemDesc, file.GetString(), MAX_ITEMDESC_LENGTH+1);	

		ASSERT(m_QuestItemList.GetData(pInfo->ItemIdx) == FALSE);
		m_QuestItemList.Add(pInfo, pInfo->ItemIdx);

		max = pInfo->ItemIdx;

		pInfo = NULL;
	}

	file.Release();
}


QUEST_ITEM_INFO* CQuestItemManager::GetQuestItem(DWORD ItemIdx)
{
	return m_QuestItemList.GetData(ItemIdx);
}
