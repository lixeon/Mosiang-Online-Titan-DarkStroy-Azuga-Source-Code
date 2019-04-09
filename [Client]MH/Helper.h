//////////////////////////////////////////////
// 1. AnimationÀ» °¡´ÉÇÏ°Ô ¸¸µé°ÍÀÓ. (ÇöÀç ÇÑÀå Å×½ºÆ®)
// 
//////////////////////////////////////////////
#ifndef _HELPER_
#define _HELPER_

#include "./interface/cAni.h"
#include "./interface/cDialog.h"


enum HELPER_MOTION {

	emHM_Stand,
	emHM_MAX,
};

class cHelper : public cDialog // WindowId µî·Ï ÇØ ³õ´Â´Ù.
{
protected:
	cAni	m_MotionList[emHM_MAX];	// µ¿ÀÛ ¸®½ºÆ®
	WORD	m_wCurMotion;			// ÇöÀç µ¿ÀÛ


	DWORD	m_dwStartTime;
	DWORD	m_dwGreetTime;
	DWORD	m_dwCurTime;

	BOOL	m_bGreetCheck;

public:
	cHelper();
	virtual ~cHelper();

	virtual void Render();
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	void AddSprite( WORD wIdx, cImage* sprite, WORD delay );
	void SetMaxSprite( WORD wIdx, int nMaxNum );

	void SetMotion( HELPER_MOTION Idx );
	WORD GetMotion() { return m_wCurMotion; }

	void SetStartTime(DWORD time) { m_dwStartTime = m_dwCurTime = time; m_bGreetCheck = TRUE; }
	void SetGreetTime(DWORD time) { m_dwGreetTime = time; }
	DWORD GetGreetTime() { return m_dwGreetTime; }
	
	BOOL IsGreetCheck() { return m_bGreetCheck; }
	void StopGreetCheck() { m_bGreetCheck = FALSE; }
};

#endif //_HELPER_