#include "stdafx.h"
#include ".\survivalmodemanager.h"
#include "MainGame.h"
#include "GameIn.h"
#include "MapChange.h"
#include "ChatManager.h"
#include "MiniMapDlg.h"
#include "SurvivalCountDialog.h"
#include "ObjectManager.h"
#include "Effect\EffectManager.h"
#include "./Interface/cScriptManager.h"


GLOBALTON(CSurvivalModeManager);

CSurvivalModeManager::CSurvivalModeManager(void)
{
//	Init();
}

CSurvivalModeManager::~CSurvivalModeManager(void)
{
	Release();
}

void CSurvivalModeManager::Init()
{
	m_dwSVVModeStartTime = 0;
	m_dwElapsedTime = gCurTime;
	
	m_wModeState = eSVVMode_None;
	m_dwStateRemainTime = 0;
	m_nUserTotalCount = 0;
	m_nUserAlive = 0;

	m_dwUsingCountLimit = 0;
	m_dwItemUsingCounter = 0;

	m_ImageNumber.Init( 68, 0 );
	m_ImageNumber.SetFillZero( FALSE );
	m_ImageNumber.SetLimitCipher( 2 );
	m_ImageNumber.SetPosition( 512 + 42, 80 );
	m_ImageNumber.SetScale( 3.0f, 3.0f );

	SCRIPTMGR->GetImage( 60, &m_ImageStart, PFT_HARDPATH );
}

void CSurvivalModeManager::Release()
{

}

void CSurvivalModeManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch(Protocol)
	{
	case MP_SURVIVAL_ALIVEUSER_COUNT:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			
			CSurvivalCountDialog* pDlg = GAMEIN->GetSurvivalCountDlg();
			pDlg->SetCounterNumber(pmsg->dwData);
		}
		break;
	case MP_SURVIVAL_READY_ACK:
		{
			m_dwSVVModeStartTime = gCurTime;
			ChangeStateTo(eSVVMode_Ready);
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1472) );
		}
		break;
	case MP_SURVIVAL_FIGHT_ACK:
		{
			ChangeStateTo(eSVVMode_Fight);
			m_dwStateRemainTime = eSVVMD_TIME_STARTRENDER + gCurTime;	//	시작 글자 찍기위해..
		}
		break;
	case MP_SURVIVAL_END_ACK:
		{
			ChangeStateTo(eSVVMode_End);

			MSG_SVVMODE_ENDINFO* pmsg = (MSG_SVVMODE_ENDINFO*)pMsg;
			GAMEIN->GetSurvivalCountDlg()->SetWinnerName(pmsg->WinnerName);

			CPlayer* pTheWinner = (CPlayer*)OBJECTMGR->GetObject(pmsg->WinnerID);

			CHATMGR->AddMsg( CTC_SURVIVAL, CHATMGR->GetChatMsg(1471), pmsg->WinnerName );

			if( pTheWinner )
			{
				// magi82(4) - Titan(071023) 타이탄일때 이펙트 처리(서바이벌 승리 이펙트)
				if(pTheWinner->InTitan() == TRUE)
					EFFECTMGR->StartEffectProcess(eEffect_TitanLevelUpSentence,pTheWinner,NULL,0,0);
				else
					EFFECTMGR->StartEffectProcess(eEffect_LevelUpSentence,pTheWinner,NULL,0,0);
			}
		}
		break;
	case MP_SURVIVAL_NONE_ACK:
		{
			ChangeStateTo(eSVVMode_None);
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1473) );
		}
		break;
	case MP_SURVIVAL_RETURNTOMAP:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			GAMEIN->GetMiniMapDialog()->SetActive( TRUE );
			ChangeStateTo( eSVVMode_None );

			MAPCHANGE->SetGameInInitKind( eGameInInitKind_SVMapLeave );
			DWORD MapNum = pmsg->dwData;
			MAINGAME->SetGameState( eGAMESTATE_MAPCHANGE, &MapNum, 4 );
			CHATMGR->SaveChatList();
		}
		break;
	case MP_SURVIVAL_STOP_NACK:
	case MP_SURVIVAL_READY_NACK:
		{

		}
		break;
	case MP_SURVIVAL_ITEMUSINGCOUNT_LIMIT:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			m_dwUsingCountLimit = pmsg->dwData;
		}
		break;
	default:
		break;
	}
}

