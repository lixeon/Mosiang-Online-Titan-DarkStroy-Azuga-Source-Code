// MiniFriendDialog.h: interface for the CMiniFriendDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MINIFRIENDDIALOG_H__3565EDD2_97D4_4E4F_8535_23A911D95094__INCLUDED_)
#define AFX_MINIFRIENDDIALOG_H__3565EDD2_97D4_4E4F_8535_23A911D95094__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INTERFACE\cDialog.h"
#include ".\interface\cWindowHeader.h"

class CMiniFriendDialog : public cDialog  
{
	cStatic * m_pName;
	cEditBox * m_pNameEdit;
	cButton * m_pAddOkBtn;
	cButton * m_pAddCancelBtn;
	
public:
	CMiniFriendDialog();
	virtual ~CMiniFriendDialog();
	void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID=0);
	void Linking();
	virtual void SetActive(BOOL val);
	void SetName(char* Name);
};

#endif // !defined(AFX_MINIFRIENDDIALOG_H__3565EDD2_97D4_4E4F_8535_23A911D95094__INCLUDED_)
