// GuageDialog.cpp: implementation of the CGuageDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuageDialog.h"
#include "ObjectGuagen.h"
#include "WindowIDEnum.h"
#include "./Interface/cButton.h"
#include "./interface/cStatic.h"
#include "MHTimeManager.h"
#include "MussangManager.h"
#include "ObjectManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuageDialog::CGuageDialog()
{
	m_pMussangBtn	= NULL;
	m_pFlicker01	= NULL;
	m_pFlicker02	= NULL;

	m_bFlicker		= FALSE;
	m_bFlActive		= FALSE;
	m_dwFlickerSwapTime	= 0;
}

CGuageDialog::~CGuageDialog()
{

}

void CGuageDialog::Linking()
{
	m_pMussangBtn	= (cButton*)GetWindowForID(CG_MUSSANGSTART);
	m_pFlicker01	= (cStatic*)GetWindowForID(CG_MUSSANG1);
//	m_pFlicker02	= (cStatic*)GetWindowForID(CG_MUSSANG2);

	if( m_pMussangBtn )
	{
		DisableMussangBtn(TRUE);
	}
	((CObjectGuagen*)GetWindowForID(CG_GUAGEMUSSANG))->SetValue( 0, 0 );
	m_pFlicker01->SetActive(FALSE);
//	m_pFlicker02->SetActive(FALSE);
}

void CGuageDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if( we & WE_BTNCLICK )
	{
		if( lId == CG_MUSSANGSTART )
		{
			//무쌍 발동 정보 보내기	//SW070912 기획변경
			if(!HERO->IsDied() && !HERO->InTitan())
				MUSSANGMGR->SendMsgMussangOn();
		}
	}
}

void CGuageDialog::DisableMussangBtn(BOOL bDisable)
{
	DWORD FullColor = 0xffffffff;
	DWORD HalfColor = RGBA_MAKE(200,200,200,255);
	
	if( bDisable )
	{
		FullColor = HalfColor;
	}

	m_pMussangBtn->SetDisable(bDisable);
	m_pMussangBtn->SetImageRGB(FullColor);
}

void CGuageDialog::SetFlicker( BOOL bFlicker )
{
	m_bFlicker = bFlicker;
	m_dwFlickerSwapTime = gCurTime;

	m_pFlicker01->SetActive(bFlicker);
//	m_pFlicker02->SetActive(bFlicker);
}

void CGuageDialog::FlickerMussangGuage()
{
	if(!m_bFlicker)	return;

	if( gCurTime - m_dwFlickerSwapTime > FLICKER_TIME )
	{
		m_bFlActive = !m_bFlActive;

		m_pFlicker01->SetActive(m_bFlActive);
//		m_pFlicker02->SetActive(!m_bFlActive);
		
		m_dwFlickerSwapTime = gCurTime;
	}
}

void CGuageDialog::Render()
{
	FlickerMussangGuage();
	cDialog::Render();
}
