// MallNoticeDialog.h: interface for the CMallNoticeDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MALLNOTICEDIALOG_H__1ED31059_188E_416F_A390_B1FF5EDBCC0B__INCLUDED_)
#define AFX_MALLNOTICEDIALOG_H__1ED31059_188E_416F_A390_B1FF5EDBCC0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cTabDialog.h"

class CMallNoticeDialog : public cTabDialog
{
public:
	CMallNoticeDialog();
	virtual ~CMallNoticeDialog();

	virtual void Add(cWindow * window);
	void OnActionEvent(LONG lId, void * p, DWORD we);

};

#endif // !defined(AFX_MALLNOTICEDIALOG_H__1ED31059_188E_416F_A390_B1FF5EDBCC0B__INCLUDED_)
