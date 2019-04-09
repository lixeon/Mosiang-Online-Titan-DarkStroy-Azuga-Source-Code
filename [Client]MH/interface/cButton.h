//////////////////////////////////////////////////////////////////////
// class name : cButton 
// Lastest update : 2002.10.29. by taiyo
//////////////////////////////////////////////////////////////////////

#ifndef _cBUTTON_H_
#define _cBUTTON_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cWindow.h"
#include "cWindowDef.h"




class cButton : public cWindow  
{
public:
	cButton();
	virtual ~cButton();

	virtual void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, cImage * overImage, cImage * pressImage, cbFUNC Func, LONG ID=0);
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	virtual void Render();
	void SetText(char * text, DWORD basicColor, DWORD overColor=NULL, DWORD PressColor=NULL);
	void SetTextValue(DWORD value);
	void SetTextXY(LONG x, LONG y) { m_textXY.x = x; m_textXY.y = y; }

	BOOL IsClickInside() { return m_bClickInside; }
	void SetTextAni( BYTE x, BYTE y ) { m_BtnTextAniX = x; m_BtnTextAniY = y; }

	void SetClickSound( SNDIDX idx ) { m_sndIdx = idx; m_bSound = TRUE; }
	
	void SetShadowTextXY(LONG x, LONG y)	{	m_shadowTextXY.x = x; m_shadowTextXY.y = y;		}
	void SetShadowColor(DWORD color)		{	m_shadowColor = color;							}
	void SetShadow(BOOL val)				{	m_bShadow = val;								}
	
//KES INTERFACE 031028	
	void SetAlign( int nAlign )				{ m_nAlign = nAlign; }
//

protected:	
	cImage	m_OverImage;
	cImage	m_PressImage;
	
	cCoord m_textXY;
	
	// Text label of button
	int m_nCurTextLen;
	char m_szText[MAX_TEXT_SIZE];
	DWORD m_fgBasicColor;
	DWORD m_fgOverColor;
	DWORD m_fgPressColor;
	DWORD m_fgCurColor;
	
//KES INTERFACE 031006
	BOOL m_bClickInside;
	BYTE m_BtnTextAniX;
	BYTE m_BtnTextAniY;
	int	 m_nAlign;

	SNDIDX	m_sndIdx;
	BOOL	m_bSound;
	
	BOOL			m_bShadow;
	cCoord			m_shadowTextXY;
	DWORD			m_shadowColor;
};

#endif // _cBUTTON_H_
