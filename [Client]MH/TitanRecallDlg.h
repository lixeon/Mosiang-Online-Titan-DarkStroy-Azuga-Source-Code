// CTitanRecallDlg.h: interface for the CTitanPartsChangeDlg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _TITANRECALLDLG_H
#define _TITANRECALLDLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./interface/cDialog.h"
#include "GameResourceStruct.h"
#include "ProgressBarDlg.h"

class CItem;

class CTitanRecallDlg : public CProgressBarDlg
{
protected:	
	BOOL		m_bSuccessRecall;
	
public:
	CTitanRecallDlg();
	virtual ~CTitanRecallDlg();

	void Linking();
	virtual void Render();	
	BOOL OnActionEvent(LONG lId, void * p, DWORD we);	
		
	BOOL GetSuccessRecall();
	void SetSuccessRecall(BOOL bVal);
};

#endif // _TITANRECALLDLG_H