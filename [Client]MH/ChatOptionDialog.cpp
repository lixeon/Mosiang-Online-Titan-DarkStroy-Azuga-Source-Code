
#include "stdafx.h"
/*
#include "ChatOptionDialog.h"
#include "ChatManager.h"
#include "WindowIDEnum.h"
#include "./Interface/cCheckBox.h"


void CChatOptionDialog::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID)
{
	cDialog::Init(x,y,wid,hei,basicImage,ID);
	m_bFirst	= TRUE;
	m_bChanged	= FALSE;
}



void CChatOptionDialog::SetActive(BOOL val)
{
	if( m_bDisable ) return;

	cDialog::SetActive(val);

	if( val )
	{
		sChatOption* pOption = CHATMGR->GetOption();

		for( int i = 0 ; i < CTO_COUNT ; ++i )
			m_pBtnOption[i]->SetChecked( pOption->bOption[i] );

		m_bFirst	= FALSE;
		m_bChanged	= FALSE;
	}
	else if( m_bFirst == FALSE )	//ÃÊ±âÈ­ Á¦¿ÜÇÏ°í ²°À»¶§ ÀúÀå...
	{
		if( m_bChanged )
			CHATMGR->SaveUserOption();
	}
}


void CChatOptionDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	sChatOption* pOption = CHATMGR->GetOption();

	if( we & WE_CHECKED )
	{
		pOption->bOption[lId - COI_CB_OPTION01] = TRUE;
	}
	else if( we & WE_NOTCHECKED )
	{
		pOption->bOption[lId - COI_CB_OPTION01] = FALSE;
	}
	
	m_bChanged = TRUE;
}


void CChatOptionDialog::Linking()
{
	m_pBtnOption[0] = (cCheckBox*)GetWindowForID(COI_CB_OPTION01);
	m_pBtnOption[1] = (cCheckBox*)GetWindowForID(COI_CB_OPTION02);
	m_pBtnOption[2] = (cCheckBox*)GetWindowForID(COI_CB_OPTION03);
	m_pBtnOption[3] = (cCheckBox*)GetWindowForID(COI_CB_OPTION04);
	m_pBtnOption[4] = (cCheckBox*)GetWindowForID(COI_CB_OPTION05);
	m_pBtnOption[5] = (cCheckBox*)GetWindowForID(COI_CB_OPTION06);
	m_pBtnOption[6] = (cCheckBox*)GetWindowForID(COI_CB_OPTION07);
	m_pBtnOption[7] = (cCheckBox*)GetWindowForID(COI_CB_OPTION08);
	m_pBtnOption[8] = (cCheckBox*)GetWindowForID(COI_CB_OPTION09);
	m_pBtnOption[9] = (cCheckBox*)GetWindowForID(COI_CB_OPTION10);
	m_pBtnOption[10] = (cCheckBox*)GetWindowForID(COI_CB_OPTION11);
	m_pBtnOption[11] = (cCheckBox*)GetWindowForID(COI_CB_OPTION12);
}
*/