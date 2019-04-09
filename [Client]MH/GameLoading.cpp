// GameLoading.cpp: implementation of the CGameLoading class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameLoading.h"
#include "MainGame.h"
#include "GameIn.h"
#include "mhMap.h"
#include "cWindowSystemFunc.h"

#include "./WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cResourceManager.h"
#include "SkillManager_Client.h"
#include "./Input/Mouse.h"
#include "./Input/Keyboard.h"
#include "PathManager.h"
#include "LoadingDlg.h"
#include "./Interface/cScriptManager.h"
#include "cMsgBox.h"
#include "./Input/UserInput.h"
#include "UserInfoManager.h"
#include "ChatManager.h"

#include "GMNotifyManager.h"
#include "GameResourceManager.h"
#include "MHAudioManager.h"

extern HWND _g_hWnd;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CGameLoading)
DWORD WINAPI LoadingThread(void *p);			// 로딩 스레드
CGameLoading::CGameLoading()
{
	m_bThreadTerminated=FALSE;
	m_bMapLoadComplete=FALSE;
	m_MapNum = -1;

	m_pDlg	= NULL;
}

CGameLoading::~CGameLoading()
{

}

BOOL CGameLoading::Init(void* pInitParam)
{
	// 로그인,캐릭선택,캐릭생성에서 사용하는 멥 삭제 : 왜 여기서해?KES
//	MAP->Release();

	AUDIOMGR->StopBGM();

	m_bThreadTerminated=FALSE;
	m_bMapLoadComplete=FALSE;
	NETWORK->SetCurState(this);

	GameLoadingParam * lParam = (GameLoadingParam *)pInitParam;

	MSG_WORD	msg;
	msg.Category	= MP_USERCONN;
	msg.Protocol	= MP_USERCONN_CHARACTERSELECT_SYN;
	msg.dwObjectID	= lParam->selectedCharNum;
	msg.wData		= gChannelNum;
	NETWORK->Send(&msg, sizeof(msg));

	char filename[256];
	sprintf(filename,"Image/LoadingMap/maploadingimage%02d.tga",lParam->m_MapNum);
	if(m_LoadingImage.LoadSprite(filename) == FALSE)
	{
		sprintf(filename,"Image/LoadingMap/maploadingimage00.tga");
		m_LoadingImage.LoadSprite(filename);
	}
	m_pDlg = (CLoadingDlg *)CreateGameLoading_m();
	

#ifndef _HK_LOCAL_
#ifndef _TL_LOCAL_
	///////////////////////////////////////////////////////////////////////
	// 06. 02 로딩화면 팁추가 - 이영준
	CMHFile fp;

#ifdef _FILE_BIN_
	if( !fp.Init( ".\\Image\\LoadingTip.bin", "rb" ) )	return FALSE;	
#else
	if( !fp.Init( ".\\Image\\LoadingTip.txt", "rt" ) )	return FALSE;
#endif

	WORD TipCount = fp.GetWord();

	WORD CurTip = (rand() % TipCount) + 1;

	sprintf(filename,"Image/LoadingMap/LoadingTip%02d.tga",CurTip);
	m_TipImage.LoadSprite(filename);

	RECT rect;

//	rect.top = 0;
//	rect.bottom = 115;
//	rect.left = 0;
//	rect.right = 440;

	rect.top = 0;
	rect.bottom = 28;
	rect.left = 0;
	rect.right = 619;

	m_TipImage.SetImageSrcRect(&rect);

	fp.Release();
	///////////////////////////////////////////////////////////////////////
#endif
#endif //_HK_LOCAL_

	WINDOWMGR->AfterInit();

	m_bFirst = TRUE;
	m_MapNum = -1;

	g_UserInput.SetInputFocus( FALSE );	//로딩중엔 인풋 처리 안함. 무시.

	USERINFOMGR->SetMapChage( FALSE );//맵이동이 아닌경우에 처리할것이 있음

	return TRUE;
}
void CGameLoading::Release(CGameState* pNextGameState)
{
	m_LoadingImage.Release();
#ifndef _HK_LOCAL_
#ifndef _TL_LOCAL_
	m_TipImage.Release();
#endif
#endif	//_HK_LOCAL_
	LOGFILE("WINDOWMGR->DestroyWindowAll();");
	WINDOWMGR->DestroyWindowAll();
//	WINDOWMGR->AddListDestroyWindow( m_pDlg );
	m_pDlg = NULL;
	LOGFILE("RESRCMGR->ReleaseResource(28)");
	RESRCMGR->ReleaseResource(28);//로딩 이미지 레이어 번호
}

