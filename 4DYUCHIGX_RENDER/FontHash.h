#ifndef FONTHASH_H
#define FONTHASH_H

#include <windows.h>



struct FONTCACHE_ITEM_CONTAINER
{
	FONTCACHE_ITEM_CONTAINER*	pPrv;
	FONTCACHE_ITEM_CONTAINER*	pNext;
	void*			pItem;
	DWORD			dwKey;
	
	
	DWORD			dwKeySize;
	
	
	HFONT			hFont;
	DWORD			dwColor;
	char			szString[1];
};

class CFontHash
{	
	DWORD						m_dwItemSize;
	char*						m_pItemContainerPool;
	FONTCACHE_ITEM_CONTAINER**	m_ppItemContainerTable;
	
	DWORD						m_dwReservedNodeNum;
	DWORD						m_dwReservedItemNum;

	DWORD						m_dwItemNum;
	DWORD						m_dwMaxItemNum;
	DWORD						m_dwItemContainerSize;
	FONTCACHE_ITEM_CONTAINER**	m_ppHashContainerList;



	FONTCACHE_ITEM_CONTAINER*	AllocItemContainer();
	void						FreeItemContaiener(FONTCACHE_ITEM_CONTAINER* pContainer);

public:
	DWORD						GetItemNum() {return m_dwItemNum;}
	
	CFontHash();
	~CFontHash();
	BOOL						Initialize(DWORD dwMaxItemNum,DWORD dwMaxKeySize);
	FONTCACHE_ITEM_CONTAINER*	InsertItem(char* pKey,DWORD dwSize,HFONT hFont,DWORD dwColor,void* pItem);
	BOOL						DeleteItem(FONTCACHE_ITEM_CONTAINER*	pItemContainer);
	void*						SearchItem(char* pKey,DWORD dwSize,HFONT hFont,DWORD dwColor);
	void						Cleanup();
};


#endif

