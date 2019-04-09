// IntroReplayDlg.h: interface for the CIntroReplayDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_INTROREPLAYDLG_H)
#define _INTROREPLAYDLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./interface/cDialog.h"

class CIntroReplayDlg : public cDialog  
{

public:
	CIntroReplayDlg();
	virtual ~CIntroReplayDlg();

	void	Linking();
};

#endif // !defined(_INTROREPLAYDLG_H)
