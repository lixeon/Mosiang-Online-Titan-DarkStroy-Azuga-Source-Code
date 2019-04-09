// TitanGuageDlg.cpp: implementation of the CTitanGuageDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TitanGuageDlg.h"
#include "WindowIDEnum.h"
#include "ObjectGuagen.h"
#include "GameIn.h"
#include "TitanInventoryDlg.h"
#include "TitanManager.h"
#include "ItemManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTitanGuageDlg::CTitanGuageDlg()
{

}

CTitanGuageDlg::~CTitanGuageDlg()
{

}

void CTitanGuageDlg::Linking()
{
	//m_TitanGuage[eTITAN_GuageHp] = (CObjectGuagen*)GetWindowForID(TITAN_GUAGE_CHARLIFE);
	//m_TitanGuage[eTITAN_GuageMp] = (CObjectGuagen*)GetWindowForID(TITAN_GUAGE_CHARNAERYUK);
	m_TitanGuage = (CObjectGuagen*)GetWindowForID(TITAN_GUAGE_CHARLIFE);
	m_pHpPercent = (cStatic*)GetWindowForID(TITAN_GUAGE_HPTEXT);
	//m_pMpPercent = (cStatic*)GetWindowForID(TITAN_GUAGE_mptext);
}

void CTitanGuageDlg::SetActive( BOOL val )
{
	cDialog::SetActive( val );

	if( !val )
	{
		if(GAMEIN->GetTitanInventoryDlg()->IsActive() == TRUE)
		{
			GAMEIN->GetTitanInventoryDlg()->SetActive(FALSE);
		}
	}
}

BOOL CTitanGuageDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{
	switch( lId )
	{
	case TITAN_GUAGE_LOOKBTN:
		{
			if(GAMEIN->GetTitanInventoryDlg()->IsActive())
                GAMEIN->GetTitanInventoryDlg()->SetActive( FALSE );
			else
				GAMEIN->GetTitanInventoryDlg()->SetActive( TRUE );

		}
		break;
	}

	return TRUE;
}

void CTitanGuageDlg::SetLife(DWORD dwLife)
{
	titan_calc_stats* pTitanStatsInfo = TITANMGR->GetTitanStats();

	GUAGEVAL Life = 0.0f;
	Life = (GUAGEVAL)dwLife / (GUAGEVAL)(pTitanStatsInfo->MaxFuel);
	//m_TitanGuage[eTITAN_GuageHp]->SetValue(Life,0);
	m_TitanGuage->SetValue(Life,0);

	char buf[64] = { 0, };
	wsprintf(buf, " : %u/%u", dwLife, pTitanStatsInfo->MaxFuel);
	m_pHpPercent->SetStaticText(buf);
}

void CTitanGuageDlg::SetNaeRyuk(DWORD dwNaeRyuk)
{
	//titan_calc_stats* pTitanStatsInfo = TITANMGR->GetTitanStats();

	//GUAGEVAL NaeRyuk = 0.0f;
	//NaeRyuk = (GUAGEVAL)dwNaeRyuk / (GUAGEVAL)(pTitanStatsInfo->MaxSpell);
	//m_TitanGuage[eTITAN_GuageMp]->SetValue(NaeRyuk,0);

	//char buf[64] = { 0, };
	//wsprintf(buf, " : %u/%u", dwNaeRyuk, pTitanStatsInfo->MaxSpell);
	//m_pMpPercent->SetStaticText(buf);
}