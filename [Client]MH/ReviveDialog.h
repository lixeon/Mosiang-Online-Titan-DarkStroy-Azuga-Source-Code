// ReviveDialog.h: interface for the CReviveDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REVIVEDIALOG_H__A7D60BE2_A7EB_44C3_934B_63CC9FA02380__INCLUDED_)
#define AFX_REVIVEDIALOG_H__A7D60BE2_A7EB_44C3_934B_63CC9FA02380__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"

class CReviveDialog : public cDialog  
{
	cButton*	m_pPresentBtn;
	cButton*	m_pLoginBtn;
	cButton*	m_pVillageBtn;

public:
	CReviveDialog();
	virtual ~CReviveDialog();

	void Linking();
	virtual void SetActive( BOOL val );
};

#endif // !defined(AFX_REVIVEDIALOG_H__A7D60BE2_A7EB_44C3_934B_63CC9FA02380__INCLUDED_)
