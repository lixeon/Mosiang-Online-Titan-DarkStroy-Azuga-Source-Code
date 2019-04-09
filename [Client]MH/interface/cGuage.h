// cGuage.h: interface for the cGuage class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _cGUAGE_H_
#define _cGUAGE_H_

#if _MSC_VER > 1000
#pragma once 
#pragma warning(disable:4244)
#endif // _MSC_VER > 1000

#include "cWindow.h"

// coded by taiyo
// 2003.10.29.
//!!!!! This cGugae class is deprecated !!!!!
// you had better use cGuagen OR CObjectGuagen class

class cGuage : public cWindow  
{
public:
	cGuage();
	virtual ~cGuage();
	
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	virtual void Render();
	virtual void SetAbsXY(LONG x, LONG y);
	void InitGuage(LONG x, LONG y);
	void SetPieceImage(cImage * piece) { m_GuagePieceImage = *piece; }

	void SetMinValue(LONG minv) 
	{ 
		m_minValue = minv; 
		m_fChanged = TRUE; 
	}
	void SetMaxValue(LONG maxv) { 
		m_maxValue = maxv; 
		m_fChanged = TRUE; 
	}
	void SetCurValue(LONG val) { 
		m_curValue = val;
		if(m_maxValue < m_curValue)
			m_curValue = m_maxValue;
		m_fChanged = TRUE;
	}
	
protected:
	
	cImage	m_GuagePieceImage;
	VECTOR2 m_imgAbsPos;
	VECTOR2 m_imgRelPos;

	
	WORD m_curValue;
	WORD m_minValue;
	WORD m_maxValue;
	float m_fillPersent;
	BOOL m_fChanged;
};

#endif // _cGUAGE_H_
