// AlertDlg.h: interface for the CAlertDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALERTDLG_H__FC8F717E_760F_4232_BE4D_B9A09BDB7881__INCLUDED_)
#define AFX_ALERTDLG_H__FC8F717E_760F_4232_BE4D_B9A09BDB7881__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"

enum { AB_OKCANCEL, AB_YESNO };

class CAlertDlg : public cDialog  
{
public:
	CAlertDlg();
	virtual ~CAlertDlg();
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	virtual void Add(cWindow * window);
	void SetcbBtn(void (*cbFunc)(LONG lId, void * p, DWORD we)) { cbBtnFunc = cbFunc; }
	void SetObj(void * obj) { m_obj = obj; }
	void * GetObj(){ return m_obj; }
	cButton * m_pOk;
	cButton * m_pCancel;
	void (*cbBtnFunc)(LONG lId, void * p, DWORD we);
	void * m_obj;
};

#endif // !defined(AFX_ALERTDLG_H__FC8F717E_760F_4232_BE4D_B9A09BDB7881__INCLUDED_)
