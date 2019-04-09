#include "StdAfx.h"
#include "TitanMixProgressBarDlg.h"
#include "WindowIDEnum.h"
#include "GameIn.h"
#include "TitanMixDlg.h"

CTitanMixProgressBarDlg::CTitanMixProgressBarDlg()
{

}

CTitanMixProgressBarDlg::~CTitanMixProgressBarDlg()
{

}

void CTitanMixProgressBarDlg::Linking()
{
	m_pProgressGuagen = (CObjectGuagen*)GetWindowForID(TITANMIX_PROGRESSBAR_GAGE);
	m_pRemaintimeStatic = (cStatic*)GetWindowForID(TITANMIX_PROGRESSBAR_TIME);
}

void CTitanMixProgressBarDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{
	switch(lId)
	{
	case TITANMIX_PROGRESSBAR_CANCEL:
		{
			InitProgress();			
			GAMEIN->GetTitanMixDlg()->SetDisable(FALSE);			
		}
		break;
	}
}

void CTitanMixProgressBarDlg::SuccessProcess()
{
	
}