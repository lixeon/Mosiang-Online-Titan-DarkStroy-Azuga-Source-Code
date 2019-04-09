#include "stdafx.h"
#include "AutoAnswerDlg.h"
#include "WindowIDEnum.h"

#include "./Interface/cTextArea.h"
#include "./Interface/cStatic.h"
#include "./Interface/cButton.h"

#include "AutoNoteManager.h"
#include "ChatManager.h"


CAutoAnswerDlg::CAutoAnswerDlg(void)
{
}

CAutoAnswerDlg::~CAutoAnswerDlg(void)
{
}

void CAutoAnswerDlg::Linking()
{
	m_dwEndTime	= 0;

	m_pTextAreaDesc	= (cTextArea*)GetWindowForID( ASD_TEXTAREA_DESC );
	m_pStcQuestion = (cStatic*)GetWindowForID( ASD_STC_QUESTION );
	m_pStcAnswer = (cStatic*)GetWindowForID( ASD_STC_ANSWER );
	m_pStcTime = (cStatic*)GetWindowForID( ASD_STC_TIME );	

	m_pTextAreaDesc->SetScriptText( CHATMGR->GetChatMsg( 1722 ) );
	m_pTextAreaDesc->SetTextColor( RGB_HALF(128,128,128) );

	for( int i = 0 ; i < 4 ; ++i )
	{
		m_pBtnColor[i]		= (cButton*)GetWindowForID( ASD_BTN_COLOR1 + i );
		m_v2BtnPos[i].x		= m_pBtnColor[i]->GetRelX();
		m_v2BtnPos[i].y		= m_pBtnColor[i]->GetRelY();
	}

	m_bAnswerStart = FALSE;
	m_nAnswerPos = 0;
}

void CAutoAnswerDlg::SetActive( BOOL val )
{
	if( val == TRUE )
	{
		m_dwEndTime = gCurTime + 120*1000;	//120초
		m_Question.Release();
		m_Question.LoadSprite( "ANRaster.tga" );
	}
	else
	{
		m_Question.Release();
		DeleteFile( "ANRaster.tga" );
	}

	cDialog::SetActive( val );
}

void CAutoAnswerDlg::SetActiveWithTime( BOOL val, DWORD dwTime )
{
	if( val == TRUE )
	{
		m_dwEndTime = gCurTime + dwTime*1000;	//초
		m_Question.Release();
		m_Question.LoadSprite( "ANRaster.tga" );
	}
	else
	{
		m_Question.Release();
		DeleteFile( "ANRaster.tga" );
	}

	cDialog::SetActive( val );
}

DWORD CAutoAnswerDlg::ActionEvent( CMouse * mouseInfo )
{
	DWORD we = WE_NULL;
	if( m_bDisable ) return we;

	char buf[64];
	int nLimitTime = ((int)(m_dwEndTime - gCurTime)) / 1000;
	if( nLimitTime < 0 ) nLimitTime = 0;

	static int last = 0;
	if( last != nLimitTime )
	{
		wsprintf( buf, "%2d", nLimitTime );
		m_pStcTime->SetStaticText( buf );
		last = nLimitTime;
	}

	we = cDialog::ActionEvent( mouseInfo );
	return we;
}

void CAutoAnswerDlg::OnActionEvent( LONG lId, void * p, DWORD we )
{
//	if( m_bAnswerStart == FALSE ) return;
	if( m_nAnswerPos > 3 ) return;

	if( we & WE_BTNCLICK )
	{
		if( lId >= ASD_BTN_COLOR1 && lId <= ASD_BTN_COLOR4 )
		{
			//			m_dwAnswer[m_nAnswerPos] = m_dwOriginalPos[lId - ASD_BTN_COLOR1];

			m_dwAnswer[m_nAnswerPos] = lId - ASD_BTN_COLOR1;

			char strColor[4][16] = { "  *  ", "  *  ", "  *  ", "  *  " };
			char buf[128] = {0,};			
			wsprintf( buf, "%s %s", m_pStcAnswer->GetStaticText(), strColor[lId-ASD_BTN_COLOR1] );
			m_pStcAnswer->SetStaticText( buf );

			++m_nAnswerPos;
			if( m_nAnswerPos == 4 )
			{
				AUTONOTEMGR->AnswerToQuestion( m_dwAnswer[0], m_dwAnswer[1], m_dwAnswer[2], m_dwAnswer[3] );
				m_bAnswerStart = FALSE;
			}
		}
	}
}

void CAutoAnswerDlg::Shuffle( DWORD* pColorTable )
{
	/*
	for( int i = 0 ; i < 4 ; ++i )
	{
	m_dwOriginalPos[pColorTable[i]] = i;
	m_pBtnColor[i]->SetRelXY( m_v2BtnPos[pColorTable[i]].x, m_v2BtnPos[pColorTable[i]].y );
	}
	*/	
	int randY = rand() % 121 - 60; //--- -60~60 까지 렌덤
	SetAbsXY( GetAbsX(), GetAbsY() + randY );	//---해주어야 리프레쉬되네

}

void CAutoAnswerDlg::SetQuestion( char* strQuestion )
{
	if( m_pStcQuestion )
		m_pStcQuestion->SetStaticText( strQuestion );

	m_bAnswerStart = TRUE;
	m_nAnswerPos = 0;

	m_pStcAnswer->SetStaticText( "" );
}

void CAutoAnswerDlg::Retry()
{
	m_bAnswerStart = TRUE;
	m_nAnswerPos = 0;
	m_pStcAnswer->SetStaticText( "" );	
}

void CAutoAnswerDlg::SaveImage( BYTE* pRaster )
{
	char filename[256];
	sprintf( filename, "ANRaster.tga" );

	WriteTGA( filename, (char*)pRaster, 128, 32, 3*128, 24 );
}

void CAutoAnswerDlg::Render()
{
	cDialog::Render();

	if( IsActive() )
	{
		VECTOR2 scale, pos;
		scale.x = 1.0f;
		scale.y = 1.0f;
		pos.x = GetAbsX() + 70;
		pos.y = GetAbsY() + 180;

		m_Question.RenderSprite( &scale, NULL, 0.f, &pos, 0xffffffff );
	}
}

void CAutoAnswerDlg::SetActiveTestClient()
{

}
