#pragma once
#include "interface\cdialog.h"

class cPushupButton;
class cDialog;

class CTipBrowserDlg :
	public cDialog
{
	cPushupButton*	m_pButton[4];
	cDialog*		m_pDlg[4];
	WORD			m_wCurDlg;

public:
	CTipBrowserDlg(void);
	virtual ~CTipBrowserDlg(void);

	void Show();
	void Close();

	void Linking();
	void OnActionEvent(LONG lId, void * p, DWORD we);
};
