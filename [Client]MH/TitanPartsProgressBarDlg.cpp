#include "StdAfx.h"
#include "TitanPartsProgressBarDlg.h"
#include "WindowIDEnum.h"
#include "GameIn.h"
#include "TitanPartsMakeDlg.h"

CTitanPartsProgressBarDlg::CTitanPartsProgressBarDlg()
{

}

CTitanPartsProgressBarDlg::~CTitanPartsProgressBarDlg()
{

}

void CTitanPartsProgressBarDlg::Linking()
{
	m_pProgressGuagen = (CObjectGuagen*)GetWindowForID(TITANPARTS_PROGRESSBAR_GAGE);
	m_pRemaintimeStatic = (cStatic*)GetWindowForID(TITANPARTS_PROGRESSBAR_TIME);
}

void CTitanPartsProgressBarDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{
	switch(lId)
	{
	case TITANPARTS_PROGRESSBAR_CANCEL:
		{
			InitProgress();			
			GAMEIN->GetTitanPartsMakeDlg()->SetDisable(FALSE);			
		}
		break;
	}
}