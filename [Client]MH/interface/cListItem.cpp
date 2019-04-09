// cListItem.cpp: implementation of the cListItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "cListItem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
template<class Type>
cListItem<Type>::cListItem()
{
	m_pItemList = new cLinkedList<Type>;
}
template<class Type>
cListItem<Type>::~cListItem()
{
	RemoveAll();
	SAFE_DELETE(m_pItemList);
}
template<class Type>
LONG cListItem<Type>::GetItemCount()
{
	return m_pItemList->GetCount();
}
template<class Type>
void cListItem<Type>::AddItem(char * str)
{
	Type * item = new Type;
	wsprintf(item->string, str);
	item->rgb = RGBA_MAKE(255,255,255,255);
	m_pItemList->AddTail(item);
}
template<class Type>
void cListItem<Type>::AddItem(char * str, LONG idx)
{
	Type * item = new Type;
	wsprintf(item->string, str);
	item->rgb = RGBA_MAKE(255,255,255,255);
	if(m_wMaxLine < GetItemCount())
		m_pItemList->DeleteHead();
	m_pItemList->InsertpAfter(item, idx);
}
template<class Type>
void cListItem<Type>::AddItem(char * str, DWORD color)
{
	Type * item = new Type;
	wsprintf(item->string, str);
	item->rgb = color;
	if(m_wMaxLine < GetItemCount())
		m_pItemList->DeleteHead();
		
	m_pItemList->AddTail(item);
}
template<class Type>
void cListItem<Type>::RemoveAll()
{
	m_pItemList->DeleteAll();
}
template<class Type>
void cListItem<Type>::RemoveItem(LONG idx)
{
	Type * item = m_pItemList->DeleteAt(idx);
	SAFE_DELETE(item);
}
template<class Type>
void cListItem<Type>::RemoveItem(char * str)
{
	POS pos = m_pItemList->GetFirstPos();
	Type * item = NULL;
	while (pos)
    {
        item = m_pItemList->GetNextPos(pos);
		if(strcmp(item->string, str) == 0)
		{
			m_pItemList->GetPrevPos(pos);
			m_pItemList->DeletePos(pos);
			SAFE_DELETE(item);
		}
	}
}
*/