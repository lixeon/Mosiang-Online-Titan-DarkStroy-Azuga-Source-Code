// MussangManager.cpp: implementation of the CMussangManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MussangManager.h"
#include "GameIn.h"
#include "ObjectManager.h"
#include "ChatManager.h"
#include "GuageDialog.h"
#include "ObjectGuagen.h"
#include "WindowIDEnum.h"
#include "StatusIconDlg.h"
#include "StatsCalcManager.h"
#include "CharacterDialog.h"
#include "ChatManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CMussangManager)

CMussangManager::CMussangManager()
{
	Init();
}

CMussangManager::~CMussangManager()
{

}

void CMussangManager::Init()
{
	m_dwMussangPoint		= 0;
	m_dwMaxMussangPoint		= 0;
	m_bIsMussangMode		= FALSE;
	m_bIsMussangReady		= FALSE;
	m_bValidBTNforSendMsg	= TRUE;

	// 06. 03. 국내무쌍 - 이영준
#ifndef _JAPAN_LOCAL_
	m_bMussang = FALSE;
#endif
}

void CMussangManager::SetMussangPoint( DWORD dwMussangPoint )
{
	m_dwMussangPoint = dwMussangPoint;

	if( m_dwMussangPoint > m_dwMaxMussangPoint )
		ASSERTMSG(0, "Over m_dwMaxMussangPoint");
	float value = (float)m_dwMussangPoint/(float)m_dwMaxMussangPoint;
	((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGEMUSSANG))->SetValue( value, 0 );
}

//void CMussangManager::SetMussangMaxPointFromLevel( DWORD dwLevel )
//{
//	DWORD dwMaxPoint = ( ( dwLevel + 3 ) * 120 ) - ( ( 30 - dwLevel ) * 10 );
//
//	ASSERT(dwMaxPoint>0);
//
//	m_dwMaxMussangPoint = dwMaxPoint;
//}

void CMussangManager::SetMussangReady(BOOL bVal) 
{
	m_bIsMussangReady = bVal;

	//무쌍 게이지 깜박임. 버튼 활성화
	GAMEIN->GetGuageDlg()->DisableMussangBtn(!bVal);	//TRUE는 비사용화
	GAMEIN->GetGuageDlg()->SetFlicker(bVal);
}

void CMussangManager::SendMsgMussangOn()
{
	if( !m_bValidBTNforSendMsg )	return;	//리턴 메시지 받고 TRUE시킨다.
	if( !m_bIsMussangReady )
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1230));
		return;
	}
#ifndef _JAPAN_LOCAL_
	if(m_bMussang)
		return;
#endif

	MSGBASE msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_MUSSANG_SYN;
	msg.dwObjectID = HERO->GetID();
	NETWORK->Send( &msg, sizeof(msg) );

	m_bValidBTNforSendMsg = FALSE;
}

void CMussangManager::SetMussangMode( BOOL bVal, DWORD time )
{
#ifdef _JAPAN_LOCAL_
	if( bVal )
	{
		if(m_bIsMussangMode)
		{
			HERO->RemoveObjectEffect( MUSSANG_EFFECT_ID );
			STATUSICONDLG->RemoveIcon(HERO, eStatusIcon_MussangMode, 0);
		}
		STATUSICONDLG->AddIcon(HERO, eStatusIcon_MussangMode, 0, MUSSANG_VALID_TIME);
	}
	else
	{
		STATUSICONDLG->RemoveIcon(HERO, eStatusIcon_MussangMode, 0);

	}
#else
	// 06. 03. 국내무쌍 - 이영준
	m_bMussang = bVal;


	DWORD dwTime = 0;

	//SW061121 무쌍발동시간 서버로 부터 초기화로 변경
	switch(HERO->GetStage())
	{
	case eStage_Normal:	
		{
			m_wMussangStat = 0;
		}
		break;
	case eStage_Hwa:		
	case eStage_Geuk:
		{
			m_wMussangStat = 10;
		}
		break;
	case eStage_Hyun:
	case eStage_Tal:
		{
			m_wMussangStat = 15;
		}
		break;
	}

	if( bVal && time )
		dwTime = time;
	else
		dwTime = 0;

	STATSMGR->CalcItemStats(HERO);

	GAMEIN->GetCharacterDialog()->SetAttackRate();
	GAMEIN->GetCharacterDialog()->SetDefenseRate();
	GAMEIN->GetCharacterDialog()->SetGenGol();
	GAMEIN->GetCharacterDialog()->SetMinChub();
	GAMEIN->GetCharacterDialog()->SetCheRyuk();
	GAMEIN->GetCharacterDialog()->SetSimMek();
	GAMEIN->GetCharacterDialog()->SetCritical();
	GAMEIN->GetCharacterDialog()->UpdateData();

	if( bVal )
	{
		STATUSICONDLG->AddIcon(HERO, eStatusIcon_MussangMode, 0, dwTime);
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1284) );		
	}
	else
	{
		STATUSICONDLG->RemoveIcon(HERO, eStatusIcon_MussangMode, 0);
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1285) );
	}

	m_bIsMussangMode = bVal;
#endif
}
