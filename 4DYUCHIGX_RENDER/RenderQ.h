#ifndef RENDER_Q_H
#define RENDER_Q_H

#include "VariableBuffer.h"
#include "../4DyuchiGXGFunc/global.h"


class CRenderQueue
{
	DWORD				m_dwMaxItemNum;
	DWORD				m_dwCurrentItemNum;

	SORT_ITEM_FLOAT*	m_pRenderItemList;
	DWORD				m_dwItemSize;
	CVariableBuffer		m_Buffer;
	//	char*			m_pBuffer;

	
public:
	BOOL			Initialize(DWORD dwItemSize,DWORD dwMaxItemNum);
	BOOL			InsertItem(char* pItem,float fDistance);
	void			Sort()							{ QSortFloatDSC(m_pRenderItemList,m_dwCurrentItemNum); }
	

	void*			GetItemSeq(DWORD dwIndex)		{ return m_pRenderItemList[dwIndex].pItem;}
	DWORD			GetItemNum()					{ return m_dwCurrentItemNum;}
	void			Clear()							{ m_dwCurrentItemNum = 0;m_Buffer.Clear();}
	CRenderQueue();
	~CRenderQueue();



};

#endif

