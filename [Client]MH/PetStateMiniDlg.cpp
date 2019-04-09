#include "stdafx.h"
#include ".\petstateminidlg.h"
#include "./interface/cGuagen.h"
#include "./interface/cStatic.h"
#include "./interface/cButton.h"
#include "WindowIDEnum.h"
#include "Pet.h"
#include "PetManager.h"

CPetStateMiniDlg::CPetStateMiniDlg(void)
{
	m_pNameDlg			= NULL;
	m_pStateDlg			= NULL;
	m_pFriend			= NULL;
	m_pStamina			= NULL;

	m_pFriendGuage		= NULL;
	m_pStaminaGuage		= NULL;

	m_pPetUseRestBtn	= NULL;
	m_pPetInvenBtn		= NULL;

	m_pDlgToggleBtn		= NULL;
}

CPetStateMiniDlg::~CPetStateMiniDlg(void)
{
}

void CPetStateMiniDlg::Linking()
{
	m_pNameDlg		= (cStatic*)GetWindowForID(PSMN_NAME);
	m_pStateDlg		= (cStatic*)GetWindowForID(PSMN_STATE);
	m_pFriend		= (cStatic*)GetWindowForID(PSMN_GUAGETEXT1);
	m_pStamina		= (cStatic*)GetWindowForID(PSMN_GUAGETEXT2);

	m_pFriendGuage	= (cGuagen*)GetWindowForID(PSMN_GUAGE1);
	m_pStaminaGuage = (cGuagen*)GetWindowForID(PSMN_GUAGE2);

	m_pPetUseRestBtn	= (cButton*)GetWindowForID(PSMN_USEREST_BTN);
	m_pPetInvenBtn	= (cButton*)GetWindowForID(PSMN_INVEN_BTN);

	m_pDlgToggleBtn = (cButton*)GetWindowForID(PSMN_TOGGLE_B_BTN);
}

void CPetStateMiniDlg::OnActionEvent(LONG lId, void *p, DWORD we)
{
	if( we & WE_BTNCLICK )
	{
		if( PSMN_TOGGLE_B_BTN == lId )
		{
			PETMGR->TogglePetStateDlg();
		}
		else if( PSMN_USEREST_BTN == lId )
		{
			if(NULL == PETMGR->GetCurSummonPet())	return;
			PETMGR->SendPetRestMsg(!PETMGR->GetCurSummonPet()->IsPetRest());
		}
		else if( PSMN_INVEN_BTN == lId )
		{
			PETMGR->OpenPetInvenDlg();
		}
	}		
}

