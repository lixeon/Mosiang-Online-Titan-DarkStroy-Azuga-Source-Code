// ListBoxContainer.h: interface for the CListBoxContainer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LISTBOXCONTAINER_H__84A36B2E_5520_475E_A4FC_B98F609ABE40__INCLUDED_)
#define AFX_LISTBOXCONTAINER_H__84A36B2E_5520_475E_A4FC_B98F609ABE40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../4DyuchiGXGFunc/global.h"
class CListBoxContainer  
{
	VBHASH_HANDLE			m_pNameHash;

	DelFunc					m_pDelFunc;
	CListBox*				m_pListBox;
public:
	BOOL					Initialize(DWORD dwMaxItemNum,DWORD dwMaxSize,CListBox* pListBox,DelFunc pDelFunc);
	BOOL					InsertItem(char* szName,void* pItem);
	void*					GetSelectedItem();
	BOOL					DeleteSelectedItem();
	BOOL					DeleteItem(char* szName);
	void					DeleteAll();
	void*					GetItem(int iSeqIndex);
	int						GetItemNum();
	
	CListBoxContainer();
	virtual ~CListBoxContainer();

};

#endif // !defined(AFX_LISTBOXCONTAINER_H__84A36B2E_5520_475E_A4FC_B98F609ABE40__INCLUDED_)
