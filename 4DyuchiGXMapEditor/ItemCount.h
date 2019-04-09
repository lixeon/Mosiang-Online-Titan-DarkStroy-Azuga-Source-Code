// ItemCount.h: interface for the CItemCount class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMCOUNT_H__BD49F090_6668_489B_8B9B_1EAF30F03B12__INCLUDED_)
#define AFX_ITEMCOUNT_H__BD49F090_6668_489B_8B9B_1EAF30F03B12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../4DyuchiGXGFunc/global.h"

struct ITEM_LINK
{
	DWORD		dwItem;
	ITEM_LINK*	pNext;
};
struct COUNT_ITEM
{
	ITEM_LINK	item;
	void*	pSearchHandle;
	DWORD	dwCount;
	DWORD	dwSize;
	char	pKey[1];
};
#define COUNT_ITEM_SIZE	20
class CItemCount  
{
	DWORD				m_dwMaxItemClassNum;
	DWORD				m_dwItemClassNum;
	COUNT_ITEM**		m_ppItemList;
	VBHASH_HANDLE		m_pHashHandle;
	DWORD				m_dwMaxKeySize;
	
public:
	BOOL				Initialize(DWORD dwMaxItemSize,DWORD dwMaxItemClassNum);
	BOOL				Insert(DWORD dwItem,char* pKey,DWORD dwSize);
	DWORD				GetItemClassNum() {return m_dwItemClassNum;}
	DWORD				GetItemNum(DWORD dwClassIndex) {return m_ppItemList[dwClassIndex]->dwCount;}
	DWORD				GetItem(DWORD* pdwResult,DWORD dwClassIndex,DWORD dwMaxItemNum);
	

	CItemCount();
	~CItemCount();

};

#endif // !defined(AFX_ITEMCOUNT_H__BD49F090_6668_489B_8B9B_1EAF30F03B12__INCLUDED_)
