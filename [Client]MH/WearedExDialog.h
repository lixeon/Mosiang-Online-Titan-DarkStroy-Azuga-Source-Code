// WearedExDialog.h: interface for the CWearedExDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEAREDEXDIALOG_H__921DB71C_9868_404D_9125_3AEE663B3341__INCLUDED_)
#define AFX_WEAREDEXDIALOG_H__921DB71C_9868_404D_9125_3AEE663B3341__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cIconDialog.h"

class CWearedExDialog : public cIconDialog  
{
public:
	CWearedExDialog();
	virtual ~CWearedExDialog();

	BOOL AddItem(WORD relPos, cIcon * InIcon);
	BOOL DeleteItem(WORD relPos, cIcon ** outIcon);
};

#endif // !defined(AFX_WEAREDEXDIALOG_H__921DB71C_9868_404D_9125_3AEE663B3341__INCLUDED_)
