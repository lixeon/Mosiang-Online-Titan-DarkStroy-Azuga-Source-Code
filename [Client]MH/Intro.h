// Intro.h: interface for the CIntro class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _INTRO_H
#define _INTRO_H

#include "GameState.h"
#include "MovieControl.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define INTRO USINGTON(CIntro)

class CIntro : public CGameState
{
	MovieControl* m_MovieControl;
	BOOL m_bReplayIntroFlag;

public:
	CIntro();
	virtual ~CIntro();

	BOOL Init(void* pInitParam);
	void Release(CGameState* pNextGameState);

	void Process();
	void BeforeRender();
	void AfterRender();

	void NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg);
	void SetReplayIntroFlag()	{	m_bReplayIntroFlag = TRUE;	}
};

EXTERNGLOBALTON(CIntro);

#endif	// _Intro_H