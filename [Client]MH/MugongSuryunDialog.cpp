// MugongSuryunDialog.cpp: implementation of the CMugongSuryunDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MugongSuryunDialog.h"
#include "MugongDialog.h"
#include "SuryunDialog.h"

#include "GameIn.h"
#include "MainBarDialog.h"

#include "cMsgBox.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMugongSuryunDialog::CMugongSuryunDialog()
{
	m_type = WT_MUGONGSURYUNDIALOG;	
	m_pMugongDlg = NULL;
	m_pSuryunDlg = NULL;
}

CMugongSuryunDialog::~CMugongSuryunDialog()
{

}

void CMugongSuryunDialog::Add(cWindow * window)
{
	if(window->GetType() == WT_MUGONGDIALOG)
		m_pMugongDlg = (CMugongDialog*)window;
	else if(window->GetType() == WT_MUGONGDIALOG)
		m_pSuryunDlg = (CSuryunDialog*)window;


	if(window->GetType() == WT_PUSHUPBUTTON)
		AddTabBtn(curIdx1++, (cPushupButton * )window);
	else if(window->GetType() == WT_MUGONGDIALOG ||
		window->GetType() == WT_SURYUNDIALOG )
	
	{
		AddTabSheet(curIdx2++, window);
	}
	else 
		cDialog::Add(window);
}

void CMugongSuryunDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	
}

BOOL CMugongSuryunDialog::FakeMoveIcon(LONG x, LONG y, cIcon * icon)
{
	return m_pMugongDlg->FakeMoveIcon(x,y,icon);
}

void CMugongSuryunDialog::SetActive( BOOL val )
{
	if(!val)
	{
		cMsgBox* pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID(MBI_MUGONGDELETE);
		if(pMsgBox)
			WINDOWMGR->AddListDestroyWindow(pMsgBox);
		
		SetDisable(FALSE);
	}

	cTabDialog::SetActive( val );
	
//--- main bar icon
//	CMainBarDialog* pDlg = GAMEIN->GetMainInterfaceDialog();
//	if( pDlg )
//		pDlg->SetPushBarIcon( OPT_MUGONGDLGICON, m_bActive );
}