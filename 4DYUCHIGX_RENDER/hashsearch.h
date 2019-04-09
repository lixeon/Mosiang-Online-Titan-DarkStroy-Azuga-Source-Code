#ifndef HASHSEARCH_H
#define HASHSEARCH_H

#include <windows.h>



struct ITEM_CONTAINER
{
	ITEM_CONTAINER*	pPrv;
	ITEM_CONTAINER*	pNext;
	void*			pItem;
	DWORD			dwKey;

	DWORD			dwUIDNum;
	DWORD			dwUID[1];
};

class CHashSearch
{	
	char*					m_pItemContainerPool;
	ITEM_CONTAINER**		m_ppItemContainerTable;
	
	DWORD					m_dwReservedNodeNum;
	DWORD					m_dwReservedItemNum;

	DWORD					m_dwItemNum;
	DWORD					m_dwMaxItemNum;
	DWORD					m_dwItemContainerSize;
	ITEM_CONTAINER**		m_ppHashContainerList;



	ITEM_CONTAINER*			AllocItemContainer();
	void					FreeItemContaiener(ITEM_CONTAINER* pContainer);

public:
	DWORD					GetItemNum() {return m_dwItemNum;}
	//DWORD					DebugReport(void* pItem);
	CHashSearch();
	~CHashSearch();
	BOOL					Initialize(DWORD dwMaxItemNum,DWORD dwUIDNum);
	ITEM_CONTAINER*			InsertItem(DWORD* pdwUID,DWORD dwUIDNum,void* pItem);
	BOOL					DeleteItem(ITEM_CONTAINER*	pItemContainer);
	void*					SearchItem(DWORD* pdwUID,DWORD dwUIDNum);
	void					Cleanup();
};


#endif

