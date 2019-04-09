// TitanRepairDlg.h: interface for the CTitanPartsChangeDlg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _TITANREPAIRDLG_H
#define _TITANREPAIRDLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./interface/cDialog.h"
#include "GameResourceStruct.h"

class CItem;

class CTitanRepairDlg : public cDialog
{	


public:
	CTitanRepairDlg();
	virtual ~CTitanRepairDlg();
	void Linking();
	virtual void SetActive( BOOL val );
	BOOL OnActionEvent(LONG lId, void * p, DWORD we);	
};

#endif // _TITANREPAIRDLG_H