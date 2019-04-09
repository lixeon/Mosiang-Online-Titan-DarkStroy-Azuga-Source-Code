// PartyMemberDlg.cpp: implementation of the CPartyMemberDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartyMemberDlg.h"
#include "PartyManager.h"
#include "GameIn.h"

#include "WindowIDEnum.h"
#include "./Interface/cPushupButton.h"
#include "./ObjectGuagen.h"
#include "./Interface/cStatic.h"
#include "PartyBtnDlg.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
DWORD PMD_LOGIN_BASICCOLOR  = RGBA_MAKE(255,255,255,255);
DWORD PMD_LOGIN_OVERCOLOR  = RGBA_MAKE(255,255,255,255);
DWORD PMD_LOGIN_PRESSCOLOR  = RGBA_MAKE(255,234,0,255);

DWORD PMD_LOGOUT_BASICCOLOR  = RGBA_MAKE(172,182,199,255);
DWORD PMD_LOGOUT_OVERCOLOR  = RGBA_MAKE(172,182,199,255);
DWORD PMD_LOGOUT_PRESSCOLOR  = RGBA_MAKE(255,234,0,255);

CPartyMemberDlg::CPartyMemberDlg()
{
	m_MemberID = 0;
	m_bRealActive = FALSE;
	
	m_bSetTopOnActive = FALSE;	//KESÃß°¡

	m_pPartyBtnDlg = NULL;
	m_nIndex = -1;
	m_bOption = TRUE;
	m_bMember = TRUE;
}

CPartyMemberDlg::~CPartyMemberDlg()
{

}

void CPartyMemberDlg::SetActive(BOOL val)
{
	if( m_bDisable ) return;
	if( m_bMember == FALSE )
		val = FALSE;	
	m_bRealActive = val;	
	if(m_MemberID == 0)
		val = FALSE;
	cDialog::SetActive(val);
}

void CPartyMemberDlg::Linking(int i)
{
	m_pName = (cPushupButton *)GetWindowForID(PA_MEMBER1NAME+i);
	m_pLife = (CObjectGuagen *)GetWindowForID(PA_GUAGEMEMBER1LIFE+i);
	m_pNaeryuk = (CObjectGuagen *)GetWindowForID(PA_GUAGEMEMBER1NAERYUK+i);
	m_pLevel = (cStatic *)GetWindowForID(PA_MEMBER1LEVEL+i);
	m_nIndex = i;
}


void CPartyMemberDlg::SetMemberData(PARTY_MEMBER* pInfo)
{
	if(pInfo == NULL)
	{
		m_MemberID = 0;
	}
	else
	{
		char buf[10];
		sprintf(buf, "Lv.%d", pInfo->Level);
		m_MemberID = pInfo->dwMemberID;
		if(pInfo->bLogged)
		{
			m_pName->SetText(pInfo->Name, PMD_LOGIN_BASICCOLOR, PMD_LOGIN_OVERCOLOR, PMD_LOGIN_PRESSCOLOR);
			m_pLife->SetValue(pInfo->LifePercent*0.01f, 0);
			m_pNaeryuk->SetValue(pInfo->NaeRyukPercent*0.01f, 0);
			m_pLevel->SetStaticText(buf);
		}

		else
		{
			m_pName->SetText(pInfo->Name, PMD_LOGOUT_BASICCOLOR, PMD_LOGOUT_OVERCOLOR, PMD_LOGOUT_PRESSCOLOR);
			m_pLife->SetValue(0, 0);
			m_pNaeryuk->SetValue(0, 0);
			m_pLevel->SetStaticText("");
		}		
	}

	SetActive(m_bRealActive);
}

DWORD CPartyMemberDlg::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	we = cDialog::ActionEvent(mouseInfo);
	
	if(we & WE_LBTNCLICK)
	{
		GAMEIN->GetPartyDialog()->SetClickedMemberID(m_MemberID);
	}
	return we;
}

void CPartyMemberDlg::SetNameBtnPushUp(BOOL val)
{
	m_pName->SetPush(val);
}

void CPartyMemberDlg::Render()
{
	if( m_pPartyBtnDlg && m_nIndex != -1 )
	{
		float x = m_pPartyBtnDlg->GetAbsX();
		float y = m_pPartyBtnDlg->GetAbsY();

		if( m_bOption )
		{
			this->SetAbsXY( (LONG)x, (LONG)(y+134 + ( 48 * m_nIndex )) );
			m_pName->SetAbsXY( (LONG)(x+9), (LONG)(y+148 + ( 48 * m_nIndex )) );
			m_pLife->SetAbsXY( (LONG)(x+6), (LONG)(y+168 + ( 48 * m_nIndex )) );
			m_pNaeryuk->SetAbsXY( (LONG)(x+6), (LONG)(y+174 + ( 48 * m_nIndex )) );
			m_pLevel->SetAbsXY( (LONG)(x+9), (LONG)(y+135 + ( 48 * m_nIndex )) );
		}
		else
		{
			this->SetAbsXY( (LONG)x, (LONG)(y + 54 + ( 48 * m_nIndex )) );
			m_pName->SetAbsXY( (LONG)(x+9), (LONG)(y + 68 + ( 48 * m_nIndex )) );
			m_pLife->SetAbsXY( (LONG)(x+6), (LONG)(y + 88 + ( 48 * m_nIndex )) );
			m_pNaeryuk->SetAbsXY( (LONG)(x+6), (LONG)(y + 94 + ( 48 * m_nIndex )) );
			m_pLevel->SetAbsXY( (LONG)(x+9), (LONG)(y + 55 + ( 48 * m_nIndex )) );
		}
	}

	cDialog::Render();
}