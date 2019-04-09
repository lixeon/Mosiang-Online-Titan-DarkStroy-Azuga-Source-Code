// WantRegistDialog.h: interface for the CWantRegistDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WANTREGISTDIALOG_H__D0DA3E84_19F0_4258_929C_89F906654785__INCLUDED_)
#define AFX_WANTREGISTDIALOG_H__D0DA3E84_19F0_4258_929C_89F906654785__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INTERFACE\cDialog.h"

class cStatic;
class cEditBox;

class CWantRegistDialog : public cDialog  
{
	cStatic*	m_WantedName;
	cEditBox*	m_PrizeEdit;

	BOOL		m_bShow;
	DWORD		m_dwStartShowTime;

public:
	CWantRegistDialog();
	virtual ~CWantRegistDialog();

	void Linking();

	void SetWantedName(char* pName);

	virtual void SetActive( BOOL val );
	virtual DWORD ActionEvent( CMouse * mouseInfo );
	virtual void Render();
};

#endif // !defined(AFX_WANTREGISTDIALOG_H__D0DA3E84_19F0_4258_929C_89F906654785__INCLUDED_)
