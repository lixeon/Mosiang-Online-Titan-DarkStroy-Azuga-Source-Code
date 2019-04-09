// cJackpotManager.cpp: implementation of the cJackpotManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cJackpotManager.h"
#include "MHTimeManager.h"
#include "ObjectManager.h"

#include "ChatManager.h"
#include "GMNotifyManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(cJackpotManager)

cJackpotManager::cJackpotManager()
{
	m_bIconActive = TRUE;			// keep the states itself
	m_bActive = TRUE;

	Init();
}

cJackpotManager::~cJackpotManager()
{

}

void cJackpotManager::Process()
{
	if(m_bIconActive)
	{
		AddRndMoneyForDraw();
	}
}

void cJackpotManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{

// 	char* strKind[NUM_PRIZE] = {"대박", "천운", "행운"};
	switch( Protocol )
	{
	case MP_JACKPOT_PRIZE_NOTIFY:
		{
			MSG_JACKPOT_PRIZE_NOTIFY* pmsg = (MSG_JACKPOT_PRIZE_NOTIFY*)pMsg;

			// Snake_legs
//			DWORD tmp = pmsg->dwRestTotalMoney - m_dwJackpotTotalMoney;
//			if(tmp > MONEY_PER_MON)
//				m_dwIncreaseMoney = tmp;
//!RestTotalMoney는 항상 줄어서 온다.			
			m_dwJackpotTotalMoney = pmsg->dwRestTotalMoney;
/*
	!이펙트 출력 같이..
*/
//			int kind = pmsg->nPrizeKind;
//			CHATMGR->AddMsg( CTC_JACKPOT, "[적립 상금] %s 님이 %s 을 얻으셨습니다.", pmsg->Name, strKind[kind] );
//			CHATMGR->AddMsg( CTC_JACKPOT, CHATMGR->GetChatMsg(1096), pmsg->Name, CHATMGR->GetChatMsg(1093 + pmsg->nPrizeKind) );	//It needs fix when kind changed
//#ifdef _TL_LOCAL_
//			CHATMGR->AddMsg( CTC_JACKPOT, CHATMGR->GetChatMsg(1097), CHATMGR->GetChatMsg(1093 + pmsg->nPrizeKind), pmsg->Name, AddComma( pmsg->dwPrizeMoney ), (pmsg->nPrizeKind) + 1 );
//#else
			CHATMGR->AddMsg( CTC_JACKPOT, CHATMGR->GetChatMsg(1097), CHATMGR->GetChatMsg(1093 + pmsg->nPrizeKind), pmsg->Name, (pmsg->nPrizeKind) + 1 , AddComma( pmsg->dwPrizeMoney ) );
//#endif

			CPlayer * pPlayer = (CPlayer *)OBJECTMGR->GetObject(pmsg->dwPrizeUserID);
			if(!pPlayer) return;
			EFFECTMGR->StartEffectProcess(1034,pPlayer,NULL,0,0);	//"maintain_eff_jackpot.beff"
		}
		break;			

	case MP_JACKPOT_TOTALMONEY_NOTIFY:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			// Snake_legs
//			DWORD tmp = pmsg->dwData - m_dwJackpotTotalMoney;
//			if(tmp > MONEY_PER_MON)
//				m_dwIncreaseMoney = tmp;
			
			m_dwJackpotTotalMoney = pmsg->dwData;
		}
		break;
//	case MP_JACKPOT_PRIZE_EFFECT:
//		{
//			MSG_DWORD * msg = (MSG_DWORD *)pMsg;
//			CPlayer * pPlayer = (CPlayer *)OBJECTMGR->GetObject(msg->dwData);
//			if(!pPlayer) return;
//			EFFECTMGR->StartEffectProcess(1034,pPlayer,NULL,0,0);	//"maintain_eff_jackpot.beff"
//			break;
//		}
	}
}

void cJackpotManager::AddFakeMoneyForDraw()	//Link to where MonsterDieMsg
{	// Plus Money Client User Itself
	m_dwJackpotTotalMoney += MONEY_PER_MON;
}

DWORD RndTickTime[] =
{
	1000,
	3000,
	2000,
	4000,
};

void cJackpotManager::AddRndMoneyForDraw() //Process
{	// Fake as Realtime AddTotalMoney
	DWORD dwTmp = DWORD(m_dwIncreaseMoney / (UPDATE_TICKTIME_LENGTH * 24) );	//per second

	if( gCurTime - m_dwFakeOldTickTime < RndTickTime[m_iRndArg] )
		return;

	m_iRndArg++;
	m_iRndArg > 4 ? 0 : m_iRndArg;

	m_dwJackpotTotalMoney += dwTmp;
}

void cJackpotManager::Init()
{
	m_dwJackpotTotalMoney	= 0;	// Not Real TotalMoney //!Update when Every MapChanges
	m_dwIncreaseMoney		= 0;		// For AddRndMoney

	//Snake_legs
	m_dwFakeOldTickTime		= 0;
	m_iRndArg				= 0;
}
