// MNFrontDialog.h: interface for the CMNFrontDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MNFRONTDIALOG_H__D00BB1E3_30CB_4489_983F_8F470E53C26E__INCLUDED_)
#define AFX_MNFRONTDIALOG_H__D00BB1E3_30CB_4489_983F_8F470E53C26E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./interface/cDialog.h"

class CMNFrontDialog : public cDialog
{
protected:

public:
	CMNFrontDialog();
	virtual ~CMNFrontDialog();

	void Linking();
	void OnActionEvent(LONG lId, void * p, DWORD we);
};

#endif // !defined(AFX_MNFRONTDIALOG_H__D00BB1E3_30CB_4489_983F_8F470E53C26E__INCLUDED_)
