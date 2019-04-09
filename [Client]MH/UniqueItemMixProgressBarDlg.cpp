#include "StdAfx.h"
#include "UniqueItemMixProgressBarDlg.h"
#include "WindowIDEnum.h"
#include "GameIn.h"
#include "UniqueItemMixDlg.h"

CUniqueItemMixProgressBarDlg::CUniqueItemMixProgressBarDlg()
{

}

CUniqueItemMixProgressBarDlg::~CUniqueItemMixProgressBarDlg()
{

}

void CUniqueItemMixProgressBarDlg::Linking()
{
	m_pProgressGuagen = (CObjectGuagen*)GetWindowForID(UNIQUEITEMMIX_PROGRESS_GAGE);
	m_pRemaintimeStatic = (cStatic*)GetWindowForID(UNIQUEITEMMIX_PROGRESSBAR_TIME);
}

void CUniqueItemMixProgressBarDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{	
	switch(lId)
	{
	case UNIQUEITEMMIX_PROGRESSBAR_CANCEL:
		{
			InitProgress();			
			GAMEIN->GetUniqueItemMixDlg()->SetDisable(FALSE);			
		}
		break;
	}
}