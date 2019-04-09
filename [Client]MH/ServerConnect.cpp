// ServerConnect.cpp: implementation of the CServerConnect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerConnect.h"
#include "../Engine/GraphicEngine.h"
#include "mmsystem.h"
#include "MainGame.h"

#include ".\Interface\cWindowManager.h"
#include "cWindowSystemFunc.h"

#include "MHTimeManager.h"

#include <vfw.h>

#define _LOGOWINDOW_

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLOBALTON(CServerConnect);

extern HINSTANCE g_hInst;

extern HWND _g_hWnd;

CServerConnect::CServerConnect()
{
	m_bShowingLogoAVI	= TRUE;
	m_hWndAVI		= NULL;
}


CServerConnect::~CServerConnect()
{
	
}


BOOL CServerConnect::Init(void* pInitParam)
{
#ifdef _LOGOWINDOW_
	CreateInitLogo_m();
#endif

	WINDOWMGR->AfterInit();

	m_dwStartTime = gCurTime;

//	StartShowLogoAVI();

	return TRUE;
}
void CServerConnect::Release(CGameState* pNextGameState)
{

//	EndShowLogoAVI();
}

void CServerConnect::StartShowLogoAVI()
{
	if( m_hWndAVI )
		return;
/*
	m_hWndAVI = MCIWndCreate( _g_hWnd, g_hInst,
		MCIWNDF_NOERRORDLG|MCIWNDF_NOMENU|MCIWNDF_NOOPEN|MCIWNDF_NOPLAYBAR
		|MCIWNDF_NOTIFYMODE|MCIWNDF_NOTIFYERROR,
		"D:/¹¬Çâ/°³¹ß/CWorking/logo.avi" );
*/
	if( m_hWndAVI )
	{
		MCIWndPlay(m_hWndAVI);
		m_bShowingLogoAVI = TRUE;
		MAINGAME->PauseRender( TRUE );
	}
}

void CServerConnect::EndShowLogoAVI()
{
	if( m_hWndAVI )
	{
		MCIWndClose(m_hWndAVI);
		MCIWndDestroy(m_hWndAVI);
		m_hWndAVI = NULL;
	}
	MAINGAME->PauseRender( FALSE );
	m_bShowingLogoAVI = FALSE;
}

void CServerConnect::Process()
{
//	MHTIMEMGR->Process();
#ifdef _LOGOWINDOW_
	if(m_dwStartTime - gCurTime > 1000)
#endif
//	if( !m_bShowingLogoAVI )
	{
		MAINGAME->SetGameState(eGAMESTATE_TITLE);
	}
}
void CServerConnect::BeforeRender()
{
	g_pExecutive->GetRenderer()->BeginRender(0,0,0);
	g_pExecutive->GetRenderer()->EndRender();
}
void CServerConnect::AfterRender()
{
	WINDOWMGR->Render();
}

void CServerConnect::NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg)
{	
//	LOG(EC_UNKNOWN_PROTOCOL);
}


