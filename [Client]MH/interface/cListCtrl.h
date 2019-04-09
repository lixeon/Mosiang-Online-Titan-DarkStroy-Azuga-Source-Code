// cListCtrl.h: interface for the cListCtrl class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _cLISTCTRL_H_
#define _cLISTCTRL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cWindow.h"
#include "cReportItem.h"


class cListCtrl : public cWindow, public cReportItem
{
public:
	cListCtrl();
	virtual ~cListCtrl();

	virtual void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID);
	virtual void InitListCtrl(WORD wMaxColumns, WORD wLinePerPage);
	virtual void InitListCtrlImage(cImage * HeadImage, BYTE HHeight, cImage * BodyImage, BYTE BHeight, cImage * OverImage );
	virtual void Render();
	virtual DWORD ActionEvent(CMouse * mouseInfo);

	void SetMargin(const RECT * textRelRect);
	WORD PtIdxInRow(LONG x, LONG y);
	int GetSelectedRowIdx(){ return m_SelectedRowIdx; }
	void SetSelectedRowIdx( int Idx )		{	m_SelectedRowIdx = Idx;	}

	void SetOverImageScale( float x, float y ) { m_OverImageScale.x = x; m_OverImageScale.y = y; }
	void SetSelectOption( WORD wOption ) { m_wSelectOption = wOption; }
	
protected:

	cImage m_HeadImage;
	cImage m_BodyImage;
	cImage m_OverImage;

	cImageScale		m_OverImageScale;

	BYTE m_bHeadLineHeight;
	BYTE m_bBodyLineHeight;

	WORD m_wLinePerPage;
	RECT m_textRelRect;				// right, bottom field is not used

	int m_SelectedRowIdx;					//현재 선택된 RITEM의 dwData값을 저장
	LONG m_lTopListItemIdx;

	int	m_nOverIdx;
	float m_fGap;
	
	WORD	m_wSelectOption;	// 오버 이미지를 사용할때 옵션값이다. ( 0: 기본, 1: 선택시 )
};


#endif // _cLISTCTRL_H_
