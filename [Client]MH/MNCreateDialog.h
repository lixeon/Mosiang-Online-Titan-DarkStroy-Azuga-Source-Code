// MNCreateDialog.h: interface for the CMNCreateDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MNCREATEDIALOG_H__AB2192B1_11BD_4C3A_9233_3E2AB0660707__INCLUDED_)
#define AFX_MNCREATEDIALOG_H__AB2192B1_11BD_4C3A_9233_3E2AB0660707__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./interface/cDialog.h"

class CMNCreateDialog : public cDialog
{
public:
	CMNCreateDialog();
	virtual ~CMNCreateDialog();

	void Linking();
	void OnActionEvent(LONG lId, void * p, DWORD we);
};

#endif // !defined(AFX_MNCREATEDIALOG_H__AB2192B1_11BD_4C3A_9233_3E2AB0660707__INCLUDED_)
