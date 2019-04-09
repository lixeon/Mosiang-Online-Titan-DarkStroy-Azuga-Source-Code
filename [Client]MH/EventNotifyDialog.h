// EventNotifyDialog.h: interface for the CEventNotifyDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVENTNOTIFYDIALOG_H__E2980439_B4F9_4B5C_9113_5371513C00AC__INCLUDED_)
#define AFX_EVENTNOTIFYDIALOG_H__E2980439_B4F9_4B5C_9113_5371513C00AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"

class cStatic;
class cTextArea;

class CEventNotifyDialog : public cDialog
{
protected:

	cStatic*	m_pStcTitle;
	cTextArea*	m_pTAContext;
	
	
public:
	CEventNotifyDialog();
	virtual ~CEventNotifyDialog();

	virtual void SetActive( BOOL val );
	virtual DWORD ActionEvent( CMouse* mouseInfo );
	void Linking();

	void SetTitle( char* pStrTitle );
	void SetContext( char* pStrContext );
	void SetEventCount( BOOL bAdd );
};

#endif // !defined(AFX_EVENTNOTIFYDIALOG_H__E2980439_B4F9_4B5C_9113_5371513C00AC__INCLUDED_)
