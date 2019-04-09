// PartyDialog.cpp: implementation of the CPartyDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartyDialog.h"
#include "PartyMemberDlg.h"
#include "PartyBtnDlg.h"

#include "PartyManager.h"
#include "ObjectManager.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"

#include "GameIn.h"
#include "MainBarDialog.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPartyDialog::CPartyDialog() 
{
	Init();
}

CPartyDialog::~CPartyDialog()
{

}

void CPartyDialog::Init()
{
	m_bActive = FALSE;
	m_ClickedMemberID = 0;
	m_bShowMember = TRUE;
	m_bOption = TRUE;
}

void CPartyDialog::RegistMemberDlg(int i, CPartyMemberDlg* pDlg)
{
	m_MemberDlg[i] = pDlg;
	m_MemberDlg[i]->Linking(i);
//	m_MemberDlg[i]->SetPartyBtnDlg( m_PartyBtnDlg );
}

void CPartyDialog::RegistBtnDlg(CPartyBtnDlg* pDlg)
{
	m_PartyBtnDlg = pDlg;
	m_PartyBtnDlg->Linking();

	for( int i = 0; i <MAX_PARTY_LISTNUM-1; ++i )
		m_MemberDlg[i]->SetPartyBtnDlg( m_PartyBtnDlg );
}

BOOL CPartyDialog::IsActive()
{
	return m_bActive;
}

void CPartyDialog::SetActive(BOOL bState)
{
	m_bActive = bState;

	if(bState == TRUE)
	{
		if(PARTYMGR->CanActivate() == FALSE)
		{
			m_bActive = FALSE;
		}
	}
	for(int j=0; j<MAX_PARTY_LISTNUM-1; ++j)
		m_MemberDlg[j]->SetActive(m_bActive);
	m_PartyBtnDlg->SetActive(m_bActive);

//--- main bar icon
//	CMainBarDialog* pDlg = GAMEIN->GetMainInterfaceDialog();
//	if( pDlg )
//		pDlg->SetPushBarIcon( OPT_PARTYDLGICON, m_bActive );
}

void CPartyDialog::RefreshDlg()
{
	if(HEROID == 0)
		return;

	for(int i=0; i<MAX_PARTY_LISTNUM-1; ++i)
		m_MemberDlg[i]->SetMemberData(NULL);

	int membercount=0;
	for(i=0; i<MAX_PARTY_LISTNUM; ++i)
	{
		DWORD PlayerID = PARTYMGR->GetPartyMemberID(i);
		if((PlayerID != 0) && (HEROID != PlayerID))
		{
			ASSERT( membercount < 6 );
			if( membercount < 6 )	//오류가 나서 파티가 꼬이더라도 다운은 안되도록
				m_MemberDlg[membercount++]->SetMemberData(PARTYMGR->GetPartyMemberInfo(i));
		}
	}

	m_PartyBtnDlg->RefreshDlg();
}

/*
BOOL CPartyDialog::IsActive()
{
	if(m_bMinDlg == MIN_PARTYDLG_MODE)
	{
		return m_pPartyMinDlg->IsActive();
	}
	else
	{
		return m_pPartyMaxDlg->IsActive();
	}
}

void CPartyDialog::SetActive(BOOL bbb)
{
	if(bbb == TRUE)
	{
		if(m_bMinDlg == MIN_PARTYDLG_MODE)
		{
			m_pPartyMaxDlg->SetActive(FALSE);
			m_pPartyMinDlg->SetActive(TRUE);
			m_pPartyMinDlg->RefreshDlg();
		}
		else
		{
			m_pPartyMinDlg->SetActive(FALSE);
			m_pPartyMaxDlg->SetActive(TRUE);
			m_pPartyMaxDlg->RefreshDlg();
		}
	}
	else
	{
		m_pPartyMaxDlg->SetActive(FALSE);
		m_pPartyMinDlg->SetActive(FALSE);
	}
}

void CPartyDialog::RefreshDlg()
{
	if(m_bMinDlg == MIN_PARTYDLG_MODE)
	{
		m_pPartyMinDlg->RefreshDlg();
	}
	else
	{
		m_pPartyMaxDlg->RefreshDlg();
	}
}

void CPartyDialog::SetDlgMode(BOOL state)
{
	m_bMinDlg = state;
}

void CPartyDialog::RegistMaxDlg(CPartyMaxDialog* pDlg)
{
	m_pPartyMaxDlg = pDlg;
	m_pPartyMaxDlg->Linking();
}
void CPartyDialog::RegistMinDlg(CPartyMinDialog* pDlg)
{
	m_pPartyMinDlg = pDlg;
	m_pPartyMinDlg->Linking();
}



void CPartyDialog::MemberBtnPushUp(int n)
{
	m_pPartyMaxDlg->MemberBtnPushUp(n);
}

  */

void CPartyDialog::MemberBtnPushUp(DWORD id)
{
	for(int i=0; i<MAX_PARTY_LISTNUM-1; ++i)
	{
		if(m_MemberDlg[i]->m_MemberID ==id)
		{
			m_MemberDlg[i]->SetNameBtnPushUp(TRUE);
			m_ClickedMemberID = id;
		}
		else
			m_MemberDlg[i]->SetNameBtnPushUp(FALSE);
	}
}

void CPartyDialog::SetClickedMemberID(DWORD id)
{
	if( id == 0 )
	{
		for( int i = 0; i <MAX_PARTY_LISTNUM-1; ++i )
			m_MemberDlg[i]->SetNameBtnPushUp( FALSE );
		m_ClickedMemberID = 0;
		return;
	}
	else
	{
		CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( id );
//		if( !pPlayer )
//		{
//			for( int i = 0; i <MAX_PARTY_LISTNUM-1; ++i )
//				m_MemberDlg[i]->SetNameBtnPushUp( FALSE );
//			m_ClickedMemberID = 0;
//		}
//		else
//		{
			MemberBtnPushUp(id);
//		}

		OBJECTMGR->SetSelectedObject( pPlayer );
	}

//	if(m_ClickedMemberID == id)
//	{
//		OBJECTMGR->SetSelectedObject( OBJECTMGR->GetObject( id ) );
//		return;
//	}
	
//	MemberBtnPushUp(id);
}

DWORD CPartyDialog::GetClickedMemberID()
{
	return m_ClickedMemberID;
}

void CPartyDialog::ShowMember()
{
	m_bShowMember = !m_bShowMember;

	for( int i = 0; i < MAX_PARTY_LISTNUM-1; ++i )
	{
		m_MemberDlg[i]->ShowMember( m_bShowMember );
		m_MemberDlg[i]->SetActive( m_bShowMember );
	}
}

void CPartyDialog::ShowOption()
{
	m_bOption = !m_bOption;

	m_PartyBtnDlg->ShowOption( m_bOption );

	for( int i = 0; i < MAX_PARTY_LISTNUM-1; ++i )
		m_MemberDlg[i]->ShowOption( m_bOption );
}

void CPartyDialog::SetOptionActive( BOOL bFlag )
{
	m_bOption = bFlag;
	ShowOption();
}

void CPartyDialog::SetMemberActive( BOOL bFlag )
{
	m_bShowMember = bFlag;
	ShowMember();
}