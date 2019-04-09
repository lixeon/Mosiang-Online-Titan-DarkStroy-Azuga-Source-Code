// MixManager.cpp: implementation of the CMixManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MixManager.h"
#include "MHFile.h"
#include "GameResourceStruct.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CMixManager)
CMixManager::CMixManager()
{
	m_MixItemTable.Initialize(MAX_ITEM_NUM);
}
void CMixManager::Release()
{
	ITEM_MIX_INFO* pInfo;
	m_MixItemTable.SetPositionHead();
	while(pInfo = m_MixItemTable.GetData())
	{
		if(pInfo->psResultItemInfo)
		{
			for(int i = 0 ; i < pInfo->wResultItemNum ; ++i)
			{
				if(pInfo->psResultItemInfo[i].wResItemIdx != 0)
				{
					if(pInfo->psResultItemInfo[i].psMaterialItemInfo)
					{
						delete [] pInfo->psResultItemInfo[i].psMaterialItemInfo;
						pInfo->psResultItemInfo[i].psMaterialItemInfo = NULL;
					}
				}
			}

			delete [] pInfo->psResultItemInfo;
			pInfo->psResultItemInfo = NULL;
		}
		delete pInfo;
		pInfo = NULL;
	}
	m_MixItemTable.RemoveAll();
}
CMixManager::~CMixManager()
{
	Release();
}
/*

CItemShow * CMixManager::NewShowItem(WORD wItemIdx)
{
	return NULL;
}
void CMixManager::DeleteShowItem(CItemShow * pSItem)
{

}*/
void CMixManager::LoadMixResultItemInfo(CMHFile * fp, ITEM_MIX_RES * pResInfo)
{
	pResInfo->wResItemIdx = fp->GetWord();
	SafeStrCpy(pResInfo->szResItemName, fp->GetString(), MAX_ITEMNAME_LENGTH+1);
	pResInfo->wMixKind = fp->GetWord();
	ASSERT(pResInfo->wMixKind < 7);
	pResInfo->Money = fp->GetDword();
	pResInfo->SuccessRatio = fp->GetWord();
	pResInfo->wMaterialItemNum = fp->GetWord();
	pResInfo->psMaterialItemInfo = new ITEM_MIX_MATERIAL[pResInfo->wMaterialItemNum];
	for( int i = 0 ; i < pResInfo->wMaterialItemNum ; ++i)
	{
		pResInfo->psMaterialItemInfo[i].wMatItemIdx = fp->GetWord();
		SafeStrCpy(pResInfo->psMaterialItemInfo[i].szMatItemName, fp->GetString(), MAX_ITEMNAME_LENGTH+1);
		pResInfo->psMaterialItemInfo[i].wMatItemNum = fp->GetWord();
	}
}
void CMixManager::LoadMixList()
{
	Release();

	CMHFile file;
#ifdef _FILE_BIN_
	if(!file.Init("Resource/ItemMixList.bin", "rb"))
		return;
#else
	if(!file.Init("Resource/ItemMixList.txt", "rt"))
		return;
#endif

	ITEM_MIX_INFO * pInfo = NULL;
	WORD wTmpItemIdx = 0;
	WORD wTmpRestIdx = 0;
	WORD wTmpResultNum = 0;
//	WORD count = 0;
	while(1)
	{
		if(file.IsEOF())
			break;

//		count++;

		wTmpItemIdx = file.GetWord();
		pInfo = m_MixItemTable.GetData(wTmpItemIdx);
		if(!pInfo)
		{
			pInfo = new ITEM_MIX_INFO;
			memset(pInfo, 0, sizeof(ITEM_MIX_INFO));
			pInfo->wItemIdx = wTmpItemIdx;
			SafeStrCpy(pInfo->szItemName, file.GetString(), MAX_ITEMNAME_LENGTH+1);
			wTmpResultNum = file.GetWord();
			pInfo->wResultItemNum = wTmpResultNum;		
		}
		else
		{
			ASSERT(pInfo->wItemIdx == wTmpItemIdx);
			pInfo->wItemIdx = wTmpItemIdx;
			SafeStrCpy(pInfo->szItemName, file.GetString(), MAX_ITEMNAME_LENGTH+1);
			wTmpResultNum = file.GetWord();
			ASSERT(pInfo->wResultItemNum == wTmpResultNum);
			pInfo->wResultItemNum = wTmpResultNum;
		}
		
		if(!pInfo->psResultItemInfo)
		{
			pInfo->psResultItemInfo = new ITEM_MIX_RES[pInfo->wResultItemNum];
			memset(pInfo->psResultItemInfo, 0, sizeof(ITEM_MIX_RES)*pInfo->wResultItemNum);
		}
		for(wTmpRestIdx = 0 ; wTmpRestIdx < pInfo->wResultItemNum ; ++wTmpRestIdx)
		{	
			if(pInfo->psResultItemInfo[wTmpRestIdx].wResItemIdx == 0)
			{
				LoadMixResultItemInfo(&file, &pInfo->psResultItemInfo[wTmpRestIdx]);
				break;
			}
		}

		if(!m_MixItemTable.GetData(pInfo->wItemIdx))
			m_MixItemTable.Add(pInfo, pInfo->wItemIdx);
		pInfo = NULL;
		
	}
	file.Release();
}

ITEM_MIX_INFO * CMixManager::GetMixItemInfo(WORD wItemIdx)
{
	return m_MixItemTable.GetData(wItemIdx);
}

/*
void CMixManager::RealAddItem( )
{

}
*/
