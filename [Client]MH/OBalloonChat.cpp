// OBalloonChat.cpp: implementation of the COBalloonChat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OBalloonChat.h"
#include "./interface/cFont.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define BALLOON_BORDER		10

COBalloonChat::COBalloonChat()
{
//	m_fgColor			= RGB_HALF( 0, 0, 200 );
	m_fgColor			= RGB_HALF(70,70,70);
	m_lTall				= 0;
	m_wFontIdx			= 0;
	m_dwStartTime		= 0;
	m_dwDisappearTime	= 0;
	m_bShow				= FALSE;
	m_bActive			= TRUE;
	InitChatMsg();
}

COBalloonChat::~COBalloonChat()
{

}


void COBalloonChat::InitChatMsg()
{
	for( int i = 0 ; i < MAX_CHATBALLON_TEXT_LINE ; ++i )
	{
		m_szChatMsg[i][0] = 0;	//NULL ½ºÆ®¸µ ³Ö±â.
		m_ChatMsgLen[i] = 0;
	}

	m_nLineNum = 0;
}

LONG COBalloonChat::GetBalloonHeight()
{
	return m_nLineNum * 15 + 2 + BALLOON_BORDER * 2;
}

void COBalloonChat::SetDisappearTime(DWORD disappearTime)
{ 
	m_dwDisappearTime	= disappearTime; 
	m_dwStartTime		= gCurTime; 
}

void COBalloonChat::SetChat(char * chatMsg)
{
	if( chatMsg == NULL )	return;
	if( *chatMsg == 0 )		return;


	int nLen = strlen( chatMsg );
	if( nLen > MAX_CHAT_LENGTH ) return;

//	char buf[128];
	int nCpyNum = 0;

	m_nLineNum = 0;

#ifdef _TL_LOCAL_
	BYTE brk[512];
	int nBrkNum = g_TBreak.FindThaiWordBreak( chatMsg, nLen, brk, 512, FTWB_SEPARATESYMBOL );
	int nCutPos = 0;
	int nCount = 0;

	for( int i = 0 ; i < nBrkNum ; ++i )
	{
		++nCount;
		nCutPos += brk[i];
		if( CFONT_OBJ->GetTextExtentEx(0, chatMsg, nCutPos ) > MAX_CHATBALLON_TEXT_EXTENT )
		{
			if( nCount == 1 )	//한단어만 썼는데 넘어가버리면 강제로 끊는다.
			{
				for( int k = 1 ; k <= nCutPos ; ++k )
				{
					if( CFONT_OBJ->GetTextExtentEx(0, chatMsg, k ) > MAX_CHATBALLON_TEXT_EXTENT )
					{
						nCutPos = nCutPos - brk[i] + k-1; //쓴 글자까지만
						strncpy( m_szChatMsg[m_nLineNum], chatMsg, nCutPos );	//전단계까지
						m_szChatMsg[m_nLineNum][nCutPos] = 0;
						m_ChatMsgLen[m_nLineNum] = nCutPos;

						chatMsg += nCutPos;
						brk[i] -= nCutPos;	//나머지부분 계산
						nCutPos = 0;
						nCount = 0; //초기화
						--i;
						break;
					}
				}
			}
			else
			{
				nCutPos -= brk[i];
				strncpy( m_szChatMsg[m_nLineNum], chatMsg, nCutPos );	//전단계까지

				m_szChatMsg[m_nLineNum][nCutPos] = 0;
				m_ChatMsgLen[m_nLineNum] = nCutPos;

				--i;
				chatMsg += nCutPos;
				nCutPos = 0;
				nCount = 0; //초기화
			}

			if( *chatMsg == ' ' )
			{
				++chatMsg;
				++i;
			}

			if( ++m_nLineNum >= MAX_CHATBALLON_TEXT_LINE )
			{
//				nCutPos = 0;
				break;
			}
		}
	}

	if( nCutPos > 0 && m_nLineNum < MAX_CHATBALLON_TEXT_LINE )
	{
		strncpy( m_szChatMsg[m_nLineNum], chatMsg, nCutPos );
		m_szChatMsg[m_nLineNum][nCutPos] = 0;
		m_ChatMsgLen[m_nLineNum] = nCutPos;
		++m_nLineNum;
	}

	int Width = 0;
	int strPixelLen = 0;
	for( int i = 0 ; i < m_nLineNum ; ++i )
	{
		Width = CFONT_OBJ->GetTextExtentEx(m_wFontIdx, m_szChatMsg[i], m_ChatMsgLen[i]);
		if( Width > strPixelLen )
			strPixelLen = Width;
	}
	SetPosX(-strPixelLen/2);


#else
	while( nLen > MAX_CHATBALLON_TEXT_LENGTH )
	{
		if( ( chatMsg + MAX_CHATBALLON_TEXT_LENGTH ) != CharNext( CharPrev( chatMsg, chatMsg + MAX_CHATBALLON_TEXT_LENGTH ) ) )
			nCpyNum = MAX_CHATBALLON_TEXT_LENGTH - 1;
		else
			nCpyNum = MAX_CHATBALLON_TEXT_LENGTH;

		strncpy( m_szChatMsg[m_nLineNum], chatMsg, nCpyNum );
		m_szChatMsg[m_nLineNum][nCpyNum] = 0;

		m_ChatMsgLen[m_nLineNum] = nCpyNum;

		nLen -= nCpyNum;
		chatMsg += nCpyNum;
		if( *chatMsg == ' ' ) ++chatMsg;

		if( ++m_nLineNum >= MAX_CHATBALLON_TEXT_LINE-1 )
		{
//			nLen = 0;
			break;
		}
	}

	if( nLen > 0 )
	{
		strncpy( m_szChatMsg[m_nLineNum], chatMsg, MAX_CHATBALLON_TEXT_LENGTH );
		m_szChatMsg[m_nLineNum][MAX_CHATBALLON_TEXT_LENGTH] = 0;
		m_ChatMsgLen[m_nLineNum] = nLen;
		++m_nLineNum;
	}

	int strPixelLen = CFONT_OBJ->GetTextExtentEx(m_wFontIdx, m_szChatMsg[0], m_ChatMsgLen[0]);
	SetPosX(-strPixelLen/2);

#endif
	cBalloonOutline::SetBalloonSize(strPixelLen+8, 30, m_nLineNum);
}

