// cAnimationManager.cpp: implementation of the cAnimationManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cAnimationManager.h"
#include "cWindow.h"
#include "cDialog.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
ANISHAKEVECTOR normallShake[] =
{
	{	0.f,	10.f,	50	},
	{	0.f,	-10.f,	50	},
	{	0.f,	20.f,	50	},
	{	0.f,	-20.f,	50	},
	{	0.f,	15.f,	50	},
	{	0.f,	-15.f,	50	},
	{	0.f,	10.f,	50	},
	{	0.f,	-10.f,	50	},
	{	0.f,	5.f,	50	},
	{	0.f,	-5.f,	50	},
	{	0.f,	2.f,	50	},
	{	0.f,	-2.f,	50	},
	{	0.f,	1.f,	50	},
	{	0.f,	-1.f,	50	},
};
ANISHAKEVECTOR normall2Shake[] =
{
	{	0.f,	10.f,	30	},
	{	0.f,	-10.f,	30	},
	{	0.f,	20.f,	30	},
	{	0.f,	-20.f,	30	},
	{	0.f,	15.f,	30	},
	{	0.f,	-15.f,	30	},
	{	0.f,	10.f,	30	},
	{	0.f,	-10.f,	30	},
	{	0.f,	5.f,	30	},
	{	0.f,	-5.f,	30	},
	{	0.f,	2.f,	30	},
	{	0.f,	-2.f,	50	},
	{	0.f,	1.f,	50	},
	{	0.f,	-1.f,	50	},
};
WORD cAnimationManager::MAG_GAB = 10;
ANISHAKEINFO cAnimationManager::m_SHAKEMODE[];
cAnimationManager::cAnimationManager()
{
	m_OrgX					= 0;
	m_OrgY					= 0;	
	m_CurIdx				= 0;
	m_dwLastShakingPosChangedTime	= 0;
	m_bShaking				= FALSE;
	m_pWindow				= NULL;
}

GLOBALTON(cAnimationManager);
cAnimationManager::~cAnimationManager()
{
	EndFadeAll();
}
void cAnimationManager::Init()
{
	m_SHAKEMODE[SHAKE_NORMAL_TYPE].SetParam(normallShake, sizeof(normallShake)/sizeof(ANISHAKEVECTOR));
	m_SHAKEMODE[SHAKE_NORMAL2_TYPE].SetParam(normall2Shake, sizeof(normall2Shake)/sizeof(ANISHAKEVECTOR));
}
void cAnimationManager::RollBack()
{
	m_bShaking = FALSE;
	if( m_pWindow )
	{
		m_pWindow->SetAbsXY(m_OrgX, m_OrgY);
		m_pWindow = NULL;
	}
}
void cAnimationManager::StartShake(ESHAKETYPE shakeType, cWindow * window)
{
	RollBack();

	m_shakeType = shakeType;
	m_OrgX		= (long)window->GetAbsX();
	m_OrgY		= (long)window->GetAbsY();
	m_CurIdx	= 0;
	m_pWindow	= window;
	m_bShaking	= TRUE;
	m_dwLastShakingPosChangedTime = gCurTime;
}

void cAnimationManager::ShakeProcess()
{
	if(!m_bShaking) return;

	ANISHAKEVECTOR * shV = &m_SHAKEMODE[m_shakeType].pV[m_CurIdx];

	if(gCurTime - m_dwLastShakingPosChangedTime >= shV->Duration)
	{
		m_dwLastShakingPosChangedTime = gCurTime;
		if( ++m_CurIdx >= m_SHAKEMODE[m_shakeType].nN )
		{
			m_bShaking	= FALSE;
			m_pWindow->SetAbsXY(m_OrgX, m_OrgY);
			return;
		}
	}

	m_pWindow->SetAbsXY((long)(m_OrgX + shV->varVector.x), (long)(m_OrgY + shV->varVector.y));

}

