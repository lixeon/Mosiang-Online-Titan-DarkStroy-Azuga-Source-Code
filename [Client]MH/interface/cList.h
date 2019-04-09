// cList.h: interface for the cList class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _cLIST_H_
#define _cLIST_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cWindow.h"
#include "cListItem.h"


class cButton;

// 
// cList : °íÁ¤ »çÀÌÁî ¸®½ºÆ®
// 
class cList : public cWindow , public cListItem
{
public:
	cList();
	virtual ~cList();
	
	virtual void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID);
	virtual void InitList(WORD wMaxLine, RECT * textClippingRect);
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	virtual void Render();
	virtual void Add(cWindow * pushupBtn);
	virtual void SetAbsXY(LONG x, LONG y);

	//virtual DWORD ActionEvent(CMouse * mouseInfo);
	void SetMargin(RECT * textClippingRect);
	void OnUpwardItem();
	void OnDownwardItem();
	void SetLineUpward(BOOL val=TRUE) { m_fUpwardLine = val; }	// yet, not used

	BOOL PtInListBar(LONG x, LONG y);
	void SetListBar(LONG x, LONG y, WORD wid, WORD hei, int interval);
	void SetListBarImage(cImage * image){ m_BarImage = *image; }
protected:
	void Resize();

	BOOL m_fUpwardLine;
	LONG m_lLineHeight;
	LONG m_lLineNum;
	LONG m_lTopListItemIdx;
	RECT m_textClippingRect;

	cButton * m_pUpwardBtn;
	cButton * m_pDownwardBtn;

	// list bar
	cImage m_BarImage;
	VECTOR2	m_barListPos;
	cSize	m_barListSize;
	int		m_interval;
	BOOL	m_fBarDrag;
	LONG	m_dragGabY;

};

#endif // _cLIST_H_
