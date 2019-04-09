// MNJoinDialog.h: interface for the CMNJoinDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MNJOINDIALOG_H__43D8EB80_DC3B_4B4F_A725_6447956B2BB4__INCLUDED_)
#define AFX_MNJOINDIALOG_H__43D8EB80_DC3B_4B4F_A725_6447956B2BB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./interface/cDialog.h"

class CMNJoinDialog : public cDialog
{
public:
	CMNJoinDialog();
	virtual ~CMNJoinDialog();

	void Linking();
	void OnActionEvent(LONG lId, void * p, DWORD we);
};

#endif // !defined(AFX_MNJOINDIALOG_H__43D8EB80_DC3B_4B4F_A725_6447956B2BB4__INCLUDED_)