BOOL cAnimationManager::MagProcess(RECT * dockingBarRect, cWindow* window, VECTOR2 * outWindowPos)
{
	BOOL rt = FALSE;
	if( abs( (int)(dockingBarRect->left - (LONG)(window->GetAbsX()) ) ) <= MAG_GAB )
	{
		outWindowPos->x = (float)dockingBarRect->left;
		rt = TRUE;
	}
	else if(abs((int)(dockingBarRect->left - (window->GetAbsX()+window->GetWidth()))) <= MAG_GAB)
	{
		outWindowPos->x = (float)dockingBarRect->left-window->GetWidth();
		rt = TRUE;
	}

	if(abs((int)(dockingBarRect->right - window->GetAbsX())) <= MAG_GAB)
	{
		outWindowPos->x = (float)dockingBarRect->right;
		rt = TRUE;
	}
	else if(abs((int)(dockingBarRect->right - (window->GetAbsX()+window->GetWidth()))) <= MAG_GAB)
	{
		outWindowPos->x = (float)(dockingBarRect->right - window->GetWidth());
		rt = TRUE;
	}

	if(abs((int)(dockingBarRect->top - window->GetAbsY())) <= MAG_GAB)
	{
		outWindowPos->y = (float)dockingBarRect->top;
		rt = TRUE;
	}
	else if(abs((int)(dockingBarRect->top - (window->GetAbsY()+window->GetHeight()))) <= MAG_GAB)
	{
		outWindowPos->y = (float)dockingBarRect->top-window->GetHeight();
		rt = TRUE;
	}
	
	if(abs((int)(dockingBarRect->bottom - window->GetAbsY())) <= MAG_GAB)
	{
		outWindowPos->y = (float)dockingBarRect->bottom;
		rt = TRUE;
	}
	else if(abs((int)(dockingBarRect->bottom - (window->GetAbsY()+window->GetHeight()))) <= MAG_GAB)
	{
		outWindowPos->y = (float)(dockingBarRect->bottom - window->GetHeight());
		rt = TRUE;
	}

//KES 031017 CAPTION이 밖으로 나가지 못하도록
	RECT rc = *((cDialog*)window)->GetCaptionRect();
	OffsetRect( &rc, (int)window->GetAbsX(), (int)window->GetAbsY() );

	if( rc.left < dockingBarRect->left )
	{
		outWindowPos->x = (float)dockingBarRect->left;
		rt = TRUE;
	}
	else if( rc.right > dockingBarRect->right )
	{
		outWindowPos->x = (float)(dockingBarRect->right - ( rc.right - rc.left ));
		rt = TRUE;
	}
	if( rc.top < dockingBarRect->top )
	{
		outWindowPos->y = (float)dockingBarRect->top;
		rt = TRUE;
	}
	else if( rc.bottom > dockingBarRect->bottom )
	{
		outWindowPos->y = (float)(dockingBarRect->bottom - ( rc.bottom - rc.top ));
		rt = TRUE;
	}

	return rt;
}

void cAnimationManager::StartFade(BYTE fadeType, short fadeStart, short fadeEnd, DWORD fadeTime, cDialog * pWnd)
{
	if(FindFadeNode(pWnd))
	{
		return ;
	}
	FADE_INFO * pInfo		= new FADE_INFO;
	
	pInfo->fadeType					= fadeType;
	pInfo->fadeStartValue			= fadeStart;
	pInfo->fadeEndValude			= fadeEnd;
	//pInfo->fadeTotalCount			= fadeCount; 
	//pInfo->fadeCurCount				= 0;
	pInfo->fadeTime					= fadeTime;
	//for(int i = 0 ; i < fadeCount ; ++i)
	//{
	//	pInfo->fadeTime[i] = fadeTime[i];
	//}
	pInfo->fadeStartTime			= gCurTime;
	pInfo->pWnd						= pWnd;
	pInfo->bEnd						= FALSE;
	if(pInfo->fadeType == ANI_FADE_IN)
	{
		pInfo->pWnd->SetAlpha((BYTE)pInfo->fadeStartValue);
		pInfo->pWnd->SetActive(TRUE);
		pInfo->pWnd->SetDisable(TRUE);
	}
	m_FadeList.AddTail(pInfo);
	
}
BOOL cAnimationManager::FindFadeNode(cDialog * window)
{
	FADE_INFO * pInfo;
	POS pos = m_FadeList.GetFirstPos();
	while(pInfo = m_FadeList.GetNextPos(pos))
	{
		if(pInfo->pWnd == window)
			return TRUE;
	}
	return FALSE;
}
void cAnimationManager::FadeProcess()
{
	FADE_INFO * pInfo;
	POS pos = m_FadeList.GetFirstPos();
	while(pInfo = m_FadeList.GetNextPos(pos))
	{
		DWORD dwElapsed = gCurTime - pInfo->fadeStartTime;
		if(dwElapsed > pInfo->fadeTime)
		{
			pInfo->pWnd->SetAlpha((BYTE)pInfo->fadeEndValude);
			if(pInfo->fadeType == ANI_FADE_IN)
			{
				pInfo->pWnd->SetActive(TRUE);
				pInfo->pWnd->SetDisable(FALSE);
			}
			else
			{
				pInfo->pWnd->SetAlpha(255);
				pInfo->pWnd->SetActive(FALSE);
			}
			pInfo->bEnd = TRUE;
		}
		else
		{
			pInfo->pWnd->SetAlpha( (BYTE)(pInfo->fadeStartValue + ((float)dwElapsed * (float)(pInfo->fadeEndValude- pInfo->fadeStartValue) / (float)pInfo->fadeTime )) );
		}
	}

	for(int i  = 0 ; i < m_FadeList.GetCount() ; ++i)
	{
		pInfo = m_FadeList.GetAt(i);
		if(pInfo->bEnd == TRUE)
		{
			m_FadeList.DeleteAt(i);
			break;
		}
	}
}

void cAnimationManager::EndFadeAll()
{
	POS pos = m_FadeList.GetFirstPos();
	while( FADE_INFO* pFI = m_FadeList.GetNextPos( pos ) )
	{
		delete pFI;
	}
	m_FadeList.DeleteAll();
}
