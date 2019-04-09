
//////////////////////////////////////////////////////////////////////
// class name : cListDialog 
// Lastest update : 2002.10.29. by taiyo
//////////////////////////////////////////////////////////////////////

#ifndef _cLISTDIALOG_H_
#define _cLISTDIALOG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cDialog.h"
#include "cListItem.h"


enum {LD_TOP, LD_MIDDLE, LD_DOWN};


class cListDialog : public cDialog, public cListItem
{
public:
	cListDialog();
	virtual ~cListDialog();

	virtual void Init(LONG x, LONG y, WORD wid, cImage * topImage, WORD topHei, cImage * middleImage, WORD middleHei, cImage * downImage, WORD downHei, cImage * overImage, BYTE num, LONG ID);
	virtual void InitList(WORD wMaxLine, RECT * textClippingRect);
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	virtual void Add(cWindow * btn);
	virtual void Render();
	BOOL IsMaxLineOver();

	BOOL Resize(BYTE middleNum);
	WORD GetHeight(BYTE i)
	{
		if(i == LD_TOP) return m_topHeight;
		if(i == LD_MIDDLE) return m_middleHeight;
		if(i == LD_DOWN) return m_downHeight;
		return 0;
	}
	BYTE GetMiddleImageNum(){ return m_middleNum; }
	void SetMinMaxMiddleNum(BYTE min, BYTE max) { m_minMiddleNum = min; m_maxMiddleNum = max; }
	void SetOverImageScale(float x, float y) { m_OverImageScale.x = x; m_OverImageScale.y = y; }

	void OnUpwardItem();
	void OnDownwardItem();
	int PtIdxInRow(LONG x, LONG y);

	virtual void AddItem(char * str, DWORD color, int line=-1);
//	virtual void RemoveItem( char * str );

	void onSize(BOOL fSize);
//	virtual void ListMouseCheck( LONG x, LONG y, DWORD we );
	void ListMouseCheck( CMouse * mouseInfo );
	
	int GetCurSelectedRowIdx();
	void SetCurSelectedRowIdx(int Idx);
	void ResetGuageBarPos();
	void SetLineHeight( LONG height );

	void SetAutoScroll( BOOL bAuto ) { m_bAutoScroll = bAuto; }
	
	int GetSelectRowIdx()		{ return m_SelectedRowIdx;	}
	int GetTopListItemIdx()		{ return m_lTopListItemIdx;	}
	void SetTopListItemIdx(int Idx);	

	cPtrList* GetListItem()		{ return &m_ListItem; }

	BOOL PtInWindow(LONG x, LONG y);

	void SetShowSelect(BOOL val);

	ITEM*	GetSelectedITEM( CMouse * mouseInfo );
	
protected:

	LONG m_lLineHeight;
	LONG m_lLineNum;
	LONG m_lTopListItemIdx;
	cImage m_TopImage;		WORD m_topHeight;
	cImage m_MiddleImage;	WORD m_middleHeight;
	cImage m_DownImage;		WORD m_downHeight;

	BYTE m_middleNum;
	BYTE m_minMiddleNum;
	BYTE m_maxMiddleNum;
	RECT m_textRelRect;						// only left,top,bottom field is used

	cButton * m_pUpwardBtn;
	cButton * m_pDownwardBtn;
	cButton * m_pLengthLongBtn;
	cButton * m_pLengthShortBtn;

	cGuageBar * m_pGuageBar;

	// 마우스 클릭에 대한 처리
	int		m_SelectedRowIdx;

	int		m_ClickSelected;

	cImage			m_OverImage;
	cImageScale		m_OverImageScale;
	float			m_fGap;
	
	BOOL			m_bAutoScroll;
	BOOL			m_bShowSelect;

	// list bar
	//cImage * m_pBarImage;
	//VECTOR2	m_barListPos;
	//cSize	m_barListSize;
	//int		m_interval;
	//BOOL	m_fBarDrag;
	//LONG	m_dragGabY;
};

#endif // _cLISTDIALOG_H_