/*
void CSurvivalModeManager::Process()
{


	switch(m_wModeState)
	{
	case eSVVMode_None:
		{
			//GM 에 의해 Ready 로 변화
		}
		break;
	case eSVVMode_Ready:
		{
			//10초 카운트 후 Fight 로 변화
			if( CheckRemainTime() )
			{
				ChangeStateTo(eSVVMode_Fight);
			}
		}
		break;
	case eSVVMode_Fight:
		{
			//1명 살아남을 시 End 로 변화
		}
		break;
	case eSVVMode_End:
		{
			//10초 카운트 후 Ready 로 변화
			if( CheckRemainTime() )
			{
				ChangeStateTo(eSVVMode_None);
			}
		}
		break;
	default:
		break;		
	}
}*/

void CSurvivalModeManager::Render()
{
	if(!m_dwStateRemainTime)	return;

	switch(m_wModeState)
	{
	case eSVVMode_Ready:
		{
			DWORD RemainTime = m_dwStateRemainTime > gCurTime ? m_dwStateRemainTime - gCurTime : 0;
			m_ImageNumber.SetNumber( RemainTime / 1000 );
			if( m_ImageNumber.IsNumberChanged() )
				m_ImageNumber.SetFadeOut( 500 );

			m_ImageNumber.Render();
		}
		break;
	case eSVVMode_Fight:
		{
			VECTOR2 Pos, Scale;
			Pos.x = (1024 - 512) / 2;
			Pos.y = 32;
			Scale.x = 1.f;
			Scale.y = 1.f;

			m_ImageStart.RenderSprite( &Scale, NULL, 0.0f, &Pos, 0xffffffff );
		}
		break;
	case eSVVMode_End:
		{
			DWORD RemainTime = m_dwStateRemainTime > gCurTime ? m_dwStateRemainTime - gCurTime : 0;
			m_ImageNumber.SetNumber( RemainTime / 1000 );
			if( m_ImageNumber.IsNumberChanged() )
				m_ImageNumber.SetFadeOut( 500 );

			m_ImageNumber.Render();
		}
		break;
	default:
		break;
	}

	CheckRemainTime();	//서버 응답이 없어도 자체적으로 시간 처리.
}

BOOL CSurvivalModeManager::CheckRemainTime()
{
	if(m_dwStateRemainTime)
	{
		if(gCurTime > m_dwStateRemainTime)
		{
			m_dwStateRemainTime = 0;
			return TRUE;
		}
		else
			return FALSE;
	}

	return FALSE;
}

void CSurvivalModeManager::ChangeStateTo( WORD nextState )
{
	if(m_wModeState == nextState)
		return;

	m_wModeState = nextState;

	switch(nextState)
	{
	case eSVVMode_None:
		{
			m_dwStateRemainTime = 0;
		}
		break;
	case eSVVMode_Ready:
		{
			m_dwStateRemainTime = gCurTime + eSVVMD_TIME_READY;

			m_ImageNumber.SetFillZero( FALSE );
			m_ImageNumber.SetFadeOut( 0 );
			m_ImageNumber.SetScale( 3.0f, 3.0f );
			m_ImageNumber.SetPosition( 512 + 32, 80 );

			m_dwItemUsingCounter = 0;
		}
		break;
	case eSVVMode_Fight:
		{
			m_dwStateRemainTime = 0;
		}
		break;
	case eSVVMode_End:
		{
			m_dwStateRemainTime = gCurTime + eSVVMD_TIME_END;

			m_ImageNumber.SetFillZero( FALSE );
			m_ImageNumber.SetFadeOut( 0 );
			m_ImageNumber.SetScale( 3.0f, 3.0f );
			m_ImageNumber.SetPosition( 512 + 32, 130 );
		}
		break;
	default:
		ASSERT(0);
		break;
	}
}

BOOL CSurvivalModeManager::AddItemUsingCount()	//제한 카운트 안넘었으면 TRUE 반환
{
	if( m_dwItemUsingCounter >= m_dwUsingCountLimit )
	{
		return FALSE;
	}
	else
	{
		++m_dwItemUsingCounter;
		return TRUE;
	}
}
