#ifndef _TITANPARTS_PROGRESSBARDLG_H
#define _TITANPARTS_PROGRESSBARDLG_H

#include "ProgressBarDlg.h"

class CTitanPartsProgressBarDlg : public CProgressBarDlg
{
public:
	CTitanPartsProgressBarDlg();
	~CTitanPartsProgressBarDlg();

	void OnActionEvent(LONG lId, void * p, DWORD we);
	void Linking();	
};

#endif //_TITANPARTS_PROGRESSBARDLG_H