// TitanChangePreViewDlg.h: interface for the CTitanChangePreViewDlg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _TITANCHANGEPREVIEWDLG_H
#define _TITANCHANGEPREVIEWDLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./interface/cDialog.h"
#include "./interface/cStatic.h"

class CTitanChangePreViewDlg : public cDialog
{

public:

	CTitanChangePreViewDlg();
	virtual ~CTitanChangePreViewDlg();
	void Linking();
	virtual void SetActive( BOOL val );
};

#endif // _TITANCHANGEPREVIEWDLG_H