// ItemCount.cpp: implementation of the CItemCount class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "4dyuchigxmapeditor.h"
#include "ItemCount.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
// Various Bytes HashTable
typedef void (*DelFunc)(void* pVoid);
GLOBAL_FUNC_DLL DWORD			__stdcall	VBHDeleteWithKey(VBHASH_HANDLE pHash,void* pKeyData,DWORD dwSize,DelFunc pDelFunc);



GLOBAL_FUNC_DLL void*			__stdcall	VBHGetSearchHandle(VBHASH_HANDLE pHash,void* pKeyData,DWORD dwSize);


GLOBAL_FUNC_DLL	VBHASH_HANDLE	__stdcall	VBHCreate();
GLOBAL_FUNC_DLL	BOOL			__stdcall	
GLOBAL_FUNC_DLL	DWORD			__stdcall	VBHGetMaxBucketNum(VBHASH_HANDLE pHash);
GLOBAL_FUNC_DLL void			__stdcall	VBHDeleteAll(VBHASH_HANDLE pHash);
*/

CItemCount::CItemCount()
{
	memset(this,0,sizeof(CItemCount));

	
	
	
}

BOOL CItemCount::Initialize(DWORD dwMaxKeySize,DWORD dwMaxItemClassNum)
{
	m_dwMaxItemClassNum = dwMaxItemClassNum;
	m_dwMaxKeySize = dwMaxKeySize;

	m_ppItemList = new COUNT_ITEM*[m_dwMaxItemClassNum];
	memset(m_ppItemList,0,sizeof(COUNT_ITEM*)*m_dwMaxItemClassNum);

	m_pHashHandle = VBHCreate();
	VBHInitialize(m_pHashHandle,128,m_dwMaxKeySize,dwMaxItemClassNum);

	return TRUE;
}

BOOL CItemCount::Insert(DWORD dwItem,char* pKey,DWORD dwSize)
{
	if (m_dwItemClassNum >= m_dwMaxItemClassNum)
		return FALSE;

	if (dwSize > m_dwMaxKeySize)
		return FALSE;

	DWORD dwResult;

	if (VBHSelect(m_pHashHandle,&dwResult,1,pKey,dwSize))
	{
		m_ppItemList[dwResult]->dwCount++;
		ITEM_LINK*	pNew = new ITEM_LINK;
		pNew->dwItem = dwItem;
		pNew->pNext = m_ppItemList[dwResult]->item.pNext;
		m_ppItemList[dwResult]->item.pNext = pNew;
		
	}
	else
	{
		m_ppItemList[m_dwItemClassNum] = (COUNT_ITEM*)malloc(COUNT_ITEM_SIZE+dwSize);
		m_ppItemList[m_dwItemClassNum]->dwCount = 0;
		m_ppItemList[m_dwItemClassNum]->dwCount++;
		m_ppItemList[m_dwItemClassNum]->dwSize = dwSize;
		m_ppItemList[m_dwItemClassNum]->item.dwItem = dwItem;
		m_ppItemList[m_dwItemClassNum]->item.pNext = NULL;

		memcpy(m_ppItemList[m_dwItemClassNum]->pKey,pKey,dwSize);

		if (m_ppItemList[m_dwItemClassNum]->pSearchHandle = VBHInsert(m_pHashHandle,m_dwItemClassNum,pKey,dwSize))
			m_dwItemClassNum++;
		else
		{
			free(m_ppItemList[m_dwItemClassNum]);
			m_ppItemList[m_dwItemClassNum] = NULL;
			return FALSE;
		}
	}
	return TRUE;

}

DWORD CItemCount::GetItem(DWORD* pdwResult,DWORD dwClassIndex,DWORD dwMaxItemNum)
{

	DWORD	dwCount = 0;
	ITEM_LINK*	pCur = &m_ppItemList[dwClassIndex]->item;
	while (pCur)
	{
		if (dwCount >= dwMaxItemNum)
			goto lb_return;

		pdwResult[dwCount] = pCur->dwItem;
		dwCount++;
		pCur = pCur->pNext;
	}
lb_return:
	return dwCount;
}
CItemCount::~CItemCount()
{
	VBHDeleteAll(m_pHashHandle);

	for (DWORD i=0; i<m_dwItemClassNum; i++)
	{
		ITEM_LINK*	pNext;

		
		while (m_ppItemList[i]->item.pNext)
		{
			pNext = m_ppItemList[i]->item.pNext->pNext;
			delete m_ppItemList[i]->item.pNext;
			m_ppItemList[i]->item.pNext = pNext;

		}
		free(m_ppItemList[i]);
		m_ppItemList[i] = NULL;
	}
	VBHRelease(m_pHashHandle);
	delete [] m_ppItemList;
	m_ppItemList = NULL;
}
