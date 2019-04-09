// ShoutDialog.h: interface for the CShoutDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOUTDIALOG_H__2851FDE7_5AC8_4835_8F46_900FC49814D7__INCLUDED_)
#define AFX_SHOUTDIALOG_H__2851FDE7_5AC8_4835_8F46_900FC49814D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"

class cEditBox;



class CShoutDialog : public cDialog
{
	DWORD			m_dwItemIdx;
	DWORD			m_dwItemPos;
	cEditBox*		m_pMsgBox;
	

public:
	CShoutDialog();
	virtual ~CShoutDialog();

	void Linking();
	BOOL SendShoutMsgSyn();

	void SetItemInfo( DWORD ItemIdx, DWORD ItemPos )			{	m_dwItemIdx = ItemIdx;	m_dwItemPos = ItemPos;	}
};

#endif // !defined(AFX_SHOUTDIALOG_H__2851FDE7_5AC8_4835_8F46_900FC49814D7__INCLUDED_)
