#ifndef _TITANMIX_PROGRESSBARDLG_H
#define _TITANMIX_PROGRESSBARDLG_H

#include "ProgressBarDlg.h"

class CTitanMixProgressBarDlg : public CProgressBarDlg
{
public:
	CTitanMixProgressBarDlg();
	~CTitanMixProgressBarDlg();

	void OnActionEvent(LONG lId, void * p, DWORD we);
	void Linking();

	void SuccessProcess();
};

#endif //_TITANMIX_PROGRESSBARDLG_H