void COBalloonChat::SetActive( BOOL val )
{
	m_bActive	= val;

	if( !val )
		Show( FALSE );
}

void COBalloonChat::Show( BOOL val )
{
	m_bShow		= val;

	if( !val )
	{
		m_dwStartTime = m_dwDisappearTime = 0;
	}
}

BOOL COBalloonChat::Render(LONG absX, LONG absY)
{
	if(!m_bActive) return FALSE;
	
	if( m_dwStartTime + m_dwDisappearTime > gCurTime )
	{
		m_bShow = TRUE;
	}
	else
	{
		m_dwStartTime = m_dwDisappearTime = 0;
		InitChatMsg();
		m_bShow = FALSE;
	}

	if(!m_bShow) return FALSE;

	if( m_nLineNum == 0 ) return FALSE;

	LONG lLineHeight = 15;
	LONG lMiddleTotalHeight = 2 + lLineHeight * m_nLineNum;
	LONG lPosX = absX + m_lPosX;
	LONG lPosY = absY + m_lTall - lMiddleTotalHeight - 10;	//2 = linespace , 10 = tail height

	// RaMa - 04.11.29  ->Ã¤ÆÃ°ú ³ëÁ¡»óÀº ÀÌ ÇÔ¼ö¸¦ »ç¿ëÇÑ´Ù.
	cBalloonOutline::RenderOutlineSimple( (float)lPosX, (float)lPosY, (float)absX, lMiddleTotalHeight, TRUE );
//	cBalloonOutline::RenderOutline( (float)lPosX, (float)lPosY, (float)absX, lMiddleTotalHeight );

	
	// ÅØ½ºÆ®
	RECT rect;
	for( int i = 0 ; i < m_nLineNum ; ++i )
	{
		if( m_szChatMsg[i][0] != 0 )
		{
			SetRect( &rect, (long)(lPosX + BALLOON_BORDER),
							(long)(lPosY + BALLOON_BORDER + lLineHeight * i ), 1, 1 );
			CFONT_OBJ->RenderFont(m_wFontIdx, m_szChatMsg[i], m_ChatMsgLen[i], &rect, RGBA_MERGE( m_fgColor,/*RGB_HALF(70,70,70),*/ 255 ) );
		}	
	}

//	RECT rect = {(long)(absX + m_lPosX + BALLOON_BORDER), (long)(absY + m_lTall + BALLOON_BORDER), 1, 1};
//	if( *m_szChatMsg != 0 )
//	{
//		CFONT_OBJ->RenderFont(m_wFontIdx, m_szChatMsg, m_ChatMsgLen, &rect, RGBA_MERGE( RGB_HALF(70,70,70), 255 ));
//		//CFONT_OBJ->RenderFont(m_wFontIdx, m_szChatMsg, m_ChatMsgLen, &rect, RGBA_MERGE( m_fgColor, 255 ));
//	}

	return TRUE;
}
