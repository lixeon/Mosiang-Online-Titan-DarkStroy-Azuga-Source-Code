// PyoGukManager.cpp: implementation of the CPyoGukManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PyoGukManager.h"
#include "MapDBMsgParser.h"
#include "Player.h"
#include "UserTable.h"
#include "Network.h"
#include "MHFile.h"
#include "ItemManager.h"
#include "..\[CC]Header\GameResourceManager.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPyoGukManager::CPyoGukManager()
{
}

CPyoGukManager::~CPyoGukManager()
{
}

void CPyoGukManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch( Protocol )
	{
	case MP_PYOGUK_BUY_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer)	return;
			
			if( !CheckHack( pPlayer ) )	return;
			
			BuyPyogukSyn(pPlayer);
		}
		break;
	case MP_PYOGUK_PUTIN_MONEY_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer)	return;

			if( !CheckHack( pPlayer ) )	return;

			PutInMoneyPyoguk(pPlayer, (DWORD)pmsg->dwData);
		}
		break;
	case MP_PYOGUK_PUTOUT_MONEY_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer)	return;

			if( !CheckHack( pPlayer ) )	return;

			PutOutMoneyPyoguk(pPlayer, (DWORD)pmsg->dwData);
		}
		break;
	}

}

void CPyoGukManager::LoadPyogukList()
{
	CMHFile file;
#ifdef _FILE_BIN_
	if(file.Init("./Resource/PyogukListInfo.bin","rb") == FALSE)
		return;
#else
	if(file.Init("./Resource/PyogukListInfo.txt","r") == FALSE)
		return;
#endif
	
	int count = 0;
	for(int n=0;n<MAX_PYOGUKLIST_NUM;++n)
	{
		m_PyogukListInfo[count].MaxCellNum = 30*(count+1);
		m_PyogukListInfo[count].MaxMoney = file.GetDword();
		m_PyogukListInfo[count].BuyPrice = file.GetDword();
		
		++count;
	}
	
	file.Release();

}

