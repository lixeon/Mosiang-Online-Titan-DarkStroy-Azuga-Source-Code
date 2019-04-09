// cText.h: interface for the cText class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTEXT_H__B882E313_8DC1_4862_A557_4EBABEBF8DFE__INCLUDED_)
#define AFX_CTEXT_H__B882E313_8DC1_4862_A557_4EBABEBF8DFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "cWindow.h"


class line_node;
class cImage;

class cText : public cWindow
{
public:
	cText();
	virtual ~cText();

	//virtual void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID=0);
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	virtual void Render();
	void Release();

	void SetFGColor(DWORD color){ m_fgColor = color; }
	void SetTextXY(LONG x, LONG y){	m_textXY.x = x; m_textXY.y = y; }
	void SetDisappearTime(DWORD disappearTime){ m_dwDisappearTime =disappearTime; m_dwStartTime = GetTickCount(); m_bActive = TRUE; }
	virtual void SetText(char * text){}
	
	
protected:
	cCoord m_textXY;
	DWORD m_fgColor;
	
	
/*	int		m_line_num;
	int		m_box_width;
	int		m_box_height;*/

	line_node * topLine;
	
	DWORD m_dwStartTime;
	DWORD m_dwDisappearTime;
};

#endif // !defined(AFX_CTEXT_H__B882E313_8DC1_4862_A557_4EBABEBF8DFE__INCLUDED_)
