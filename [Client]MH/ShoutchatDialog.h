// ShoutchatDialog.h: interface for the CShoutchatDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOUTCHATDIALOG_H__C3390657_73EF_488D_B24F_89B12F3C0131__INCLUDED_)
#define AFX_SHOUTCHATDIALOG_H__C3390657_73EF_488D_B24F_89B12F3C0131__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"


class cListDialog;


class CShoutchatDialog : public cDialog
{
	cListDialog*	m_pMsgListDlg;

	DWORD			m_LastMsgTime;

public:
	CShoutchatDialog();
	virtual ~CShoutchatDialog();

	void Linking();
	void Process();
	void SetActive( BOOL val );

	void AddMsg( char* str );

	void RefreshPosition();
};

#endif // !defined(AFX_SHOUTCHATDIALOG_H__C3390657_73EF_488D_B24F_89B12F3C0131__INCLUDED_)
