//////////////////////////////////////////////////////////////////////
// class name : cAni 
// Lastest update : 2003.3.10. by taiyo
//////////////////////////////////////////////////////////////////////

#ifndef __cANI_H__
#define __cANI_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cWindow.h"

class cAni : public cWindow  
{
public:
	cAni();
	virtual ~cAni();

	virtual DWORD ActionEvent(CMouse * mouseInfo);
	virtual void Render();

	void SetMaxSprite(WORD maxSprite);
	void SetTravelingType(WORD type) { m_wTravelingType = type; }
	void SetLoop(BOOL val) { m_fLoop = val; }
	void AddSprite(cImage * sprite, WORD delay);

	void SetCurSpriteIdx(LONG Idx) { _m_CurSpriteIdx = Idx; }

	void Play() { m_fAniTransactionCompleted = FALSE; }
	void Stop() { m_fAniTransactionCompleted = TRUE; }

protected:
	
	BOOL		m_fLoop;
	BOOL		m_fAniTransactionCompleted;
	WORD		m_wTravelingType;
	
	WORD		m_wMaxSprite;
	WORD *		m_pwSpriteDelay;
	cImage *	m_pSpriteImage;


	// frame calc
	LONG _m_CurSpriteIdx;
	DWORD _m_LastTick;
	DWORD _m_CurTick;
	BOOL _m_fInverse;
	
};

#endif // __cANI_H__
