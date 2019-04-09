// ServerListDialog.h: interface for the CServerListDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERLISTDIALOG_H__2827868C_735C_4097_85A8_F9A0693D724B__INCLUDED_)
#define AFX_SERVERLISTDIALOG_H__2827868C_735C_4097_85A8_F9A0693D724B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./interface/cDialog.h"
#include ".\interface\cWindowHeader.h"
#include "cRITEMEx.h"

class cLsitCtrl;

class CServerListDialog : public cDialog  
{
	cListCtrl*	m_pServerListCtrl;
	int			m_nMaxServerNum;
	int			m_nIndex;
	cButton*	m_pConnectBtn;
	cButton*	m_pExitBtn;

public:
	CServerListDialog();
	virtual ~CServerListDialog();

	void	Linking();
	void	LoadServerList();
	virtual	DWORD ActionEvent( CMouse* mouseInfo );

	int		GetSelectedIndex()		{ return m_nIndex; }

};

#endif // !defined(AFX_SERVERLISTDIALOG_H__2827868C_735C_4097_85A8_F9A0693D724B__INCLUDED_)
