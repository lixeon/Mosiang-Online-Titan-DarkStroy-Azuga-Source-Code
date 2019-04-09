// ObjectGuagen.cpp: implementation of the CObjectGuagen class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ObjectGuagen.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CObjectGuagen::CObjectGuagen()
{
	m_type = WT_GUAGENE;
	m_fGuageEffectPieceWidth	= 0.f;
	m_fIncAmount				= 0.f;
	m_dwEffectTime				= 0;
	m_dwStartTime				= 0;
	m_fOldPercentRate			= 0.f;
	m_fCurPercentRate			= 0.f;
	m_bBlink					= FALSE;
	m_dwStartBlinkTime			= 0;
	m_fGuageEffectPieceHeightScaleY	= 1.f;
}

CObjectGuagen::~CObjectGuagen()
{

}
void CObjectGuagen::SetValue( GUAGEVAL val, DWORD estTime )
{
	//GuageOverCheck
	if(val > 1)
		val = 1;

	// ¿¡³ÊÁö°¡ ÀÏÁ¤ ÀÌÇÏ·Î ¶³¾î Áö¸é ¹øÂ½
	// ¸¶ÀÌ³Ê½º °¨¼Ò´Â Èçµé¸²
	if( m_fCurPercentRate > m_fOldPercentRate )
	{
//		m_ani.Init();
//		m_ani.StartShake(SHAKE_NORMAL2_TYPE, this);
//		m_bBlink = TRUE;
//		m_dwStartBlinkTime = gCurTime;
	}
	

	m_fOldPercentRate	= m_fCurPercentRate;
	m_dwEffectTime		= estTime;
	if( m_dwEffectTime )
	{
		m_fIncAmount	= ( val - m_fOldPercentRate ) / m_dwEffectTime;	// 1ÃÊ¿¡ º¯È­µÇ¾î¾ß ÇÒ ¾ç
		m_dwStartTime	= gCurTime;
	}
	else
	{
		m_fCurPercentRate = val;
	}

	cGuagen::SetValue(val);
}
DWORD CObjectGuagen::ActionEvent(CMouse * mouseInfo)
{
	cGuagen::ActionEvent(mouseInfo);

//	if( m_bActive ) m_ani.ShakeProcess();

	return WE_NULL;
}
void CObjectGuagen::Render()
{
	cGuagen::Render();

//	if(m_ID == CG_GUAGELIFE)
//	{
//		int a = 3;
//	}
	if(m_bActive)
	{

		VECTOR2 imgPosRect = { m_absPos.x + m_imgRelPos.x, m_absPos.y + m_imgRelPos.y };
		VECTOR2 scaleRate;
		float per = m_fPercentRate;
		if( m_dwEffectTime )
		{		
			if( gCurTime - m_dwStartTime < m_dwEffectTime )
			{
				m_fCurPercentRate = ( m_fOldPercentRate + ( gCurTime - m_dwStartTime ) * m_fIncAmount );
				per = m_fCurPercentRate;

				if( per > 1.0 ) // ÀÌ°Å ÀÌ·¸°Ô ÇÏ¸é ´Ù¸¥°÷¿¡¼­ ¹®Á¦ ¾ø³ª??
					per = 1.0;
			}
			else
			{
				m_fCurPercentRate = per;
				m_dwEffectTime	= 0;
				m_dwStartTime	= 0;
			}
		}
		scaleRate.x = m_fGuageWidth * per / m_fGuageEffectPieceWidth;
		scaleRate.y = m_fGuageEffectPieceHeightScaleY;
		if( m_bBlink )
		{
			if( gCurTime - m_dwStartBlinkTime < 4000)
			{
				static BOOL ura = FALSE;
				static DWORD tick = gCurTime;
				
				if( gCurTime - tick > 10 )
				{
					ura ^= TRUE;	
					tick = gCurTime;
				}
				if(!ura)
				{
					m_GuageEffectPieceImage.RenderSprite(&scaleRate, NULL, 0, &imgPosRect,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
				}
			}
			else
			{
				m_bBlink = FALSE;
			}
		}
		else
		{
			m_GuageEffectPieceImage.RenderSprite(&scaleRate, NULL, 0, &imgPosRect,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
		}
	}
}
