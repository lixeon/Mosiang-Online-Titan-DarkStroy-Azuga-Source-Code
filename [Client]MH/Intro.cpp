// CIntro.cpp: implementation of the CIntro class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Intro.h"
#include "MovieControl.h"
#include "MainGame.h"
#include "./Engine/Engine.h"
#include "Input/UserInput.h"
#include "OptionManager.h"
#include "MainTitle.h"
#include "MHCamera.h"
#include "mhMap.h"

GLOBALTON(CIntro);

extern HWND _g_hWnd;

CIntro::CIntro()
{
	m_MovieControl = NULL;
	m_bReplayIntroFlag = FALSE;
}


CIntro::~CIntro()
{

}

BOOL CIntro::Init(void* pInitParam)
{
	m_MovieControl = NULL;
    m_MovieControl = new MovieControl;

	m_MovieControl->SetWindowHandle(_g_hWnd);
	if(m_MovieControl->ResisterFile(L"image/Movie/Intro.avi") == FALSE)
	{
		return FALSE;
	}
	MAINGAME->GetEngine()->PauseRender(TRUE);
	m_MovieControl->PlayMovie();

	if(MAP->IsInited() == TRUE)
	{
		VECTOR3 vPivot;
		vPivot.x = 2;
		vPivot.y = 2;
		vPivot.z = 2;

		CAMERA->InitCamera( 0, 0.6f, 360.f, -500, &vPivot);
	}
    return TRUE;
}

void CIntro::Release(CGameState* pNextGameState)
{
	SAFE_DELETE(m_MovieControl);
}

void CIntro::Process()
{
//	if(m_MovieControl->HandleEvent() == TRUE || KEYBOARD->GetKeyPressed(KEY_ESCAPE) || KEYBOARD->GetKeyPressed(KEY_RETURN))
//	{
	MAINGAME->GetEngine()->PauseRender(FALSE);
	if(m_bReplayIntroFlag)
	{
		TITLE->SetServerList();
		MAINGAME->SetGameState(eGAMESTATE_TITLE);
		m_bReplayIntroFlag = FALSE;
	}
	else
	{
		MAINGAME->SetGameState(eGAMESTATE_CONNECT);
	}

	if( OPTIONMGR->GetGameOption()->bIntroFlag == FALSE )
	{
		OPTIONMGR->GetGameOption()->bIntroFlag = TRUE;
		OPTIONMGR->SaveGameOption();
	}
//	}
}

void CIntro::BeforeRender()
{

}
void CIntro::AfterRender()
{

}

void CIntro::NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg)
{

}