// cAnimationManager.h: interface for the cAnimationManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CANIMATIONMANAGER_H__CE141E86_8652_4CA2_BE5C_4C89BC4F3C6F__INCLUDED_)
#define AFX_CANIMATIONMANAGER_H__CE141E86_8652_4CA2_BE5C_4C89BC4F3C6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cLinkedList.h"

class cWindow;
class cDialog;

enum ESHAKETYPE
{
	SHAKE_NORMAL_TYPE,
	SHAKE_NORMAL2_TYPE,
	SHAKE_TYPE_MAX,
};

class ANISHAKEVECTOR
{
public:
	VECTOR2 varVector;
	DWORD	Duration;
};
class ANISHAKEINFO
{
public:
	void SetParam( ANISHAKEVECTOR  v [], DWORD max )
	{
		pV = v; nN = max;
	}
	ANISHAKEVECTOR *	pV;
	DWORD				nN;
};
ANISHAKEVECTOR normallShake[];

enum { ANI_FADE_IN, ANI_FADE_OUT };

#define ANIMGR	USINGTON(cAnimationManager)

class cAnimationManager  
{
	static WORD MAG_GAB;
public:
	cAnimationManager();
	virtual ~cAnimationManager();
	void Init();

	// ¿Ü°¢ µµÅ·éÄ
	static BOOL MagProcess(RECT * dockingBarRect, cWindow* window, VECTOR2 * outWindowPos);

	// Shake éÄ	
	void StartShake(ESHAKETYPE shakeType, cWindow * window);
	void ShakeProcess();
	static ANISHAKEINFO m_SHAKEMODE[SHAKE_TYPE_MAX];

	// FadeIn/Out éÄ
	void StartFade(BYTE fadeType, short fadeStart, short fadeEnd, DWORD fadeTime, cDialog * pWnd);
	void FadeProcess();
	void EndFadeAll();
protected:

	void RollBack();
	BOOL FindFadeNode(cDialog * window);
	
	ESHAKETYPE		m_shakeType;
	DWORD			m_dwLastShakingPosChangedTime;
	LONG			m_OrgX;
	LONG			m_OrgY;
	WORD			m_CurIdx;
	BOOL			m_bShaking;
	cWindow *		m_pWindow;

	class FADE_INFO
	{
	public:
		BYTE fadeType;
		short fadeStartValue;
		short fadeEndValude;
		//WORD fadeTotalCount; 
		//WORD fadeCurCount;
		DWORD fadeTime;
		DWORD fadeStartTime;

		cDialog * pWnd;
		BOOL bEnd;
	};
	cLinkedList<FADE_INFO> m_FadeList;
	
};
EXTERNGLOBALTON(cAnimationManager);
#endif // !defined(AFX_CANIMATIONMANAGER_H__CE141E86_8652_4CA2_BE5C_4C89BC4F3C6F__INCLUDED_)
