// SkinSelectDialog.h: interface for the CSkinSelectDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _NUMBERPADDLG_H
#define _NUMBERPADDLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./interface/cDialog.h"
#include "./interface/cListDialog.h"
#include "./interface/cIconDialog.h"
#include "./interface/cPushupButton.h"
#include "./interface/cStatic.h"
#include "./interface/cComboBox.h"
#include "ItemShow.h"
#include "GameResourceStruct.h"
#include "../input/Mouse.h"

class cListDialog;

class CNumberPadDialog : public cDialog
{
private:
	enum
	{
		ePROTECTIONSTR_MAXNUM = 10,	//보안문자 최대 저장 수
	};

public:
	CNumberPadDialog();
	virtual ~CNumberPadDialog();
	void Linking();
	virtual void SetActive( BOOL val );		
	BOOL OnActionEvent(LONG lId, void * p, DWORD we);		
	void InsertStr(char* pStr);
	
	void InitProtectionStr();
	char* GetProtectionStr();
	

private:	
	char m_pProtectionStr[ePROTECTIONSTR_MAXNUM];
	cStatic* m_pStaticPN;
	cWindow* m_pLogInWindow;
	cComboBox* m_pCombo;
};

#endif // _NUMBERPADDLG_H
