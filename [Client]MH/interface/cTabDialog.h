// cTabDialog.h: interface for the cTabDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _cTABDIALOG_H_
#define _cTABDIALOG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cDialog.h"


class cPushupButton;

class cTabDialog : public cDialog  
{
public:
	cTabDialog();
	virtual ~cTabDialog();
	
	void InitTab(BYTE tabNum);
	virtual void Render();
	virtual void RenderTabComponent();
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	void SetAbsXY(LONG x, LONG y);
	virtual void SetActive(BOOL val);
	virtual void SetAlpha(BYTE al);
	virtual void SetOptionAlpha( DWORD dwAlpha);

	void AddTabBtn(BYTE idx, cPushupButton * btn);
	void AddTabSheet(BYTE idx, cWindow * sheet);
	
	cPushupButton * GetTabBtn(BYTE idx);
	cWindow * GetTabSheet(BYTE idx);

	BYTE GetCurTabNum() { return m_bSelTabNum; }
	BYTE GetTabNum(){ return m_bTabNum; }
	
	virtual void SetDisable( BOOL val );
	virtual void SelectTab(BYTE idx);
	
	virtual cWindow * GetWindowForID(LONG id);
protected:
	BYTE curIdx1;
	BYTE curIdx2;
	BYTE m_bTabNum;
	BYTE m_bSelTabNum;
	cPushupButton ** m_ppPushupTabBtn;
	cWindow ** m_ppWindowTabSheet;
//	DWORD	m_BtnPushstartTime;
//	DWORD	m_BtnPushDelayTime;
};

#endif // _cTABDIALOG_H_
