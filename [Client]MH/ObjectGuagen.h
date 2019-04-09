// ObjectGuagen.h: interface for the CObjectGuagen class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECTGUAGEN_H__F108E926_3ACD_46A2_9B2B_B960C4E9A2EB__INCLUDED_)
#define AFX_OBJECTGUAGEN_H__F108E926_3ACD_46A2_9B2B_B960C4E9A2EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cGuagen.h"
//#include "./Interface/cAnimationManager.h"

/*
 *	CObjectGuagen class - date : 2003. 10. 11
 *	coded by taiyo
 *
 *	< SCRIPT CODE >
 *	#GUAGENEVALUE				=			SetValue(,)
 *	#GUAGEEFFECTPIECEIMAGE		=			SetPieceImage()
 *	#GUAGEEFFECTPIECEWIDTH		=			SetGuagePieceWidth()
 */

class CObjectGuagen : public cGuagen  
{
public:
	CObjectGuagen();
	virtual ~CObjectGuagen();

	void Render();
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	void SetValue( GUAGEVAL val, DWORD estTime );

	void SetEffectPieceImage(cImage * piece)		{	m_GuageEffectPieceImage = *piece;		}
	void SetGuageEffectPieceWidth(float width)		{	m_fGuageEffectPieceWidth = width;		}
	void SetGuageEffectPieceHeightScale(float hei)		{	m_fGuageEffectPieceHeightScaleY = hei;	}
protected:
	cImage			m_GuageEffectPieceImage;
	float			m_fGuageEffectPieceWidth;
	float			m_fOldPercentRate;
	float			m_fCurPercentRate;
	float			m_fIncAmount;
	DWORD			m_dwEffectTime;
	DWORD			m_dwStartTime;
//	cAnimationManager m_ani;

	BOOL			m_bBlink;
	DWORD			m_dwStartBlinkTime;
	float			m_fGuageEffectPieceHeightScaleY;
};

#endif // !defined(AFX_OBJECTGUAGEN_H__F108E926_3ACD_46A2_9B2B_B960C4E9A2EB__INCLUDED_)
