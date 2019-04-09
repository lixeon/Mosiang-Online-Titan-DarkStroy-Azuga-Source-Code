// BGMManager.cpp: implementation of the CBGMManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MHAutoPatch.h"
#include "BGMManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBGMManager::CBGMManager()
{
	m_pSoundLib = NULL;
}

CBGMManager::~CBGMManager()
{

}


BOOL CBGMManager::CreateSound( HWND hWnd )
{
	// »ç¿îµå ¶óÀÌºê·¯¸® »ý¼º.
	if( CreateSoundLib( &m_pSoundLib ) == FALSE )
	{
		return FALSE;
	}
	
	// ÃÊ±âÈ­. ÆÄÀÏ ÃÖ´ë °¹¼ö 100°³, ¼Ò¸® °´Ã¼ 200°³, eax ½Ãµµ.
	SOUND_ERROR_CODE	sResult	=	m_pSoundLib->Init( 3, 3, "miles", FALSE, TRUE);
	
//	m_pSoundLib->Set3DSpeakerType(SOUND_SPEAKER_TYPE_HEADPHONE);

//	JSD - ÀÏ´Ü µÎ°¡Áö °æ¿ì¸¸ Ã¼Å©ÇÔ
//	¸¶ÀÏÁî ÃÊ±âÈ­¿¡ ½ÇÆÐÇÏ¸é SOUND_ERROR_MILES_STARTUP_FAILED,
//	BGMµå¶óÀÌ¹ö¸¦ Ã£Áö ¸øÇÒ °æ¿ì, »ç¿îµåÄ«µå°¡ ¾ø´Ù¸é SOUND_ERROR_BGMDRIVER_STARTUP_FAILED,
	if( sResult == SOUND_ERROR_BGMDRIVER_STARTUP_FAILED ||	
		sResult == SOUND_ERROR_MILES_STARTUP_FAILED )
	{
		ReleaseSound();
		return FALSE;
	}

	return TRUE;
}

void CBGMManager::ReleaseSound()
{
	if( m_pSoundLib )
	{
		m_pSoundLib->Delete();
		m_pSoundLib = NULL;
	}
}


void CBGMManager::PlayBGM( char* pFileName )
{
	if( m_pSoundLib )
	{
		SOUND_ERROR_CODE sResult	= m_pSoundLib->PlayBGM( pFileName );
		m_pSoundLib->SetBGMVolume( 1 );
	}
}


void CBGMManager::StopBGM()
{
	if( m_pSoundLib )
		m_pSoundLib->StopBGM();
}

