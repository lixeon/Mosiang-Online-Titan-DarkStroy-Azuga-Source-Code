#include "stdafx.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cPushupButton.h"
#include "./Interface/cDialog.h"
#include "tipbrowserdlg.h"

CTipBrowserDlg::CTipBrowserDlg(void)
{
	for(int i = 0; i < 4; i++)
	{
		m_pButton[i] = NULL;
		m_pDlg[i] = NULL;
	}

	m_wCurDlg = 0;
}

CTipBrowserDlg::~CTipBrowserDlg(void)
{
	for(int i = 0; i < 4; i++)
	{
		m_pButton[i] = NULL;
		m_pDlg[i] = NULL;
	}

	m_wCurDlg = 0;
}

void CTipBrowserDlg::Linking()
{
	for(int i = 0; i < 4; i++ )
	{
		m_pDlg[i] = (cDialog*)GetWindowForID(TB_STATE_PO + i);
		m_pButton[i] = (cPushupButton*)GetWindowForID(TB_STATE_PUSHUP1 + i);
	}

	m_wCurDlg = 0;
}

void CTipBrowserDlg::Show()
{
	SetActive(TRUE);

	for(int i = 0; i < 4; i++)
	{
		m_pDlg[i]->SetActive(FALSE);
		m_pButton[i]->SetPush(FALSE);
		m_pButton[i]->SetDisable(FALSE);
	}
	
	m_pDlg[m_wCurDlg]->SetActive(TRUE);
	m_pButton[m_wCurDlg]->SetPush(TRUE);
	m_pButton[m_wCurDlg]->SetDisable(TRUE);}

void CTipBrowserDlg::Close()
{
	SetActive(FALSE);

	m_wCurDlg = 0;
}

void CTipBrowserDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if( we == WE_PUSHDOWN )
	{
		WORD id = (WORD)(lId - TB_STATE_PUSHUP1);
		if( id < 4 && (lId - TB_STATE_PUSHUP1) >= 0 )
		{
			m_wCurDlg = id;

			Show();

			return;
		}
	}
	
	if( we & WE_BTNCLICK && lId == TB_CANCELBTN )
	{
		Close();
	}
}