void CPyoGukManager::SendPyogukItemInfo(CPlayer* pPlayer)
{
	SEND_PYOGUK_ITEM_INFO PyogukItem;
	memset(&PyogukItem, 0, sizeof(SEND_PYOGUK_ITEM_INFO));
	PyogukItem.Category = MP_ITEM;
	PyogukItem.Protocol = MP_ITEM_PYOGUK_ITEM_INFO_ACK;

	ITEM_OPTION_INFO OptionInfo[SLOT_PYOGUK_NUM];
	ITEM_RARE_OPTION_INFO RareOptionInfo[SLOT_PYOGUK_NUM];
	PET_TOTALINFO PetInfo[SLOT_PYOGUK_NUM];
	TITAN_TOTALINFO TitanInfo[SLOT_PYOGUK_NUM];
	TITAN_ENDURANCE_ITEMINFO TitanEquipInfo[SLOT_PYOGUK_NUM];
	WORD OptionNum = 0;
	WORD RareOptionNum = 0;
	WORD PetInfoNum = 0;
	WORD TitanInfoNum = 0;
	WORD TitanEquipNum = 0;

	CItemSlot * pSlot = pPlayer->GetSlot(eItemTable_Pyoguk);
	PyogukItem.money = pSlot->GetPurse()->GetPurseCurMoney();
	
	POSTYPE StartPos = TP_PYOGUK_START;
	
	for(int i=0;i<SLOT_PYOGUK_NUM;++i)
	{
		if( pSlot->IsEmpty(StartPos+i) == FALSE )
		{
			PyogukItem.PyogukItem[i] = *pSlot->GetItemInfoAbs(StartPos+i);
//			if(ITEMMGR->IsOptionItem(PyogukItem.PyogukItem[i].wIconIdx, PyogukItem.PyogukItem[i].Durability))
//			{
//				// Is not null???
//				PyogukItem.PyogukItemOption[PyogukItem.wOptionCount] = *pPlayer->GetItemOption(PyogukItem.PyogukItem[i].Durability);
//				++PyogukItem.wOptionCount;
//			}
//			//SW050920 Rare
//			if(ITEMMGR->IsRareOptionItem(PyogukItem.PyogukItem[i].wIconIdx, PyogukItem.PyogukItem[i].RareIdx))
//			{
//				PyogukItem.PyogukItemRareOption[PyogukItem.wRareOptionCount] = *pPlayer->GetItemRareOption(PyogukItem.PyogukItem[i].RareIdx);
//				++PyogukItem.wRareOptionCount;
//			}
			if(ITEMMGR->IsOptionItem(PyogukItem.PyogukItem[i].wIconIdx, PyogukItem.PyogukItem[i].Durability))
			{
				memcpy(&OptionInfo[OptionNum], pPlayer->GetItemOption(PyogukItem.PyogukItem[i].Durability), sizeof(ITEM_OPTION_INFO));
				++OptionNum;
			}
			if(ITEMMGR->IsRareOptionItem(PyogukItem.PyogukItem[i].wIconIdx, PyogukItem.PyogukItem[i].RareIdx))
			{
				memcpy(&RareOptionInfo[RareOptionNum], pPlayer->GetItemRareOption(PyogukItem.PyogukItem[i].RareIdx), sizeof(ITEM_RARE_OPTION_INFO));
				++RareOptionNum;
			}
			if(ITEMMGR->IsPetSummonItem(PyogukItem.PyogukItem[i].wIconIdx))
			{
				memcpy(&PetInfo[PetInfoNum], pPlayer->GetPetManager()->GetPetTotalInfo(PyogukItem.PyogukItem[i].dwDBIdx), sizeof(PET_TOTALINFO));
				++PetInfoNum;
			}
			if(ITEMMGR->IsTitanCallItem(PyogukItem.PyogukItem[i].wIconIdx))
			{
				// tamoo 07.11.04
				TITAN_TOTALINFO* pInfo = pPlayer->GetTitanManager()->GetTitanTotalInfo(PyogukItem.PyogukItem[i].dwDBIdx);
				if( pInfo )
				{
                    memcpy(&TitanInfo[TitanInfoNum], pInfo, sizeof(TITAN_TOTALINFO));
                    ++TitanInfoNum;
				}
			}
			if( ITEMMGR->IsTitanEquipItem(PyogukItem.PyogukItem[i].wIconIdx))
			{
				// 예외 처리 - tamoo
				TITAN_ENDURANCE_ITEMINFO* pInfo = pPlayer->GetTitanManager()->GetTitanItemEnduranceInfo(PyogukItem.PyogukItem[i].dwDBIdx);
				if( pInfo )
				{
                    memcpy(&TitanEquipInfo[TitanEquipNum], pInfo, sizeof(TITAN_ENDURANCE_ITEMINFO));
                    ++TitanEquipNum;
				}
			}
		}
	}
	
	if( OptionNum )
	{
		PyogukItem.AddableInfo.AddInfo( CAddableInfoList::ItemOption, sizeof(ITEM_OPTION_INFO)*OptionNum, OptionInfo );
		PyogukItem.wOptionCount = OptionNum;
	}
	if( RareOptionNum )
	{
		PyogukItem.AddableInfo.AddInfo( CAddableInfoList::ItemRareOption, sizeof(ITEM_RARE_OPTION_INFO)*RareOptionNum, RareOptionInfo );
		PyogukItem.wRareOptionCount = RareOptionNum;
	}
	if( PetInfoNum )
	{
		PyogukItem.AddableInfo.AddInfo( CAddableInfoList::PetTotalInfo, sizeof(PET_TOTALINFO)*PetInfoNum, PetInfo );
		PyogukItem.wPetInfoCount = PetInfoNum;
	}
	if( TitanInfoNum )
	{
		PyogukItem.AddableInfo.AddInfo( CAddableInfoList::TitanTotalInfo, sizeof(TITAN_TOTALINFO)*TitanInfoNum, TitanInfo );
		PyogukItem.wTitanItemCount = TitanInfoNum;
	}
	if( TitanEquipNum )
	{
		PyogukItem.AddableInfo.AddInfo( CAddableInfoList::TitanEndrncInfo, sizeof(TITAN_ENDURANCE_ITEMINFO)*TitanEquipNum, TitanEquipInfo );
		PyogukItem.wTitanEquipItemCount = TitanEquipNum;
	}

	pPlayer->SendMsg(&PyogukItem, PyogukItem.GetSize());
}

