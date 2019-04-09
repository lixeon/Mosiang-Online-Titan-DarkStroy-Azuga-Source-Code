#ifndef _UNIQUEITEMMIX_PROGRESSBARDLG_H
#define _UNIQUEITEMMIX_PROGRESSBARDLG_H

#include "ProgressBarDlg.h"

class CUniqueItemMixProgressBarDlg : public CProgressBarDlg
{
public:
	CUniqueItemMixProgressBarDlg();
	~CUniqueItemMixProgressBarDlg();

	void OnActionEvent(LONG lId, void * p, DWORD we);
	void Linking();
};

#endif //_UNIQUEITEMMIX_PROGRESSBARDLG_H