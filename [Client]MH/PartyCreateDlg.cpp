// PartyCreateDlg.cpp: implementation of the CPartyCreateDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartyCreateDlg.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cButton.h"
#include "./Interface/cComboBox.h"
#include "PartyManager.h"
#include "cResourceManager.h"
#include "./Interface/cCheckBox.h"
#include "ChatManager.h"
#include "ObjectManager.h"
#include "cIMEex.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPartyCreateDlg::CPartyCreateDlg()
{
	m_type = WT_PARTYCREATEDLG;

	m_pThemeEdit = NULL;	
	m_pMinLevelEdit = NULL;
	m_pMaxLevelEdit = NULL;
	m_pPublicCheck = NULL;
	m_pPrivateCheck = NULL;
	m_pDistribute = NULL;
	m_pMemberNumCombo = NULL;
	m_pOKBtn = NULL;
	m_pCancelBtn = NULL;
}

CPartyCreateDlg::~CPartyCreateDlg()
{

}

void CPartyCreateDlg::Linking()
{

	m_pThemeEdit = (cEditBox*)GetWindowForID(PA_CREATE_THEME);
	m_pMinLevelEdit = (cEditBox*)GetWindowForID(PA_CREATE_MINLEVEL);
	m_pMaxLevelEdit  = (cEditBox*)GetWindowForID(PA_CREATE_MAXLEVEL);
	m_pPublicCheck = (cCheckBox*)GetWindowForID(PA_CREATE_PUBLIC);
	m_pPrivateCheck = (cCheckBox*)GetWindowForID(PA_CREATE_PRIVATE);
	m_pDistribute =	(cComboBox*)GetWindowForID(PA_CREATE_DIVISION);
	m_pMemberNumCombo = (cComboBox*)GetWindowForID(PA_CREATE_MEMBERNUM);
	m_pOKBtn = (cButton*)GetWindowForID(PA_CREATE_OK);
	m_pCancelBtn = (cButton*)GetWindowForID(PA_CREATE_CANCEL);

	m_pMinLevelEdit->SetValidCheck( VCM_NUMBER );
	m_pMaxLevelEdit->SetValidCheck( VCM_NUMBER );
	
	this->InitOption();

	//m_pDistribute =	(cComboBox*)GetWindowForID(PA_CREATECOMBOX);
	//m_pOKBtn = (cButton*)GetWindowForID(PA_CREATEOK);
	//m_pCancelBtn = (cButton*)GetWindowForID(PA_CREATECANCEL);
}

void CPartyCreateDlg::SetActive( BOOL val )
{
	cDialog::SetActive(val);

	if(val == FALSE)
	{
		this->InitOption();
	}	
}

BOOL CPartyCreateDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{
	switch(lId)
	{	
	case PA_CREATE_PUBLIC:
		{
			m_pPrivateCheck->SetChecked(FALSE);
		}
		break;
	case PA_CREATE_PRIVATE:
		{			
			m_pPublicCheck->SetChecked(FALSE);
		}
		break;
	case PA_CREATE_OK:
		{
			this->CreatePartySyn();

			this->SetActive(FALSE);		
		}
		break;
	case PA_CREATE_CANCEL:
		{
			this->SetActive(FALSE);			
		}
		break;
	}
	return TRUE;
}


DWORD CPartyCreateDlg::ActionKeyboardEvent(CKeyboard * keyInfo)
{
	DWORD we = WE_NULL;
	
	return we;
}

void CPartyCreateDlg::InitOption()
{
	if(m_pThemeEdit != NULL)
		m_pThemeEdit->SetEditText("");
	if(m_pPublicCheck != NULL)
		m_pPublicCheck->SetChecked(TRUE);
	if(m_pPrivateCheck != NULL)
		m_pPrivateCheck->SetChecked(FALSE);
	if(m_pMinLevelEdit != NULL)
		m_pMinLevelEdit->SetEditText("1");
	if(m_pMaxLevelEdit != NULL)
		m_pMaxLevelEdit->SetEditText("99");
}

BOOL CPartyCreateDlg::CreatePartySyn()
{
	PARTY_ADDOPTION AddOption;
	memset(&AddOption, 0, sizeof(PARTY_ADDOPTION));

	//제목 길이 검사
	int nStrSize = strlen(m_pThemeEdit->GetEditText());
	if(nStrSize > MAX_PARTY_NAME)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1742) );
		return FALSE;
	}	
	strcpy(AddOption.szTheme, m_pThemeEdit->GetEditText());			
    
	//최소레벨 설정	
	AddOption.wMinLevel = atoi(m_pMinLevelEdit->GetEditText());

	//최대레벨 설정
	AddOption.wMaxLevel = atoi(m_pMaxLevelEdit->GetEditText());

	//공개 여부 설정
	if(m_pPublicCheck->IsChecked() == TRUE)
		AddOption.bPublic = TRUE;
	else
		AddOption.bPublic = FALSE;

	//분배 방식 설정
	char buff[16];
	strcpy(buff,m_pDistribute->GetComboText());

	CMD_ST(buff)
		CMD_CS( RESRCMGR->GetMsg(483) )
		AddOption.bOption = ePartyOpt_Random;
	CMD_CS( RESRCMGR->GetMsg(484) )
		AddOption.bOption = ePartyOpt_Damage;
	CMD_EN

	//인원설정
	AddOption.wLimitCount = atoi(m_pMemberNumCombo->GetComboText());

	if(HERO->GetPartyIdx() != 0)
	{
		ASSERT(0);
		return FALSE;
	}

	AddOption.Category = MP_PARTY;
	AddOption.Protocol = MP_PARTY_CREATE_SYN;
	AddOption.dwObjectID = HEROID;	
	NETWORK->Send(&AddOption,sizeof(AddOption));

	PARTYMGR->SetIsProcessing(TRUE);

	return TRUE;
}
