#include "stdafx.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cPushupButton.h"
#include "./Interface/cDialog.h"
#include "./Input/Mouse.h"
#include "ReinforceDataGuideDlg.h"


CReinforceDataGuideDlg::CReinforceDataGuideDlg(void)
{
	for(int i = 0; i < 9; i++)
	{
		m_pItemKindButton[i] = NULL;
		m_pDataDlg[i] = NULL;
	}

	m_wCurData = 0;
}

CReinforceDataGuideDlg::~CReinforceDataGuideDlg(void)
{
	
}

void CReinforceDataGuideDlg::Linking()
{
	m_pItemKindButton[0] = (cPushupButton*)GetWindowForID(RFDG_BTN1);
	m_pItemKindButton[1] = (cPushupButton*)GetWindowForID(RFDG_BTN2);
	m_pItemKindButton[2] = (cPushupButton*)GetWindowForID(RFDG_BTN3);
	m_pItemKindButton[3] = (cPushupButton*)GetWindowForID(RFDG_BTN4);
	m_pItemKindButton[4] = (cPushupButton*)GetWindowForID(RFDG_BTN5);
	m_pItemKindButton[5] = (cPushupButton*)GetWindowForID(RFDG_BTN6);
	m_pItemKindButton[6] = (cPushupButton*)GetWindowForID(RFDG_BTN7);
	m_pItemKindButton[7] = (cPushupButton*)GetWindowForID(RFDG_BTN8);
	m_pItemKindButton[8] = (cPushupButton*)GetWindowForID(RFDG_BTN9);

	m_pDataDlg[0] = (cDialog*)GetWindowForID(GUIDE_SHEET1);
	m_pDataDlg[1] = (cDialog*)GetWindowForID(GUIDE_SHEET2);
	m_pDataDlg[2] = (cDialog*)GetWindowForID(GUIDE_SHEET3);
	m_pDataDlg[3] = (cDialog*)GetWindowForID(GUIDE_SHEET4);
	m_pDataDlg[4] = (cDialog*)GetWindowForID(GUIDE_SHEET5);
	m_pDataDlg[5] = (cDialog*)GetWindowForID(GUIDE_SHEET6);
	m_pDataDlg[6] = m_pDataDlg[5];
	m_pDataDlg[7] = (cDialog*)GetWindowForID(GUIDE_SHEET7);
	m_pDataDlg[8] = m_pDataDlg[7];
}

void CReinforceDataGuideDlg::Show()
{
	SetActive(TRUE);

	for(int i = 0; i < 9; i++)
	{
		m_pDataDlg[i]->SetActive(FALSE);
		m_pItemKindButton[i]->SetPush(FALSE);
		m_pItemKindButton[i]->SetDisable(FALSE);
	}
	
	m_pDataDlg[m_wCurData]->SetActive(TRUE);
	m_pItemKindButton[m_wCurData]->SetPush(TRUE);
	m_pItemKindButton[m_wCurData]->SetDisable(TRUE);
}

void CReinforceDataGuideDlg::Close()
{
	SetActive(FALSE);

	m_wCurData = 0;
}

void CReinforceDataGuideDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if( we == WE_PUSHDOWN )
	{
		WORD id = (WORD)(lId - RFDG_BTN1);
		if( id < 9 && (lId - RFDG_BTN1) >= 0 )
		{
			SelectData(id);

			Show();

			return;
		}
	}
	
	if( we & WE_BTNCLICK && lId == RFDGUIDE_OKBTN )
	{
		Close();
	}
}

DWORD CReinforceDataGuideDlg::ActionEvent(CMouse * mouseInfo)
{
	if( !m_bActive ) return WE_NULL;

	DWORD we = WE_NULL;
	we |= cWindow::ActionEvent(mouseInfo);

	if( !m_bDisable )
	{
		we |= ActionEventWindow(mouseInfo);
		we |= ActionEventComponent(mouseInfo);
	}
	return we;	
}

DWORD CReinforceDataGuideDlg::ActionEventWindow(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;

	if( m_bDisable ) return we;
	we = cDialog::ActionEventWindow( mouseInfo );

	if( mouseInfo->LButtonDown() )
		if( !PtInWindow( mouseInfo->GetMouseX(), mouseInfo->GetMouseY() ) )
		{

		}

		return we;

}
