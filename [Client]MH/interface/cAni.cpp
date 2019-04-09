// cAni.cpp: implementation of the cAni class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cAni.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cAni::cAni()
{
	m_type = WT_ANI;
	m_pSpriteImage = NULL;
	m_fLoop = TRUE;
	m_wTravelingType = 0;
	_m_fInverse = FALSE;
	_m_LastTick = _m_CurTick = 0;
	_m_CurSpriteIdx = 0;
	m_fAniTransactionCompleted = FALSE;
}

cAni::~cAni()
{
	if(m_pSpriteImage)
	delete [] m_pSpriteImage;
	if(m_pwSpriteDelay)
	delete [] m_pwSpriteDelay;
}

DWORD cAni::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if(!m_bActive) return we;

	if(m_fAniTransactionCompleted) return we;

	_m_CurTick = (GetTickCount());
	if(_m_CurTick - _m_LastTick > m_pwSpriteDelay[_m_CurSpriteIdx])
	{
//		_m_LastTick = m_pwSpriteDelay[_m_CurSpriteIdx];
		_m_LastTick = _m_CurTick;
		if(m_wTravelingType == 0)
		{
			if(++_m_CurSpriteIdx >= m_wMaxSprite)
				if(m_fLoop)
				{
					//12345 12345 12345
					_m_CurSpriteIdx = 0;
				}
				else
				{
					// 12345
					_m_CurSpriteIdx = 0;
					m_fAniTransactionCompleted = TRUE;
				}
		}
		else
		{
			//12345 54321
			if(_m_fInverse)
			{
				if(--_m_CurSpriteIdx <= 0)
					if(m_fLoop)
					{
						_m_CurSpriteIdx = 0;
						_m_fInverse = FALSE;
					}	
					else
					{
						_m_CurSpriteIdx = 0;
						_m_fInverse = FALSE;
						m_fAniTransactionCompleted = TRUE;
					}
			}
			else
			{
				if(++_m_CurSpriteIdx >= m_wMaxSprite)
				{
					_m_CurSpriteIdx = m_wMaxSprite-1;
					_m_fInverse = TRUE;
				}	
				
			}
		}
				
	}
	return we;
}

void cAni::Render()
{
	if(!m_bActive) return;
	//if(m_pSpriteImage[_m_CurSpriteIdx])
	m_pSpriteImage[_m_CurSpriteIdx].RenderSprite(NULL,NULL,0,&m_absPos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
}

void cAni::SetMaxSprite(WORD maxSprite)
{
	m_wMaxSprite = maxSprite;
	m_pSpriteImage = new cImage[m_wMaxSprite];
	m_pwSpriteDelay = new WORD[m_wMaxSprite];
	for(int i = 0 ; i < m_wMaxSprite ; i++)
	{
		m_pwSpriteDelay[i] = 0;
	}
}

void cAni::AddSprite(cImage * sprite, WORD delay)
{
	for(int i = 0 ; i < m_wMaxSprite ; i++)
	{
		if(!m_pwSpriteDelay[i])
		{
			m_pSpriteImage[i] = *sprite;
			m_pwSpriteDelay[i] = delay;
			return;
		}
	}
	ASSERT(0);
}