void CGameLoading::Process()
{
//	if(WAIT_OBJECT_0 == WaitForSingleObject(hThread, 0) )//||
		//WAIT_ABANDONED == WaitForSingleObject(hThread, 0) )
	{
//		CloseHandle(hThread);
//		m_bThreadTerminated = TRUE;
		EnterGame();
	}

	if(m_bFirst)
		m_bFirst = FALSE;
	else
	{
		if(m_bThreadTerminated == FALSE)
		{
//			SKILLMGR->Init();
			
			LOGFILE("Init EffectManager");
			EFFECTMGR->Init();
			
			m_bThreadTerminated = TRUE;
		}
		if(m_MapNum != (MAPTYPE)(-1) && m_bMapLoadComplete == FALSE && m_bThreadTerminated)
		{
			LOGFILE("MAP->InitMap(m_MapNum);");
			MAP->InitMap(m_MapNum);

			LOGFILE("PATHMGR->SetMap(MAP, m_MapNum,MAP->GetTileWidth());");
			PATHMGR->SetMap(MAP, m_MapNum,MAP->GetTileWidth());
//			MINIMAP->InitMiniMap(m_MapNum);
			
			m_bMapLoadComplete = TRUE;
		}
	}

	return;
}
void CGameLoading::BeforeRender()
{
}
void CGameLoading::AfterRender()
{
	WINDOWMGR->Render();
	//SW061111 저해상도옵션
	VECTOR2 tr;
	tr.x = tr.y = 0;
	VECTOR2 sc;
	if( GAMERESRCMNGR->IsLowResolution() )
	{
		sc.x = sc.y = (float)LOWRSLTN_W / MIDRSLTN_W;
		m_LoadingImage.RenderSprite(&sc,NULL,0,&tr,0xffffffff);
	}
	else
	{
		m_LoadingImage.RenderSprite(NULL,NULL,0,&tr,0xffffffff);
	}

#ifndef _HK_LOCAL_
#ifndef _TL_LOCAL_
	VECTOR2 tip, sz;
	//	tip.x = 550;
	//	tip.y = 30;
	//	sz.x = 0.859375;
	//	sz.y = 0.8984375;
	tip.x = 203;
	tip.y = 740;

	sz.x = 619.f/1024.f;
	sz.y = 28.f/32.f;
	if( GAMERESRCMNGR->IsLowResolution() )
	{
		sz.x = 0.859375;
		sz.y = 0.8984375;

		sz.x *= sc.x;
		sz.y *= sc.y;

		tip.x *= sc.x;
		tip.y *= sc.y;
	}
    m_TipImage.RenderSprite(&sz,NULL,0,&tip,0xffffffff);
#endif
#endif	//_HK_LOCAL_
}

void CGameLoading::NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg)
{
	switch(Category)
	{
	case MP_USERCONN:
		{
			switch(Protocol) 
			{
			case MP_USERCONN_CHARACTERSELECT_ACK:
				{
					MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
					m_MapNum = pmsg->bData;
				}
				return;

			case MP_USERCONN_CHARACTERSELECT_NACK:
				{
					LOG(EC_MAPSERVER_CLOSED);
//					WINDOWMGR->MsgBox( MBI_MAPSERVER_CLOSE, MBT_OK, "맵서버가 닫혔습니다.^n잠시후에 다시 시도하세요." );
//					MAINGAME->SetGameState(eGAMESTATE_END);	
					PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
				}
				return;
			}
		}
		break;
	case MP_CHAT:
		CHATMGR->NetworkMsgParse(Protocol, pMsg);
		break;		
	case MP_CHEAT:
		{
			switch(Protocol) 
			{
				case MP_CHEAT_EVENTNOTIFY_ON:
				{
					MSG_EVENTNOTIFY_ON* pmsg = (MSG_EVENTNOTIFY_ON*)pMsg;

					NOTIFYMGR->SetEventNotifyStr( pmsg->strTitle, pmsg->strContext );
					NOTIFYMGR->SetEventNotify( TRUE );

//					if( GAMEIN->GetEventNotifyDialog() )
//					{
//						GAMEIN->GetEventNotifyDialog()->SetTitle( pmsg->strTitle );
//						GAMEIN->GetEventNotifyDialog()->SetContext( pmsg->strContext );
//						GAMEIN->GetEventNotifyDialog()->SetActive( TRUE );
//						NOTIFYMGR->SetEventNotifyChanged( FALSE );
//						//사운드?
//					}
//					else
//					{
						NOTIFYMGR->SetEventNotifyChanged( TRUE );
//					}
						
					NOTIFYMGR->ResetEventApply();
					for(int i=0; i<eEvent_Max; ++i)
					{
						if( pmsg->EventList[i] )
							NOTIFYMGR->SetEventApply( i );
					}
				}
				break;
		
				case MP_CHEAT_EVENTNOTIFY_OFF:
				{
					MSGBASE* pmsg = (MSGBASE*)pMsg;
					NOTIFYMGR->SetEventNotify( FALSE );
					NOTIFYMGR->SetEventNotifyChanged( FALSE );
//					if( GAMEIN->GetEventNotifyDialog() )
//						GAMEIN->GetEventNotifyDialog()->SetActive( FALSE );
				}	
				break;

			}
		}
		break;
	case MP_JACKPOT:
		break;
	}
//	LOG(EC_UNKNOWN_PROTOCOL);
}

void CGameLoading::EnterGame()
{
	if(m_bMapLoadComplete && m_bThreadTerminated && !m_bDisconnected )
	{
		//여기서 게임로딩?
//		GAMEIN->InitForGame();
		int initKind = eGameInInitKind_Login;
		MAINGAME->SetGameState(eGAMESTATE_GAMEIN,&initKind,sizeof(int));
	}
}

/////////////////////// 로딩 스레드 /////////////////////////////////////
DWORD WINAPI LoadingThread(void *p)
{

	return 0;
}
//////////////////////////////////////////////////////////////////////////