void CPyoGukManager::BuyPyogukSyn(CPlayer* pPlayer)
{
	WORD pyoguknum = pPlayer->GetPyogukNum();

	PYOGUKLISTINFO* pBuyPyogukInfo = GetPyogukInfo(pyoguknum+1);

	if( pyoguknum >= TAB_PYOGUK_NUM || pPlayer->GetMoney() < pBuyPyogukInfo->BuyPrice )
	{
		MSGBASE msg;
		msg.Category = MP_PYOGUK;
		msg.Protocol = MP_PYOGUK_BUY_NACK;
		pPlayer->SendMsg(&msg, sizeof(msg));
	}
	else
	{
#ifdef _JAPAN_LOCAL_
		/*
		if( pyoguknum >= GIVEN_PYOGUK_SLOT )
		{
			MSGBASE msg;
			msg.Category = MP_PYOGUK;
			msg.Protocol = MP_PYOGUK_BUY_NACK;
			pPlayer->SendMsg(&msg, sizeof(msg));
			return;
		}
		*/

		DWORD extra = pPlayer->GetExtraPyogukSlot();
		if( pyoguknum >= (GIVEN_PYOGUK_SLOT+extra) )
		{
			MSGBASE msg;
			msg.Category = MP_PYOGUK;
			msg.Protocol = MP_PYOGUK_BUY_NACK;
			pPlayer->SendMsg(&msg, sizeof(msg));
			return;
		}
#endif

#ifdef _HK_LOCAL_
/*
		if( pyoguknum >= GIVEN_PYOGUK_SLOT )
		{
			MSGBASE msg;
			msg.Category = MP_PYOGUK;
			msg.Protocol = MP_PYOGUK_BUY_NACK;
			pPlayer->SendMsg(&msg, sizeof(msg));
			return;
		}
*/
		DWORD extra = pPlayer->GetExtraPyogukSlot();
		if( pyoguknum >= (GIVEN_PYOGUK_SLOT+extra) )
		{
			MSGBASE msg;
			msg.Category = MP_PYOGUK;
			msg.Protocol = MP_PYOGUK_BUY_NACK;
			pPlayer->SendMsg(&msg, sizeof(msg));
			return;
		}
#endif

#ifdef _TL_LOCAL_
/*
		if( pyoguknum >= GIVEN_PYOGUK_SLOT )
		{
			MSGBASE msg;
			msg.Category = MP_PYOGUK;
			msg.Protocol = MP_PYOGUK_BUY_NACK;
			pPlayer->SendMsg(&msg, sizeof(msg));
			return;
		}
*/
		DWORD extra = pPlayer->GetExtraPyogukSlot();
		if( pyoguknum >= (GIVEN_PYOGUK_SLOT+extra) )
		{
			MSGBASE msg;
			msg.Category = MP_PYOGUK;
			msg.Protocol = MP_PYOGUK_BUY_NACK;
			pPlayer->SendMsg(&msg, sizeof(msg));
			return;
		}
#endif
		
		if(pPlayer->GetPyogukNum() == 0) //처음 샀으면 그냥 초기화
		{
			PYOGUK_ITEM PyogukItem;
			memset(&PyogukItem, 0, sizeof(PYOGUK_ITEM));
			
			pPlayer->InitPyogukItemInfo(&PyogukItem);
		}
		pPlayer->SetMoney( pBuyPyogukInfo->BuyPrice, MONEY_SUBTRACTION, 0, eItemTable_Inventory, eMoneyLog_LosePyogukBuy, 0);

		pPlayer->SetPyogukNum( pyoguknum+1 );
		pPlayer->SetMaxPurseMoney(eItemTable_Pyoguk, GetPyogukInfo(pPlayer->GetPyogukNum())->MaxMoney);
		PyogukBuyPyoguk(pPlayer->GetID()); //db update		
		
		MSG_BYTE msg;
		msg.Category = MP_PYOGUK;
		msg.Protocol = MP_PYOGUK_BUY_ACK;
		msg.bData	= pyoguknum+1;
		pPlayer->SendMsg(&msg, sizeof(msg));
	}
}

