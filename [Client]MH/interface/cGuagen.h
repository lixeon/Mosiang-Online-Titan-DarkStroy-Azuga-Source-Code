// cGuagen.h: interface for the cGuagen class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGUAGEN_H__911995B7_6B58_4E71_A042_6D7F8B6C6ADF__INCLUDED_)
#define AFX_CGUAGEN_H__911995B7_6B58_4E71_A042_6D7F8B6C6ADF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "cWindow.h"

typedef float	GUAGEVAL;
typedef BOOL	GUAGEBOOL;
typedef VECTOR2 GUAGEPOS;
typedef LONG	GUAGEXY;
/*
 *	cGuagen class - date : 2003. 10. 10
 *	coded by taiyo
 *
 *	< SCRIPT CODE >
 *	#GUAGECURVALUE			=			SetValue()
 *	#GUAGEPIECEIMAGE		=			SetPieceImage()
 *	#GUAGEPOS				=			SetGuageImagePos()
 *	#GUAGEWIDTH				=			SetGuageWidth()
 *	#GUAGEPIECEWIDTH		=			SetGuagePieceWidth()
 */
class cGuagen : public cWindow  
{
public:
	cGuagen();
	virtual ~cGuagen();

	virtual void Render();

	void SetGuageImagePos( GUAGEXY imgX, GUAGEXY imgY)
	{
		m_imgRelPos.x	= (float)imgX;
		m_imgRelPos.y	= (float)imgY;
	}

	// 0 ~ 1 no aida
	void SetValue( GUAGEVAL val ) 
	{ 
		m_fPercentRate = val;
		if(1.f < m_fPercentRate)
			m_fPercentRate = 1.f;
	}

	void SetPieceImage(cImage * piece)				{	m_GuagePieceImage = *piece;			}
	void SetGuageWidth(float width)					{	m_fGuageWidth = width;				}
	void SetGuagePieceWidth(float width)			{	m_fGuagePieceWidth = width;			}
	void SetGuagePieceHeightScale(float hei)		{	m_fGuagePieceHeightScaleY = hei;	}
protected:
	
	cImage			m_GuagePieceImage;
	GUAGEPOS		m_imgRelPos;

	float			m_fGuageWidth;
	float			m_fGuagePieceWidth;
	GUAGEVAL		m_fPercentRate;
	float			m_fGuagePieceHeightScaleY;

};

#endif // !defined(AFX_CGUAGEN_H__911995B7_6B58_4E71_A042_6D7F8B6C6ADF__INCLUDED_)
