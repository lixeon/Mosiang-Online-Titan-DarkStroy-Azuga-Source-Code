// SkinSelectDialog.h: interface for the CSkinSelectDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _SKINSELECTDLG_H
#define _SKINSELECTDLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./interface/cDialog.h"
#include "./interface/cListDialog.h"
#include "./interface/cIconDialog.h"
#include "ItemShow.h"
#include "GameResourceStruct.h"
#include "../input/Mouse.h"

class cListDialog;

class CSkinSelectDialog : public cDialog
{	
private:
	cListDialog*	m_pNomalSkinListDlg;
	cIconDialog*	m_pNomalSkinIconDlg;
	CItemShow		m_NomalSkinView[SKINITEM_LIST_MAX];

	DWORD m_dwSelectIdx;
	DWORD m_dwSkinDelayTime;	//딜레이 총 시간 변수
	BOOL m_bSkinDelayResult;	//딜레이중인지 체크하는 변수

public:
	CSkinSelectDialog();
	virtual ~CSkinSelectDialog();
	void Linking();
	virtual void SetActive( BOOL val );	
	virtual DWORD ActionEvent(CMouse* mouseInfo);
	BOOL OnActionEvent(LONG lId, void * p, DWORD we);
	//virtual void Render();

	void SkinItemListInfo();
	/*
	void InitSkinDelayTime();
	void StartSkinDelayTime();
	BOOL CheckDelay();
	*/
};

#endif // _SKINSELECTDLG_H
