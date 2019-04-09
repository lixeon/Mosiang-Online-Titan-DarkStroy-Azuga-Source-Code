// TitanRegisterDlg.h: interface for the CTitanRegisterDlg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _TITANREGISTERDLG_H
#define _TITANREGISTERDLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./interface/cDialog.h"
#include "VirtualItem.h"

class CItem;
class cStatic;
class cIconDialog;

class CTitanRegisterDlg : public cDialog
{
	cIconDialog * m_pTitanRegisterDlg;
	CVirtualItem m_VirtualItem;

public:

	CTitanRegisterDlg();
	virtual ~CTitanRegisterDlg();
	void Release(BOOL bFlag = TRUE);
	void Linking();
	virtual void SetActive( BOOL val );
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);
	void AddTitanPaper(CItem* pItem);
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	static BOOL OnActionEvent(LONG lId, void * p, DWORD we);
	static void RegisterOkBtn(CTitanRegisterDlg * pThis);
	void DieCheck();
};

#endif // _TITANREGISTERDLG_H