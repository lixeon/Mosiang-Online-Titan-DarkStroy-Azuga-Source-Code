// GTRegistcancelDialog.h: interface for the CGTRegistcancelDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GTREGISTCANCELDIALOG_H__235970D1_B766_4BB4_BC4B_5E02FD60A8B5__INCLUDED_)
#define AFX_GTREGISTCANCELDIALOG_H__235970D1_B766_4BB4_BC4B_5E02FD60A8B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "./Interface/cDialog.h"


class cButton;



class CGTRegistcancelDialog : public cDialog
{
	cButton*		m_pCancelBtn;

public:
	CGTRegistcancelDialog();
	virtual ~CGTRegistcancelDialog();

	void Linking();
	void SetActive( BOOL val );
	void TournamentRegistCancelSyn();
};

#endif // !defined(AFX_GTREGISTCANCELDIALOG_H__235970D1_B766_4BB4_BC4B_5E02FD60A8B5__INCLUDED_)
