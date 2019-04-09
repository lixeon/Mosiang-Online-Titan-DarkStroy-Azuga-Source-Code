//////////////////////////////////////////////////////////////////////
// class name : cDialog 
// Lastest update : 2002.10.29. by taiyo
//////////////////////////////////////////////////////////////////////

#ifndef _cDIALOG_H_
#define _cDIALOG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cWindow.h"

class cPtrList;

class cIcon;
class cGuageBar;
class cButton;

class cDialog : public cWindow  
{
public:
	cDialog();
	virtual ~cDialog();

	virtual void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID=0);
	virtual void Render();
	virtual void RenderWindow();
	virtual void RenderComponent();
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	virtual DWORD ActionKeyboardEvent( CKeyboard* keyInfo );

	virtual DWORD ActionEventWindow(CMouse * mouseInfo);
	virtual DWORD ActionEventComponent(CMouse * mouseInfo);
	virtual DWORD ActionKeyboardEventComponent(CKeyboard * keyInfo);

	virtual void SetAutoClose(BOOL val){ m_bAutoClose = val; }
	virtual BOOL IsAutoClose(){ return m_bAutoClose; }
	BOOL PtInCaption(LONG x, LONG y);
	virtual void Add(cWindow * window);
	void DestroyCtrl();
	virtual void SetAbsXY(LONG x, LONG y);
	virtual void SetActive( BOOL val );
	virtual void SetActiveRecursive(BOOL val);
	virtual void SetAlpha(BYTE al);
	virtual void SetOptionAlpha( DWORD dwAlpha );

	void SetCaptionRect(RECT * pRect){ CopyRect(&m_captionRect, pRect); }
	RECT* GetCaptionRect() { return &m_captionRect; }

	virtual void SetDisable( BOOL val );
	
	//---- 
	//virtual void FakeAddIcon(LONG x, LONG y, cIcon * icon){}
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon){return FALSE;}
	virtual void FakeDeleteIcon(WORD pos, cIcon * icon){}
	//---- 

	virtual cWindow * GetWindowForID(LONG id);
	cPtrList * m_pComponentList;

protected:
	BOOL m_bAutoClose;
	RECT m_captionRect;
	//cGuageBar * m_pGuageBar;
	cButton * m_pCloseBtn;
	
	BOOL	m_bCaptionHit;
};

#endif // _cDIALOG_H_
