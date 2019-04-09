// SkinSelectDialog.h: interface for the CSkinSelectDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _COSTUMESKINSELECTDLG_H
#define _COSTUMESKINSELECTDLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./interface/cDialog.h"
#include "./interface/cListDialog.h"
#include "./interface/cIconDialog.h"
#include "./interface/cPushupButton.h"
#include "ItemShow.h"
#include "GameResourceStruct.h"
#include "../input/Mouse.h"

class cListDialog;

class CCostumeSkinSelectDialog : public cDialog
{	
private:	
	enum TAB_BTN
	{
		eTabBtn_Hat = 0,
		eTabBtn_Dress,
		eTabBtn_Accessory,

		eTabBtn_Max,
	};

private:
	cListDialog*	m_pCostumeSkinListDlg;
	cIconDialog*	m_pCostumeSkinIconDlg;			
	cPushupButton*	m_pCostumTabBtn[eTabBtn_Max];	
	CItemShow		m_CostumeSkinView;

	DWORD m_dwSelectIdx;
	DWORD m_dwSkinDelayTime;	//딜레이 총 시간 변수
	BOOL m_bSkinDelayResult;	//딜레이중인지 체크하는 변수

	CYHHashTable<SKIN_SELECT_ITEM_INFO>	m_CostumeSkinHat;	//머리 종류 데이터
	CYHHashTable<SKIN_SELECT_ITEM_INFO>	m_CostumeSkinDress;	//옷 종류 데이터
	CYHHashTable<SKIN_SELECT_ITEM_INFO>	m_CostumeSkinAccessory;	//악세사리 종류 데이터
	CYHHashTable<SKIN_SELECT_ITEM_INFO>* m_pCurrentSkinTable; //현재 탭의 테이블 

public:
	CCostumeSkinSelectDialog();
	virtual ~CCostumeSkinSelectDialog();
	void Linking();
	virtual void SetActive( BOOL val );	
	virtual DWORD ActionEvent(CMouse* mouseInfo);
	BOOL OnActionEvent(LONG lId, void * p, DWORD we);
	//virtual void Render();

	void CostumeSkinKindData();	//탭 종류별 스킨 데이터 정렬하는 함수
	void CostumeSkinListInfo(TAB_BTN eCostumeKind);	//종류에 따라 스킨 데이터를 리스트에 Add하는 함수
	/*
	void InitSkinDelayTime();
	void StartSkinDelayTime();
	BOOL CheckDelay();
	*/
	SKIN_SELECT_ITEM_INFO* GetCurrentSkinInfo(DWORD dwSelectIdx);
	void SetCostumTabBtnFocus(TAB_BTN eCostumeKind);
};

#endif // _COSTUMESKINSELECTDLG_H