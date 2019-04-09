// TitanDissolutionDlg.h: interface for the CTitanDissolutionDlg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _TITANDISSOLUTIONDLG_H
#define _TITANDISSOLUTIONDLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./interface/cDialog.h"
#include "VirtualItem.h"

class CItem;
class cStatic;
class cIconDialog;

class CTitanDissolutionDlg : public cDialog
{
	cIconDialog * m_pTitanDissolutionDlg;
	CVirtualItem m_VirtualItem;

public:

	CTitanDissolutionDlg();
	virtual ~CTitanDissolutionDlg();
	void Release(BOOL bFlag = TRUE);
	void Linking();
	virtual void SetActive( BOOL val );
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);
	void AddTitanPaper(CItem* pItem);
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	static BOOL OnActionEvent(LONG lId, void * p, DWORD we);
	void DieCheck();
	static void DissolutionOkBtn(CTitanDissolutionDlg * pThis);
};

#endif // _TITANDISSOLUTIONDLG_H