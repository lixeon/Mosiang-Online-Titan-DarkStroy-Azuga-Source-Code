// ReviveDialog.cpp: implementation of the CReviveDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ReviveDialog.h"
#include "WindowIDEnum.h"
#include "MHMap.h"
#include "SiegeWarMgr.h"
#include "interface/cButton.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CReviveDialog::CReviveDialog()
{

}

CReviveDialog::~CReviveDialog()
{

}

void CReviveDialog::Linking()
{
	m_pPresentBtn = (cButton*)GetWindowForID(CR_PRESENTSPOT);
	m_pLoginBtn = (cButton*)GetWindowForID(CR_LOGINSPOT);
	m_pVillageBtn = (cButton*)GetWindowForID(CR_TOWNSPOT);
}

void CReviveDialog::SetActive( BOOL val )
{
	cDialog::SetActive( val );

	if( SIEGEMGR->GetSiegeWarMapNum() && MAP->GetMapNum() == SIEGEMGR->GetSiegeWarMapNum() )
	{
		m_pPresentBtn->SetActive( FALSE );
		m_pVillageBtn->SetActive( TRUE );
	}
	else
	{
		m_pPresentBtn->SetActive( TRUE );
		m_pVillageBtn->SetActive( FALSE );
	}
}
