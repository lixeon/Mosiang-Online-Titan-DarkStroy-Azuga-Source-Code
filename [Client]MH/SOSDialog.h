// SOSDialog.h: interface for the CSOSDlg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _SOSDIALOG_H
#define _SOSDIALOG_H

#if	_MSC_VER > 1000
#pragma once
#endif

#include "./interface/cDialog.h"

class cListDialog;

class CSOSDlg : public cDialog
{
	cListDialog*	m_pListDlg;
	cButton*		m_pSOSOkBtn;
	DWORD			m_dwSelectIdx;

public:
	CSOSDlg();
	virtual ~CSOSDlg();
	void Linking();
	virtual void SetActive(BOOL val);
	virtual DWORD ActionEvent(CMouse* mouseInfo);
	void SOSMemberInfo();
	void OnActionEvent(LONG lId, void * p, DWORD we);
};

#endif	// _SOSDIALOG_H