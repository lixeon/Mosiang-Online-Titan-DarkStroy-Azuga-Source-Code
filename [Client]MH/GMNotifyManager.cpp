// GMNotifyManager.cpp: implementation of the CGMNotifyManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GMNotifyManager.h"
#include "Interface/cFont.h"
#include "MainGame.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLOBALTON(CGMNotifyManager)

CGMNotifyManager::CGMNotifyManager()
{
	Init( MAX_NOTIFYMSG_LINE );
	SetPosition( NOTIFYMSG_DEFAULT_X, NOTIFYMSG_DEFAULT_Y, NOTIFYMSG_DEFAULT_WIDTH );
	SetLineSpace( NOTIFYMSG_DEFAULT_LINESPACE );
	SetFont( NOTIFYMSG_DEFAULT_FONT );

	memset( m_ApplyEventList, 0, sizeof(BOOL)*eEvent_Max );
}

CGMNotifyManager::~CGMNotifyManager()
{
	Release();
}

void CGMNotifyManager::Init( int nMaxLine )
{
	m_nMaxLine			= nMaxLine;

	m_bUseEventNotify	= FALSE;
	ZeroMemory( m_strEventNotifyTitle, 32 );
	ZeroMemory( m_strEventNotifyContext, 128 );

	m_bNotifyChanged	= FALSE;
//	m_bNotifyActive		= FALSE;
}

void CGMNotifyManager::Release()
{
	PTRLISTPOS pos = m_ListNotify.GetHeadPosition();
	while(pos)
	{
		sGMNOTIFY* pNotify = (sGMNOTIFY *)m_ListNotify.GetNext(pos);
		SAFE_DELETE(pNotify);
	}

	m_ListNotify.RemoveAll();
}

void CGMNotifyManager::PushMsg( char* pMsg, DWORD dwColor )
{
	sGMNOTIFY* pNotify = new sGMNOTIFY;
	
	pNotify->nStrLen = strlen(pMsg);

	if( pNotify->nStrLen > MAX_NOTIFYMSG_LENGTH )
	{
		strncpy( pNotify->strMsg, pMsg, MAX_NOTIFYMSG_LENGTH );
		pNotify->strMsg[MAX_NOTIFYMSG_LENGTH] = NULL;
		pNotify->nStrLen = MAX_NOTIFYMSG_LENGTH;
	}
	else
	{
		strcpy( pNotify->strMsg, pMsg );
	}

	pNotify->dwReceiveTime	= gCurTime;
	pNotify->dwColor		= dwColor;

	m_ListNotify.AddTail( pNotify );

	if( m_ListNotify.GetCount() > m_nMaxLine ) //max가 넘어가면 처음 메세지를 지운다.
		PopMsg();
}

void CGMNotifyManager::PopMsg()
{
	if( m_ListNotify.GetCount() )
	{
		sGMNOTIFY* pNotify = (sGMNOTIFY*)m_ListNotify.GetHead();
		SAFE_DELETE( pNotify );
		m_ListNotify.RemoveHead();
	}
}

void CGMNotifyManager::AddMsg( char* pMsg, int nClass )
{
	int nLen = strlen( pMsg );

	char buf[128];
	int nCpyNum = 0;

	while( nLen > MAX_NOTIFYMSG_PER_LINE )
	{
		if( ( pMsg + MAX_NOTIFYMSG_PER_LINE ) != CharNext( CharPrev( pMsg, pMsg + MAX_NOTIFYMSG_PER_LINE ) ) )
			nCpyNum = MAX_NOTIFYMSG_PER_LINE - 1;
		else
			nCpyNum = MAX_NOTIFYMSG_PER_LINE;

		strncpy( buf, pMsg, nCpyNum );
		buf[nCpyNum] = 0;
		PushMsg( buf, dwNotifyColor[nClass] );
		nLen -= nCpyNum;
		pMsg += nCpyNum;
		if( *pMsg == ' ' ) ++pMsg;	//라인 첫글자가 스페이스가 아니도록...
	}

	if( nLen > 0 )
		PushMsg( pMsg, dwNotifyColor[nClass] );	
}

void CGMNotifyManager::Render()
{
	if( m_ListNotify.GetCount() == 0 ) return;
	if( MAINGAME->GetCurStateNum() != eGAMESTATE_GAMEIN )
	{
		return;
	}

	sGMNOTIFY* pNotify = (sGMNOTIFY*)m_ListNotify.GetHead();
//	DWORD dwElapsedTime = gCurTime - pNotify->dwReceiveTime;
	if( gCurTime - pNotify->dwReceiveTime > NOTIFYMSG_LAST_TIME )
		PopMsg();

//render
	LONG	lPosY = m_rcPos.top;
	LONG	lPosX;
	LONG	lTextWidth;
	RECT	rect;
	PTRLISTPOS pos = m_ListNotify.GetHeadPosition();
	while(pos)
	{
		sGMNOTIFY* pNotify = (sGMNOTIFY *)m_ListNotify.GetNext(pos);
		lTextWidth = CFONT_OBJ->GetTextExtentEx( m_nFontIdx, pNotify->strMsg, pNotify->nStrLen );
		lPosX = m_rcPos.left + ( ( m_rcPos.right - m_rcPos.left ) - lTextWidth ) / 2;
		SetRect( &rect, lPosX, lPosY, lPosX + lTextWidth, lPosY + 1 );
		CFONT_OBJ->RenderFont( m_nFontIdx, pNotify->strMsg, pNotify->nStrLen, &rect, RGB_HALF( 30, 30, 30 ) );
		OffsetRect( &rect, -1, -1 );
		CFONT_OBJ->RenderFont( m_nFontIdx, pNotify->strMsg, pNotify->nStrLen, &rect, pNotify->dwColor );
		lPosY += m_nLineSpace + 10;
	}
}

void CGMNotifyManager::SetPosition( LONG lx, LONG ly, LONG lWidth )
{
	m_rcPos.left	= lx;
	m_rcPos.right	= lx + lWidth;
	m_rcPos.top		= ly;
	m_rcPos.bottom	= m_rcPos.top + 1;
}

void CGMNotifyManager::SetEventNotifyStr( char* pStrTitle, char* pStrContext )
{
	SafeStrCpy( m_strEventNotifyTitle, pStrTitle, 32 );
	SafeStrCpy( m_strEventNotifyContext, pStrContext, 128 );
}

void CGMNotifyManager::SetEventNotify( BOOL bUse )
{
	m_bUseEventNotify = bUse;
}
