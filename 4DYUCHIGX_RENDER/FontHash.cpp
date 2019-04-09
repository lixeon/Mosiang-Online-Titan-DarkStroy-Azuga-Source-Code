#include "FontHash.h"


CFontHash::CFontHash()
{
	memset(this,0,sizeof(CFontHash));

}

DWORD __stdcall CreateKey(void* pData,DWORD dwSize,DWORD dwBucketNum);

BOOL CFontHash::Initialize(DWORD dwMaxItemNum,DWORD dwMaxKeySize)
{
	m_dwMaxItemNum = dwMaxItemNum;
	m_dwItemContainerSize = sizeof(FONTCACHE_ITEM_CONTAINER) - sizeof(char) + dwMaxKeySize;

	m_dwMaxItemNum = dwMaxItemNum;
	
	
	m_pItemContainerPool = new char[m_dwItemContainerSize*m_dwMaxItemNum];
	memset(m_pItemContainerPool,0,m_dwItemContainerSize*m_dwMaxItemNum);
	
	
	m_ppItemContainerTable = new FONTCACHE_ITEM_CONTAINER*[m_dwMaxItemNum];
	memset(m_ppItemContainerTable,0,sizeof(FONTCACHE_ITEM_CONTAINER*)*m_dwMaxItemNum);


	m_ppHashContainerList = new FONTCACHE_ITEM_CONTAINER*[m_dwMaxItemNum];
	memset(m_ppHashContainerList,0,sizeof(FONTCACHE_ITEM_CONTAINER*)*m_dwMaxItemNum);

	for (DWORD i=0; i<m_dwMaxItemNum; i++)
	{
		m_ppItemContainerTable[i] = (FONTCACHE_ITEM_CONTAINER*)(m_pItemContainerPool + m_dwItemContainerSize*i);
	}
	m_dwReservedNodeNum = m_dwMaxItemNum;
	m_dwReservedItemNum = m_dwMaxItemNum;

	return TRUE;
}

FONTCACHE_ITEM_CONTAINER* CFontHash::AllocItemContainer()
{

	FONTCACHE_ITEM_CONTAINER*	pContainer;

	pContainer = m_ppItemContainerTable[0];
	m_ppItemContainerTable[0] = m_ppItemContainerTable[m_dwReservedItemNum-1];
	m_dwReservedItemNum--;

	pContainer->pPrv = NULL;
	pContainer->pNext = NULL;
	pContainer->pItem = NULL;
	

	return pContainer;

}
void CFontHash::FreeItemContaiener(FONTCACHE_ITEM_CONTAINER* pContainer)
{
	m_ppItemContainerTable[m_dwReservedItemNum] = pContainer;
	m_dwReservedItemNum++;
}

	
FONTCACHE_ITEM_CONTAINER* CFontHash::InsertItem(char* pKey,DWORD dwSize,HFONT hFont,DWORD dwColor,void* pItem)
{
	FONTCACHE_ITEM_CONTAINER*	pNewContainer;
	
	DWORD	dwKey = CreateKey(pKey,dwSize,m_dwMaxItemNum);
	
	
	pNewContainer = AllocItemContainer();
	if (!pNewContainer)
		goto lb_false;

	pNewContainer->dwKey = dwKey;
	pNewContainer->hFont = hFont;
	pNewContainer->dwKeySize = dwSize;
	pNewContainer->dwColor = dwColor;
	memcpy(pNewContainer->szString,pKey,dwSize);

	pNewContainer->pItem = pItem;	// 아이템
	


	if (m_ppHashContainerList[dwKey])
	{
		pNewContainer->pNext = m_ppHashContainerList[dwKey];
		m_ppHashContainerList[dwKey]->pPrv = pNewContainer;
		m_ppHashContainerList[dwKey] = pNewContainer;
	}
	else
	{
		m_ppHashContainerList[dwKey] = pNewContainer;
	}
	m_dwItemNum++;
lb_false:
	return pNewContainer;
}

void* CFontHash::SearchItem(char* pKey,DWORD dwSize,HFONT hFont,DWORD dwColor)
{
	void*	pItem = NULL;
	DWORD	dwKey = CreateKey(pKey,dwSize,m_dwMaxItemNum);
	

	FONTCACHE_ITEM_CONTAINER*	pCur = m_ppHashContainerList[dwKey];
	
	while (pCur)
	{
		if (pCur->dwKeySize != dwSize)
			goto lb_skip;

		if (pCur->hFont != hFont)
			goto lb_skip;

		if (pCur->dwColor != dwColor)
			goto lb_skip;

		if (memcmp(pCur->szString,pKey,dwSize))
			goto lb_skip;

		goto lb_find;


lb_skip:
		pCur = pCur->pNext;
	}

	goto lb_return;

lb_find:
	pItem = pCur->pItem;

lb_return:
	return pItem;

}
	
BOOL CFontHash::DeleteItem(FONTCACHE_ITEM_CONTAINER* pItemContainer)
{
	BOOL	bResult = FALSE;
	FONTCACHE_ITEM_CONTAINER*		pCur = pItemContainer;
		

lb_find:
	FONTCACHE_ITEM_CONTAINER*		pNext;
	FONTCACHE_ITEM_CONTAINER*		pPrv;
	pNext = pCur->pNext;
	pPrv = pCur->pPrv;

	if (pPrv)
		pPrv->pNext = pNext;
	else
	{
		// 이게 맨 처음이다.
		m_ppHashContainerList[pItemContainer->dwKey] = pNext;
	}

	if (pNext)
		pNext->pPrv = pPrv;
	

	FreeItemContaiener(pCur);
	bResult = TRUE;
lb_return:
	return bResult;
}
void CFontHash::Cleanup()
{
	
	if (m_pItemContainerPool)
	{
		delete [] m_pItemContainerPool;
		m_pItemContainerPool = NULL;
	}

	if (m_ppItemContainerTable)
	{
		delete [] m_ppItemContainerTable;
		m_ppItemContainerTable = NULL;
	}
	if (m_ppHashContainerList)
	{
		delete [] m_ppHashContainerList;
		m_ppHashContainerList = NULL;
	}
	memset(this,0,sizeof(CFontHash));

}

CFontHash::~CFontHash()
{
	Cleanup();
}
DWORD __declspec(naked) __stdcall CreateKey(void* pData,DWORD dwSize,DWORD dwBucketNum)
{
	__asm
	{
		push		ebx
		push		ecx
		push		edx
		push		esi


		xor			eax,eax
		
		mov			esi,dword ptr[esp+4+4*4]	; pData
		mov			ecx,dword ptr[esp+8+4*4]	; dwSize
lb_1:
		test		ecx,1
		jz			lb_2

		mov			dl,byte ptr[esi]
		or			al,dl
		inc			esi
		dec			ecx
		jz			lb_end

lb_2:
		test		ecx,2
		jz			lb_4

		mov			dx,word ptr[esi]
		or			ax,dx
		add			esi,2
		sub			ecx,2
		jz			lb_end

lb_4:
		shr			ecx,2
lb_4_loop:
		mov			edx,dword ptr[esi]
		or			eax,edx
		loop		lb_4_loop

lb_end:
		mov			ebx,dword ptr[esp+12+4*4]	; dwMaxBucketNum
		xor			edx,edx
		div			ebx

		mov			eax,edx

		pop			esi
		pop			edx
		pop			ecx
		pop			ebx
		
		ret			4*3
	}
}