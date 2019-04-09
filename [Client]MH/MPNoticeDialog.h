// MPNoticeDialog.h: interface for the CMPNoticeDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPNOTICEDIALOG_H__BF4A4A1B_7176_44C7_A0D4_A8E0AD71ABF6__INCLUDED_)
#define AFX_MPNOTICEDIALOG_H__BF4A4A1B_7176_44C7_A0D4_A8E0AD71ABF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "INTERFACE\cDialog.h"
class cTextArea;
class CMPNoticeDialog  : public cDialog
{
	cTextArea * m_pNCaution;
	cTextArea * m_pNRedCaution;
public:
	CMPNoticeDialog();
	virtual ~CMPNoticeDialog();
	void Linking();
};

#endif // !defined(AFX_MPNOTICEDIALOG_H__BF4A4A1B_7176_44C7_A0D4_A8E0AD71ABF6__INCLUDED_)