void CPyoGukManager::PutInMoneyPyoguk(CPlayer* pPlayer, MONEYTYPE setMoney)
{
	MONEYTYPE pyogukmon = pPlayer->GetMoney(eItemTable_Pyoguk);
	MONEYTYPE maxmon = pPlayer->GetMaxPurseMoney(eItemTable_Pyoguk);
	
	if(pPlayer->GetMoney() < setMoney)
	{
		setMoney = pPlayer->GetMoney();
	}
	if( maxmon - pyogukmon < setMoney )
	{
		setMoney = maxmon - pyogukmon;
	}	
	
	if(setMoney == 0)
	{
		MSGBASE msg;
		msg.Category = MP_PYOGUK;
		msg.Protocol = MP_PYOGUK_PUTIN_MONEY_NACK;
		
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
		
	setMoney = pPlayer->SetMoney(setMoney, MONEY_SUBTRACTION, 0, eItemTable_Inventory, eMoneyLog_LosePyoguk, pPlayer->GetUserID());
	if(setMoney == 0)
	{
		ASSERTMSG(0, "Must Check!");
		return;
	}
	pPlayer->SetMoney(setMoney, MONEY_ADDITION, 0, eItemTable_Pyoguk);
	

	PyogukMoneyUpdateToDB(pPlayer->GetUserID(), pPlayer->GetMoney(eItemTable_Pyoguk));	//dbUpdate

	InsertLogMoney( eMoneyLog_LosePyoguk, pPlayer->GetID(), pPlayer->GetMoney(), setMoney, pPlayer->GetUserID(), pPlayer->GetMoney(eItemTable_Pyoguk) );
	
	//YH
	LogItemMoney( pPlayer->GetID(), pPlayer->GetObjectName(), pPlayer->GetUserID(), "#Pyoguk",
				eLog_ItemMoveInvenToPyoguk, pPlayer->GetMoney(), pPlayer->GetMoney(eItemTable_Pyoguk),
				setMoney, 0, 0, 0, 0, 0, pPlayer->GetPlayerExpPoint());

	MSG_DWORD msg;
	msg.Category = MP_PYOGUK;
	msg.Protocol = MP_PYOGUK_PUTIN_MONEY_ACK;
	msg.dwData	= pPlayer->GetMoney(eItemTable_Pyoguk);

	pPlayer->SendMsg(&msg, sizeof(msg));
}

void CPyoGukManager::PutOutMoneyPyoguk(CPlayer* pPlayer, MONEYTYPE getMoney)
{
	MONEYTYPE pyogukmon = pPlayer->GetMoney(eItemTable_Pyoguk);
	MONEYTYPE maxmon = pPlayer->GetMaxPurseMoney(eItemTable_Inventory); //인벤토리로 돈 옮기니까.
	
	if( pyogukmon < getMoney)
	{
		getMoney = pyogukmon;
	}
	//2007. 8. 30. CBH - 표국 최대금액 오버플로우 개선
	if(getMoney > maxmon - pPlayer->GetMoney())
	{
		getMoney = maxmon - pPlayer->GetMoney();
	}

	if(getMoney == 0)
		return;
	getMoney = pPlayer->SetMoney(getMoney, MONEY_SUBTRACTION, 0, eItemTable_Pyoguk);
	if(getMoney == 0) //purse에서 정말 빠졌는지 
	{
		ASSERTMSG(0, "Must Check!");
		return;
	}
	pPlayer->SetMoney(getMoney, MONEY_ADDITION, 0, eItemTable_Inventory, eMoneyLog_GetPyoguk, pPlayer->GetUserID());
	

	PyogukMoneyUpdateToDB(pPlayer->GetUserID(), pPlayer->GetMoney(eItemTable_Pyoguk));	//dbUpdate
	
	InsertLogMoney( eMoneyLog_GetPyoguk, pPlayer->GetID(), pPlayer->GetMoney(), getMoney, pPlayer->GetUserID(), pPlayer->GetMoney(eItemTable_Pyoguk) );

	//YH
	LogItemMoney( pPlayer->GetUserID(), "#Pyoguk", pPlayer->GetID(), pPlayer->GetObjectName(), 
				eLog_ItemMovePyogukToInven, pPlayer->GetMoney(eItemTable_Pyoguk), pPlayer->GetMoney(),
				getMoney, 0, 0, 0, 0, 0, pPlayer->GetPlayerExpPoint());

	MSG_DWORD msg;
	msg.Category = MP_PYOGUK;
	msg.Protocol = MP_PYOGUK_PUTOUT_MONEY_ACK;
	msg.dwData	= pPlayer->GetMoney(eItemTable_Pyoguk);

	pPlayer->SendMsg(&msg, sizeof(msg));
}

PYOGUKLISTINFO* CPyoGukManager::GetPyogukInfo(int i)
{
	ASSERT(i>0);
	ASSERT(i<=MAX_PYOGUKLIST_NUM);
	return &m_PyogukListInfo[i-1]; 
}

BOOL CPyoGukManager::CheckHack( CPlayer* pPlayer )
{
	if( pPlayer->GetShopItemManager()->GetUsingItemInfo( eIncantation_ShowPyoguk ) != NULL ||
		pPlayer->GetShopItemManager()->GetUsingItemInfo( eIncantation_ShowPyoguk7 ) != NULL ||
				pPlayer->GetShopItemManager()->GetUsingItemInfo( eIncantation_ShowPyoguk7_NoTrade ) != NULL )
		return TRUE;

	VECTOR3 vPos;
	pPlayer->GetPosition( &vPos );
	if( !GAMERESRCMNGR->IsInWareNpcPosition( &vPos ) )	return FALSE;

	return TRUE;
}
