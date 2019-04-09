// SuryunManager.cpp: implementation of the CSuryunManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _MHCLIENT_
#include "SuryunManager_client.h"
#include "MainGame.h"
#include "MapChange.h"
#include "UserInfoManager.h"
#include "GameIn.h"
#include "MPGuageDialog.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"
#include "MPRegistDialog.h"
#include "MPMissionDialog.h"
#include "MPNoticeDialog.h"
#include "InventoryExDialog.h"
#include "MugongSuryunDialog.h"
#include "cMsgBox.h"
#include "ChatManager.h"
#include "MugongBase.h"
#include "SkillInfo.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "mugongManager.h"
#include "ItemManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSuryunManager::CSuryunManager()
{
	m_MissionInfo.Initialize(64);
	m_MonsterInfo.Initialize(64);
	m_ChangeLevelTable.Initialize(64);
	LoadSuryunInfo();
	LoadMonsterInfo();
}

CSuryunManager::~CSuryunManager()
{
	SURYUNMISSIONINFO* pInfo;
	m_MissionInfo.SetPositionHead();
	while(pInfo = m_MissionInfo.GetData())
		delete pInfo;
	m_MissionInfo.RemoveAll();
	
	SURYUNMONSTERINFO* pMInfo;
	m_MonsterInfo.SetPositionHead();
	while(pMInfo = m_MonsterInfo.GetData())
		delete pMInfo;
	m_MonsterInfo.RemoveAll();	
	
	SURYUNLEVELCHANGE* pChange;
	m_ChangeLevelTable.SetPositionHead();
	while(pChange = m_ChangeLevelTable.GetData())
		delete pChange;
	m_ChangeLevelTable.RemoveAll();
}

void CSuryunManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	ySWITCH(Protocol)
		yCASE(MP_SURYUN_GOSURYUNMAP_ACK)
			GAMEIN->GetMPRegistDialog()->SetDisable(FALSE);
			GAMEIN->GetInventoryDialog()->SetDisable(FALSE);
			GAMEIN->GetMugongSuryunDialog()->SetDisable(FALSE);
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			MAPCHANGE->SetGameInInitKind(eGameInInitKind_SuryunEnter);
			DWORD MapNum = pmsg->dwData;
			MAINGAME->SetGameState(eGAMESTATE_MAPCHANGE,&MapNum,4);
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
			USERINFOMGR->SetMapChage(FALSE);
			
			WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, MBT_NOBTN, CHATMGR->GetChatMsg(669));

		yCASE(MP_SURYUN_ENTER_ACK)
			
		yCASE(MP_SURYUN_START)

		yCASE(MP_SURYUN_LEAVE_ACK)
			// do nothing

		yCASE(MP_SURYUN_RETURNTOREALWORLD)
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			MAPCHANGE->SetGameInInitKind(eGameInInitKind_SuryunLeave);
			DWORD MapNum = pmsg->dwData;
			MAINGAME->SetGameState(eGAMESTATE_MAPCHANGE,&MapNum,4);
			USERINFOMGR->SetMapChage(TRUE); //수련장 들어갈 때 false 나올 때 true			
				
		yCASE(MP_SURYUN_SUCCESSPERCENT)
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			GAMEIN->GetMPGuageDialog()->SetExpGuage(pmsg->dwData/100.f);

		yCASE(MP_SURYUN_GOSURYUNMAP_NACK)
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			ASSERT(0);
			GAMEIN->GetMPRegistDialog()->SetDisable(FALSE);
			GAMEIN->GetInventoryDialog()->SetDisable(FALSE);
			GAMEIN->GetMugongSuryunDialog()->SetDisable(FALSE);
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);


	yENDSWITCH
}

void CSuryunManager::GoSuryunMapSyn()
{
	WORD MugongIdx = GoSuryunChk();
	if( MugongIdx == 0 )
	{
		GAMEIN->GetMPRegistDialog()->SetDisable(FALSE);
		GAMEIN->GetInventoryDialog()->SetDisable(FALSE);
		GAMEIN->GetMugongSuryunDialog()->SetDisable(FALSE);
		return;
	}
	MSG_WORD msg;
	msg.Category = MP_SURYUN;
	msg.Protocol = MP_SURYUN_GOSURYUNMAP_SYN;
	msg.dwObjectID = HEROID;
	msg.wData = MugongIdx;
	
	NETWORK->Send(&msg, sizeof(msg));
}

WORD CSuryunManager::GoSuryunChk()
{
	CMugongBase* pMugong = GAMEIN->GetMPRegistDialog()->GetMugong();
	if(!pMugong)
	{		
		return 0;
	}
	
	SURYUNINFO* pSInfo = GetMissionInfo(pMugong->GetItemIdx())->GetSuryunInfo(pMugong->GetSung());
	if(pSInfo->LimitTime == 0 || pSInfo->RegenNum == 0 || pSInfo->MonNum == 0)
		return 0;
	//돈은 있는지
	if(HERO->GetMoney() < GetSuryunFee(pSInfo))
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(117));
		return 0;
	}

	// magi82 - Titan(070911) 타이탄 무공업데이트
	if( pMugong->m_pSkillInfo->GetSkillKind() == SKILLKIND_TITAN )
	{
		WINDOWMGR->MsgBox( MBI_MPNOTICE_NOTFIT, MBT_OK, CHATMGR->GetChatMsg(1659) );
		return 0;
	}

	//장착아이템과 맞는지
	if((HERO->GetWeaponEquipType() != pMugong->m_pSkillInfo->GetWeaponType()) &&
			(pMugong->m_pSkillInfo->GetSkillKind() == SKILLKIND_OUTERMUGONG))
	{
		WINDOWMGR->MsgBox( MBI_MPNOTICE_NOTFIT, MBT_OK, CHATMGR->GetChatMsg( 151 ) );
		return 0;
	}

	int adlv = GetChangedLevel(HERO->GetLevel(),pMugong->GetSung()) + pSInfo->AdjustLevel;
	if(adlv <= 0)
	{
		adlv = 1;				
//		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(673));
//		return 0;
	}
	if(GetMonsterInfo((LEVELTYPE)adlv) == NULL)
		return 0;

	return pMugong->GetItemIdx();
}

void CSuryunManager::SetActiveDlg(BOOL val)
{
	GAMEIN->GetMPMissionDialog()->SetActive(val);
	GAMEIN->GetMPNoticeDialog()->SetActive(val);
}

MONEYTYPE CSuryunManager::GetSuryunFee(SURYUNINFO* pInfo)
{
#ifdef TAIWAN_LOCAL	
	return pInfo->Fee;
#else
	if(MUGONGMGR->GetMugongNum() == 0)
		return 0;
	else
		return pInfo->Fee;
#endif
}
#endif
