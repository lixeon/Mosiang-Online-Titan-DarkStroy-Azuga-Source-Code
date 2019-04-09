// AlertDlg.cpp: implementation of the CAlertDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AlertDlg.h"
#include "./Interface/cButton.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAlertDlg::CAlertDlg()
{
	m_pOk=NULL;
	m_pCancel=NULL;
}

CAlertDlg::~CAlertDlg()
{
	m_pOk=NULL;
	m_pCancel=NULL;
}

DWORD CAlertDlg::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	DWORD we2 = WE_NULL;
	we |= cWindow::ActionEvent(mouseInfo);
	we |= cDialog::ActionEventWindow(mouseInfo);
	we2 = m_pOk->ActionEvent(mouseInfo);
	if(we2 & WE_BTNCLICK)
		cbBtnFunc(m_ID, this, 1);
	we2 = m_pCancel->ActionEvent(mouseInfo);
	if(we2 & WE_BTNCLICK)
		cbBtnFunc(m_ID, this, 0);
	return we;
}

void CAlertDlg::Add(cWindow * window)
{
	if(window->GetType() == WT_BUTTON)
	{
		if(!m_pOk)
		{
			m_pOk = (cButton *)window;
		}
		else if(!m_pCancel)
		{
			m_pCancel = (cButton *)window;
		}
	}
	cDialog::Add(window);
}