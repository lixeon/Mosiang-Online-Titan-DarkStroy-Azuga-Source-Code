// BailDialog.h: interface for the CBailDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BAILDIALOG_H__BEC1839F_4BA9_4149_898D_5983A18EF3C8__INCLUDED_)
#define AFX_BAILDIALOG_H__BEC1839F_4BA9_4149_898D_5983A18EF3C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./interface/cDialog.h"


class cEditBox;
class cTextArea;


class CBailDialog : public cDialog
{
	cEditBox*		m_pBailEdtBox;
	cTextArea*		m_pBailText;
	DWORD			m_BadFame;	
	
public:
	CBailDialog();
	virtual ~CBailDialog();
	void Open();
	void Close();
	void Linking();

	void SetFame();
	void SetBadFrameSync();
};

#endif // !defined(AFX_BAILDIALOG_H__BEC1839F_4BA9_4149_898D_5983A18EF3C8__INCLUDED_)
