// TitanMugongMixDlg.h: interface for the CTitanMugongMixDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(TITANMUGONGMIXDLG_H)
#define TITANMUGONGMIXDLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"
#include "./Interface/cIconDialog.h"

class CTitanMugongMixDlg : public cDialog
{
	cIconDialog * m_pTitanMugongIconDlg;
	cIconDialog * m_pTitanItemIconDlg;

public:
	CTitanMugongMixDlg();
	virtual ~CTitanMugongMixDlg();

	void Linking();
	virtual void SetActive( BOOL val );
	BOOL FakeMoveIcon(LONG mouseX, LONG mouseY, cIcon * icon);
	void OnActionEvent(LONG lId, void * p, DWORD we);
	void DeleteMugongIcon();
	void DeleteItemIcon();
};

#endif // !defined(TITANMUGONGMIXDLG_H)