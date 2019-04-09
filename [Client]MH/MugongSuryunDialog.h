// MugongSuryunDialog.h: interface for the CMugongSuryunDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MUGONGSURYUNDIALOG_H__F48D780C_08B2_458B_8E28_709D61AC47D3__INCLUDED_)
#define AFX_MUGONGSURYUNDIALOG_H__F48D780C_08B2_458B_8E28_709D61AC47D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cTabDialog.h"

class CMugongDialog;
class CSuryunDialog;

class CMugongSuryunDialog : public cTabDialog 
{
	CMugongDialog* m_pMugongDlg;
	CSuryunDialog* m_pSuryunDlg;

public:
	CMugongSuryunDialog();
	virtual ~CMugongSuryunDialog();

	virtual void Add(cWindow * window);
	virtual void SetActive( BOOL val );
	virtual void OnActionEvent(LONG lId, void * p, DWORD we);
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);

	CSuryunDialog* GetSuryunDialog() { return m_pSuryunDlg; }
	CMugongDialog* GetMugongDialog() { return m_pMugongDlg; }	
};

#endif // !defined(AFX_MUGONGSURYUNDIALOG_H__F48D780C_08B2_458B_8E28_709D61AC47D3__INCLUDED_)
