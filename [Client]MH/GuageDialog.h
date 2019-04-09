// GuageDialog.h: interface for the CGuageDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUAGEDIALOG_H__78CD1521_0A6D_4ABB_A485_8E21C8B23184__INCLUDED_)
#define AFX_GUAGEDIALOG_H__78CD1521_0A6D_4ABB_A485_8E21C8B23184__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"

#define FLICKER_TIME	100

class cButton;
class cStatic;

class CGuageDialog : public cDialog
{
	cButton* m_pMussangBtn;
	cStatic* m_pFlicker01;
	cStatic* m_pFlicker02;

	BOOL	m_bFlicker;
	BOOL	m_bFlActive;
	DWORD	m_dwFlickerSwapTime;

public:
	CGuageDialog();
	virtual ~CGuageDialog();

	void Linking();
	void OnActionEvent(LONG lId, void * p, DWORD we);
	virtual void Render();

	void DisableMussangBtn(BOOL bDisable);
	void SetFlicker(BOOL bFlicker);
	void FlickerMussangGuage();
};

#endif // !defined(AFX_GUAGEDIALOG_H__78CD1521_0A6D_4ABB_A485_8E21C8B23184__INCLUDED_)
