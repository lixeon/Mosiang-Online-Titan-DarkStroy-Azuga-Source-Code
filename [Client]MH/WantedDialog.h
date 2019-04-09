// WantedDialog.h: interface for the CWantedDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WANTEDDIALOG_H__3B15B3D1_1523_4A58_AB12_CC7BCE34F009__INCLUDED_)
#define AFX_WANTEDDIALOG_H__3B15B3D1_1523_4A58_AB12_CC7BCE34F009__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INTERFACE\cDialog.h"

class cListDialog;

class CWantedDialog : public cDialog
{
	cListDialog* m_pWantedLDG;
	

public:
	CWantedDialog();
	virtual ~CWantedDialog();

	void Linking();
	void SetInfo(WANTEDLIST* pInfo);
	void AddInfo(WANTEDLIST* pInfo);
	void InitWanted();
	
};

#endif // !defined(AFX_WANTEDDIALOG_H__3B15B3D1_1523_4A58_AB12_CC7BCE34F009__INCLUDED_)
