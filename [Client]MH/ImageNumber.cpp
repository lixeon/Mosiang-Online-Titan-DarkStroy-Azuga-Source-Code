#include "stdafx.h"
#include "ImageNumber.h"
#include "./interface/cScriptManager.h"

CImageNumber::CImageNumber()
{
	m_vPos.x		= 0;
	m_vPos.y		= 0;
	m_dwNumber		= 0;
	m_dwLimitCipher	= 9;	//¾ï
	m_bFillZero		= FALSE;
	
	m_nNumWidth		= 32;
	m_nGap			= 0;
}

CImageNumber::~CImageNumber()
{

}

void CImageNumber::Init( int nNumWidth, int nGap )
{
	for( int i = 0 ; i < 10 ; ++i )
	{
		//image_hard_path.txt ¿¡¼­ 49¹øºÎÅÍ »ç¿ë
		SCRIPTMGR->GetImage( 49 + i, &m_ImageNum[i], PFT_HARDPATH );
	}

	m_nNumWidth = nNumWidth;
	m_nGap		= nGap;
	m_bFadeOut	= FALSE;
	m_dwFadeOutStartTime = 0;
	m_bNumberChange = FALSE;
	m_vScale.x = 1.0f;
	m_vScale.y = 1.0f;
}


void CImageNumber::SetLimitCipher( DWORD dwCipher )
{
	if( dwCipher > 9 )	m_dwLimitCipher = 9;
	else				m_dwLimitCipher = dwCipher;
}

void CImageNumber::SetNumber( DWORD dwNum )
{
	DWORD dwLimit = (DWORD)pow( 10, m_dwLimitCipher ) - 1;

	if( dwNum != m_dwNumber )
		m_bNumberChange = TRUE;
	else
		m_bNumberChange = FALSE;

	if( dwNum > dwLimit )
		m_dwNumber = 0;
	else
		m_dwNumber = dwNum;
}

//right, Top...
void CImageNumber::SetPosition( int x, int y )
{
	m_vPos.x = (float)x;
	m_vPos.y = (float)y;
}

void CImageNumber::Render()
{
////ÀÓ½Ã ¾Ö´Ï
	DWORD dwAlpha = 255;
	if( m_bFadeOut )
	{
		DWORD dwElapsed = gCurTime - m_dwFadeOutStartTime;
		DWORD dwMinusAlpha = 255 * dwElapsed / m_dwFadeOutTime;
		if( dwMinusAlpha < 255 )
		{
			dwAlpha = 255 - dwMinusAlpha;
		}
		else
		{
			dwAlpha = 0;
		}
	}
//

	VECTOR2 vPos;

	vPos.x = m_vPos.x - m_nNumWidth;
	vPos.y = m_vPos.y;

	int nNum = m_dwNumber;
	int nChar;

	for( DWORD i = 0 ; i < m_dwLimitCipher ; ++i )
	{
		nChar = nNum % 10;
		nNum /= 10;
		m_ImageNum[nChar].RenderSprite( &m_vScale, NULL, 0.0f, &vPos, 0x00ffffff | (dwAlpha<<24)  );

		if( !m_bFillZero && nNum == 0 ) break;

		vPos.x -= m_nNumWidth + m_nGap;
	}
}


void CImageNumber::SetFadeOut( DWORD dwTime ) 
{
	if( dwTime == 0 )
	{
		m_bFadeOut			= FALSE;
	}
	else
	{
		m_bFadeOut			= TRUE;
		m_dwFadeOutTime		= dwTime; 
		m_dwFadeOutStartTime = gCurTime;
	}
}


