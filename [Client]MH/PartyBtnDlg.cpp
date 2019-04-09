// PartyBtnDlg.cpp: implementation of the CPartyBtnDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartyBtnDlg.h"
#include "WindowIDEnum.h"
#include "./Interface/cButton.h"
#include "./Interface/cStatic.h"
#include "PartyManager.h"
#include "ObjectManager.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPartyBtnDlg::CPartyBtnDlg()
{
	m_bSetTopOnActive = FALSE;	//KESÃß°¡;
	m_bOption = TRUE;
}

CPartyBtnDlg::~CPartyBtnDlg()
{
}

void CPartyBtnDlg::Linking()
{
	m_pBackGround = (cStatic*)GetWindowForID(PA_BACKGROUND);
	m_pSecedeBtn = (cButton*)GetWindowForID(PA_SECEDEBTN);
	m_pTransferBtn = (cButton*)GetWindowForID(PA_TRANSFERBTN);
	m_pForcedSecedeBtn = (cButton*)GetWindowForID(PA_FORCEDSECEDEBTN);
	m_pAddMemberBtn = (cButton*)GetWindowForID(PA_ADDPARTYMEMBERBTN);
	m_pBreakUpBtn = (cButton*)GetWindowForID(PA_BREAKUPBTN);
	m_pWarSuggestBtn = (cButton*)GetWindowForID(PA_WARSUGGESTBTN);
	m_pOptionBtn = (cButton*)GetWindowForID(PA_OPTIONBTN);
	m_pMemberBtn = (cButton*)GetWindowForID(PA_MEMBERBTN);
}

void CPartyBtnDlg::RefreshDlg()
{
	if(HERO->GetPartyIdx() == 0)
		ShowNonPartyDlg();
	else if(PARTYMGR->GetMasterID() == HEROID)
	{
		ShowPartyMasterDlg();
	}
	else
	{
		ShowPartyMemberDlg();
	}
}

void CPartyBtnDlg::ShowNonPartyDlg()
{
	m_pSecedeBtn->SetActive(FALSE);
	m_pTransferBtn->SetActive(FALSE);
	m_pForcedSecedeBtn->SetActive(FALSE);
	m_pAddMemberBtn->SetActive(FALSE);
	m_pBreakUpBtn->SetActive(FALSE);
	m_pWarSuggestBtn->SetActive(FALSE);
	m_pOptionBtn->SetActive(FALSE);
	m_pMemberBtn->SetActive(FALSE);
}

void CPartyBtnDlg::ShowPartyMasterDlg()
{
/*	m_pSecedeBtn->SetActive(TRUE);
	m_pTransferBtn->SetActive(TRUE);
	m_pForcedSecedeBtn->SetActive(TRUE);
	m_pAddMemberBtn->SetActive(TRUE);
	m_pBreakUpBtn->SetActive(TRUE);
	m_pWarSuggestBtn->SetActive(TRUE);
	m_pOptionBtn->SetActive(TRUE);
	m_pMemberBtn->SetActive(TRUE);
*/
	m_pBackGround->SetActive(m_bOption);
	m_pSecedeBtn->SetActive(m_bOption);
	m_pTransferBtn->SetActive(m_bOption);
	m_pForcedSecedeBtn->SetActive(m_bOption);
	m_pAddMemberBtn->SetActive(m_bOption);
	m_pBreakUpBtn->SetActive(m_bOption);
	m_pWarSuggestBtn->SetActive(m_bOption);
	m_pOptionBtn->SetActive(TRUE);
	m_pMemberBtn->SetActive(TRUE);

	m_pSecedeBtn->SetImageRGB( ICONCLR_DISABLE );
	m_pTransferBtn->SetImageRGB( ICONCLR_USABLE );
	m_pForcedSecedeBtn->SetImageRGB( ICONCLR_USABLE );
	m_pAddMemberBtn->SetImageRGB( ICONCLR_USABLE );
	m_pBreakUpBtn->SetImageRGB( ICONCLR_USABLE );
	m_pWarSuggestBtn->SetImageRGB( ICONCLR_USABLE );
}

void CPartyBtnDlg::ShowPartyMemberDlg()
{
/*	m_pSecedeBtn->SetActive(TRUE);
	m_pTransferBtn->SetActive(TRUE);
	m_pForcedSecedeBtn->SetActive(TRUE);
	m_pAddMemberBtn->SetActive(TRUE);
	m_pBreakUpBtn->SetActive(TRUE);
	m_pWarSuggestBtn->SetActive(TRUE);
	m_pOptionBtn->SetActive(TRUE);
	m_pMemberBtn->SetActive(TRUE);
*/
	m_pBackGround->SetActive(m_bOption);
	m_pSecedeBtn->SetActive(m_bOption);
	m_pTransferBtn->SetActive(m_bOption);
	m_pForcedSecedeBtn->SetActive(m_bOption);
	m_pAddMemberBtn->SetActive(m_bOption);
	m_pBreakUpBtn->SetActive(m_bOption);
	m_pWarSuggestBtn->SetActive(m_bOption);
	m_pOptionBtn->SetActive(TRUE);
	m_pMemberBtn->SetActive(TRUE);
	
	m_pSecedeBtn->SetImageRGB( ICONCLR_USABLE );
	m_pTransferBtn->SetImageRGB( ICONCLR_DISABLE );
	m_pForcedSecedeBtn->SetImageRGB( ICONCLR_DISABLE );
	m_pAddMemberBtn->SetImageRGB( ICONCLR_DISABLE );
	m_pBreakUpBtn->SetImageRGB( ICONCLR_DISABLE );
	m_pWarSuggestBtn->SetImageRGB( ICONCLR_DISABLE );
}

void CPartyBtnDlg::ShowOption( BOOL bOption )
{
	m_bOption = bOption;

	m_pBackGround->SetActive(m_bOption);
	m_pSecedeBtn->SetActive(m_bOption);
	m_pTransferBtn->SetActive(m_bOption);
	m_pForcedSecedeBtn->SetActive(m_bOption);
	m_pAddMemberBtn->SetActive(m_bOption);
	m_pBreakUpBtn->SetActive(m_bOption);
	m_pWarSuggestBtn->SetActive(m_bOption);
	m_pOptionBtn->SetActive(TRUE);
	m_pMemberBtn->SetActive(TRUE);

//	if( m_bOption )
//	{
//		m_pMemberBtn->SetAbsY( m_pOptionBtn->GetAbsY() + 100 );
//	}
//	else
//	{
//		m_pMemberBtn->SetAbsY( m_pOptionBtn->GetAbsY() + 20 );
//	}
}

void CPartyBtnDlg::Render()
{
	if( m_bOption )
	{
		m_pMemberBtn->SetAbsY( (LONG)(m_pOptionBtn->GetAbsY() + 100) );
	}
	else
	{
		m_pMemberBtn->SetAbsY( (LONG)(m_pOptionBtn->GetAbsY() + 20) );
	}

	cDialog::Render();
}