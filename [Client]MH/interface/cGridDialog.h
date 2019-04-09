// cGridDialog.h: interface for the cGridDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGRIDDIALOG_H__85CBF9B5_FBE7_49B2_90FA_C5EFA968CA8D__INCLUDED_)
#define AFX_CGRIDDIALOG_H__85CBF9B5_FBE7_49B2_90FA_C5EFA968CA8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cDialog.h"

class cPushupButton;

class cGridDialog : public cDialog  
{
public:
	cGridDialog();
	virtual ~cGridDialog();

	virtual void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, cPushupButton * pCellWindow, WORD cellNum, LONG ID=0);
//	virtual void Render(I4DyuchiGXGeometry* pGeometry);
//	virtual DWORD ActionEvent(CMouse * mouseInfo);

	cPushupButton * m_pWindowCell; 
	WORD m_wCellNum;
};

#endif // !defined(AFX_CGRIDDIALOG_H__85CBF9B5_FBE7_49B2_90FA_C5EFA968CA8D__INCLUDED_)
