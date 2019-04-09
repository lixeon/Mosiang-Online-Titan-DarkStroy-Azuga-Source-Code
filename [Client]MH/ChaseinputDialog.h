// ChaseinputDialog.h: interface for the CChaseinputDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHASEINPUTDIALOG_H__62846ABC_7CBF_4A95_AAE4_D19D82CC9381__INCLUDED_)
#define AFX_CHASEINPUTDIALOG_H__62846ABC_7CBF_4A95_AAE4_D19D82CC9381__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"


class cEditBox;




class CChaseinputDialog : public cDialog
{
	cEditBox*			m_pEditName;
	DWORD				m_LastChktime;
	DWORD				m_dwItemIdx;

public:
	CChaseinputDialog();
	virtual ~CChaseinputDialog();

	void Linking();
	void SetActive( BOOL val );
	void SetItemIdx( DWORD dwItem )			{	m_dwItemIdx = dwItem;	}
	
	void WantedChaseSyn();
};

#endif // !defined(AFX_CHASEINPUTDIALOG_H__62846ABC_7CBF_4A95_AAE4_D19D82CC9381__INCLUDED_)
