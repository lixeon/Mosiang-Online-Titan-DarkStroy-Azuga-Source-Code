#include "stdafx.h"
#include "Helper.h"
#include "HelperManager.h"

cHelper::cHelper()
{
	m_wCurMotion = 0; // ±âº» µ¿ÀÛ
	for(int i=0;i<emHM_MAX;++i)
	{
		m_MotionList[i].SetActive(FALSE);
		m_MotionList[i].Stop();
		m_MotionList[i].SetCurSpriteIdx(0);
	}

	m_dwStartTime = 0;
	m_dwGreetTime = 0;
	m_dwCurTime = 0;

	m_bGreetCheck = TRUE;
//	m_bGreetCheck = FALSE;
}

cHelper::~cHelper()
{
}

void cHelper::SetMotion( HELPER_MOTION Idx )
{
	// cAniÀÇ Active»óÅÂ¸¦ º¯°æÇÑ´Ù.
	 m_MotionList[m_wCurMotion].SetActive(FALSE);
	 m_MotionList[m_wCurMotion].Stop();
	 m_MotionList[m_wCurMotion].SetCurSpriteIdx(0);

	 m_MotionList[Idx].SetActive(TRUE);
	 m_MotionList[Idx].Play();

	// µ¿ÀÛ¹øÈ£ º¯°æ
	m_wCurMotion = Idx;
}

void cHelper::SetMaxSprite( WORD wIdx, int nMaxNum )
{
	m_MotionList[wIdx].SetMaxSprite( nMaxNum );
}

void cHelper::AddSprite( WORD wIdx, cImage* sprite, WORD delay )
{
	m_MotionList[wIdx].AddSprite( sprite, delay );
	m_MotionList[wIdx].Init( (LONG)m_absPos.x, (LONG)m_absPos.y, (WORD)sprite->GetImageSrcSize()->x, (WORD)sprite->GetImageSrcSize()->y, NULL );
}

DWORD cHelper::ActionEvent(CMouse * mouseInfo)
{
	m_MotionList[m_wCurMotion].ActionEvent( mouseInfo );
/*
	if( m_bGreetCheck )
	{
		m_dwCurTime = gCurTime;
		
		if( m_dwGreetTime < m_dwCurTime - m_dwStartTime )
		{
			// º¯È­¸¦ ÁØ´Ù.
			HELPERMGR->ChangeSpeechDlg( 2, FALSE );
			m_bGreetCheck = FALSE;
		}
	}
*/
	return WE_NULL;
}

void cHelper::Render()
{
	if( !IsActive() ) return;
	
	m_MotionList[m_wCurMotion].Render();
}
