
// QuickManager.cpp: implementation of the CQuickManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuickManager.h"
#include "MapDBMsgParser.h"
#include "Player.h"
#include "UserTable.h"
#include "BattleSystem_Server.h"
//#include "ItemSlot.h"
//#include "ItemContainer.h"

#include "AbilityManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// 06. 01 이영준 - 단축창 변경
//------------------------------------------------------
// 퀵포지션 연산 함수들
void CQuickManager::AddQuickPosition(POSTYPE& QuickPos, WORD SheetNum, WORD Pos)
{
	static WORD ADD_QUICK[4][10] = { { 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000a },
		 							 { 0x0010, 0x0020, 0x0030, 0x0040, 0x0050, 0x0060, 0x0070, 0x0080, 0x0090, 0x00a0 },
									 { 0x0100, 0x0200, 0x0300, 0x0400, 0x0500, 0x0600, 0x0700, 0x0800, 0x0900, 0x0a00 },
									 { 0x1000, 0x2000, 0x3000, 0x4000, 0x5000, 0x6000, 0x7000, 0x8000, 0x9000, 0xa000 } };

	if( Pos <= 10 || Pos > 0 )
		QuickPos = QuickPos | ADD_QUICK[SheetNum][Pos - 1];
}

void CQuickManager::DeleteQuickPosition(POSTYPE& QuickPos, WORD SheetNum)
{
	static WORD DELETE_QUICK[4] = { 0xfff0, 0xff0f, 0xf0ff, 0x0fff };

	QuickPos = QuickPos & DELETE_QUICK[SheetNum];
}

WORD CQuickManager::GetQuickPosition(POSTYPE QuickPos, WORD SheetNum)
{
	static WORD	GET_QUICK[4] = { 0x000f, 0x00f0, 0x0f00, 0xf000 };

	return QuickPos & GET_QUICK[SheetNum];
}

void CQuickManager::AnalyzeQuickPosition(POSTYPE QuickPos, WORD* QuickPosList)
{
	for(WORD count = 0; count < 4; count++)
	{
		QuickPosList[count] = GetQuickPosition( QuickPos, count ) >> ( count * 4 );
	}
}

void CQuickManager::UpdateQuickPosition(POSTYPE& QuickPos, WORD SheetNum, WORD Pos)
{
	if(SheetNum >= 4 && SheetNum < 0)
		return;

	DeleteQuickPosition( QuickPos , SheetNum );

	if(Pos <= 10 && Pos > 0)
		AddQuickPosition( QuickPos, SheetNum, Pos );
}

POSTYPE CQuickManager::MergeQuickPosition(WORD* QuickPosList)
{
	POSTYPE rt = 0;

	for(WORD SheetNum = 0; SheetNum < 4; SheetNum++)
	{
		if(QuickPosList[SheetNum] == 0)
			continue;

		AddQuickPosition(rt, SheetNum, QuickPosList[SheetNum]);
	}

	return rt;
}

void CQuickManager::AddAbilityQuickPosition(BYTE Kind, BYTE Pos, POSTYPE QuickPos, ABILITY_TOTALINFO* pOutAbilityTotalInfo)
{
	WORD QuickPosList[4] = {0,};

	AnalyzeQuickPosition(QuickPos, QuickPosList);

	for(WORD SheetNum = 0; SheetNum < 4; SheetNum++)
	{
		if(QuickPosList[SheetNum] == 0)
			continue;

		WORD PosNum = (QuickPosList[SheetNum] - 1) * 2;
		pOutAbilityTotalInfo->AbilityQuickPositionArray[SheetNum][PosNum] = Kind + ABILITYQUICKPOSITION_KEY;
		pOutAbilityTotalInfo->AbilityQuickPositionArray[SheetNum][PosNum + 1] = Pos + ABILITYQUICKPOSITION_KEY;
	}
}

POSTYPE CQuickManager::GetAbilityQuickPosition(BYTE Kind, BYTE Pos, ABILITY_TOTALINFO* pOutAbilityTotalInfo)
{
	WORD QuickPosList[4] = {0,};

	for(WORD SheetNum = 0; SheetNum < 4; SheetNum++)
	{
		for(WORD Num = 0; Num < 10; Num++)
		{
			WORD PosNum = Num * 2;

			if( ( pOutAbilityTotalInfo->AbilityQuickPositionArray[SheetNum][PosNum] == Kind + ABILITYQUICKPOSITION_KEY ) &&
				( pOutAbilityTotalInfo->AbilityQuickPositionArray[SheetNum][PosNum + 1] == Pos + ABILITYQUICKPOSITION_KEY ) )
			{
				QuickPosList[SheetNum] = Num + 1;
				break;
			}
		}
	}	

	return MergeQuickPosition(QuickPosList);
}
//------------------------------------------------------


CQuickManager::CQuickManager()
{

}

CQuickManager::~CQuickManager()
{

}

void CQuickManager::SendErrorMsg( CPlayer * pPlayer, MSG_QUICK_ERROR * msg, int msgSize, int ECode )
{
	msg->ECode = ECode;
	pPlayer->SendMsg(msg, msgSize);
}

void CQuickManager::SendAckMsg( CPlayer * pPlayer, MSGBASE * msg, int msgSize)
{
	pPlayer->SendMsg(msg, msgSize);
}

void CQuickManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	MSGBASE * pmsg = (MSGBASE *)pMsg;
	CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);			
	if(!pPlayer) return;
	if( BATTLESYSTEM->GetBattle(pPlayer)->GetBattleKind() == eBATTLE_KIND_SURYUN )
	{
		char buf[128];
		sprintf(buf, "수련장에서 퀵포지션 메시지를 보냈습니다. character_Idx: %d, Protocol : %d", pmsg->dwObjectID, pmsg->Protocol);
		ASSERTMSG(0, buf);
		return;
	}
	switch(Protocol)
	{
	case MP_QUICK_SET_SYN:
		{
			MSG_QUICK_SET_SYN * msg = (MSG_QUICK_SET_SYN *)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(msg->dwObjectID);
			if(!pPlayer) return;
			if(SetQuickItem(pPlayer, msg->QuickPos, msg->SrcPos, msg->SrcItemIdx))
			{
				MSG_QUICK_SET_ACK msg2;
				memcpy(&msg2, msg, sizeof(MSG_QUICK_SET_SYN));
				msg2.Protocol = MP_QUICK_SET_ACK;
				SendAckMsg(pPlayer, &msg2, sizeof(msg2));
			}
			else
			{
				MSG_QUICK_ERROR msg;
				msg.Category = MP_QUICK;
				msg.Protocol = MP_QUICK_SET_NACK;
				msg.dwObjectID = pPlayer->GetID();
				SendErrorMsg(pPlayer, &msg, sizeof(msg), 1);
			}
		}
		break;
	case MP_QUICK_ADD_SYN:
		{
			MSG_QUICK_ADD_SYN * msg = (MSG_QUICK_ADD_SYN *)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(msg->dwObjectID);
			if(!pPlayer) return;
			// 06. 01 이영준 - 단축창 변경
			// 프로토콜 변경 및 함수 인자값 추가
			//if(QUICKMNGR->AddQuickItem(pPlayer, msg->QuickPos, msg->SrcPos, msg->SrcItemIdx, msg->OldSrcPos, msg->OldSrcItemIdx))
			if(AddQuickItem(pPlayer, msg->QuickPos, msg->SrcPos, msg->SrcItemIdx, msg->SrcQuickPos, msg->OldSrcPos, msg->OldSrcItemIdx, msg->OldSrcQuickPos))
			{
				MSG_QUICK_ADD_ACK msg2;
				memcpy(&msg2, msg, sizeof(MSG_QUICK_ADD_SYN));
				msg2.Protocol = MP_QUICK_ADD_ACK;
				SendAckMsg(pPlayer, &msg2, sizeof(msg2));
			}
			else
			{
				MSG_QUICK_ERROR msg;
				msg.Category = MP_QUICK;
				msg.Protocol = MP_QUICK_ADD_NACK;
				msg.dwObjectID = pPlayer->GetID();
				SendErrorMsg(pPlayer, &msg, sizeof(msg), 1);
			}
		}
		break;
	case MP_QUICK_REM_SYN:
		{
			MSG_QUICK_REM_SYN * msg = (MSG_QUICK_REM_SYN *)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(msg->dwObjectID);
			if(!pPlayer) return;
			// 06. 01 이영준 - 단축창 변경
			// 프로토콜 변경 및 함수 인자값 추가
			//if(QUICKMNGR->RemQuickItem(pPlayer, msg->SrcPos, msg->SrcItemIdx))
			if(RemQuickItem(pPlayer, msg->SrcPos, msg->SrcItemIdx, msg->QuickPos, msg->SrcQuickPos))
			{
				MSG_QUICK_REM_ACK msg2;
				memcpy(&msg2, msg, sizeof(MSG_QUICK_REM_SYN));
				msg2.Protocol = MP_QUICK_REM_ACK;
				SendAckMsg(pPlayer, &msg2, sizeof(msg2));
			}
			else
			{
				MSG_QUICK_ERROR msg;
				msg.Category = MP_QUICK;
				msg.Protocol = MP_QUICK_REM_NACK;
				msg.dwObjectID = pPlayer->GetID();
				SendErrorMsg(pPlayer, &msg, sizeof(msg), 2);
			}
		}
		break;
	case MP_QUICK_MOVE_SYN:
		{
			MSG_QUICK_MOVE_SYN * msg = (MSG_QUICK_MOVE_SYN *)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(msg->dwObjectID);
			if(!pPlayer) return;
			// 06. 01 이영준 - 단축창 변경
			// 프로토콜 변경 및 함수 인자값 추가
			//if(QUICKMNGR->MoveQuickItem(pPlayer, msg->FromSrcPos, msg->wFromSrcItemIdx, msg->FromQuickPos, msg->ToSrcPos, msg->wToSrcItemIdx, msg->ToQuickPos))
			if(MoveQuickItem(pPlayer, msg->FromSrcPos, msg->wFromSrcItemIdx, msg->FromQuickPos, msg->FromSrcQuickPos, msg->ToSrcPos, msg->wToSrcItemIdx, msg->ToQuickPos, msg->ToSrcQuickPos, msg->DeleteToSrcQuickPos))
			{
				MSG_QUICK_MOVE_ACK msg2;
				memcpy(&msg2, msg, sizeof(MSG_QUICK_MOVE_SYN));
				msg2.Protocol = MP_QUICK_MOVE_ACK;
				SendAckMsg(pPlayer, &msg2, sizeof(msg2));
			}
			else
			{
				MSG_QUICK_ERROR msg;
				msg.Category = MP_QUICK;
				msg.Protocol = MP_QUICK_MOVE_NACK;
				msg.dwObjectID = pPlayer->GetID();
				SendErrorMsg(pPlayer, &msg, sizeof(msg), 3);
			}
		}
		break;
	}
}

BOOL ChangeV(CPlayer * pPlayer, POSTYPE WhatSrcPos, WORD WhatSrcItemIdx, POSTYPE QuickPos)
{
	//디버깅용 코드
	//WORD QuickPosList[4] = {0,};

	//QUICKMNGR->AnalyzeQuickPosition(QuickPos, QuickPosList);
	
	//return TRUE; // 임시로 DB기록 막음
	
	eQUICKICON_KIND quickGroup = GetIconKind(WhatSrcItemIdx);
	if(quickGroup == eITEM_ICON)
	{
		if( !( ( TP_INVENTORY_START <= WhatSrcPos && WhatSrcPos < TP_WEAR_END ) ||
		       ( TP_SHOPINVEN_START <= WhatSrcPos && WhatSrcPos < TP_SHOPINVEN_END ) ) )
			return FALSE;
		
		CItemSlot * pSrcSlot = pPlayer->GetSlot(WhatSrcPos);
		if(pSrcSlot == NULL)
		{
			ASSERT(0);
			return FALSE;
		}
		const ITEMBASE * pItemBase = pSrcSlot->GetItemInfoAbs(WhatSrcPos);
		if(pItemBase == 0)
		{
			ASSERT(0);
			return FALSE;
		}
		if(pItemBase->wIconIdx != WhatSrcItemIdx)
			return FALSE;

		if(EI_TRUE != pSrcSlot->UpdateItemAbs(pPlayer, WhatSrcPos, 0,0,0,QuickPos,0,UB_QABSPOS))
			return FALSE;

		// db update
		ItemUpdateToDB(pPlayer->GetID(), pItemBase->dwDBIdx, pItemBase->wIconIdx, pItemBase->Durability, 
						pItemBase->Position, pItemBase->QuickPosition, pItemBase->RareIdx );
	}
	else if( quickGroup == eMUGONG_ICON )
	{
		//무공	
		MUGONGBASE * pMugongBase = pPlayer->GetMugongBase(WhatSrcPos);
		if(pMugongBase == 0)
		{
			ASSERT(0);
			return FALSE;
		}
		if(pMugongBase->wIconIdx != WhatSrcItemIdx)
		{
			return FALSE;
		}	
		if(pMugongBase->Sung == 0)
		{
			return FALSE;
		}

		pMugongBase->QuickPosition = QuickPos;

		// db update
		MugongUpdateToDB(pMugongBase,"QUICK");
	}
	else if( quickGroup == eABILITY_ICON )
	{
		//To Do : quick position update
		ABILITYMGR->SetAbilityQuickPosition(WhatSrcItemIdx-MIN_ABILITY_INDEX, QuickPos, pPlayer->GetAbilityGroup() );
	}
	else
	{
		char buf[64] = {0,};
		sprintf(buf, "Error! quickGroup : %d", quickGroup);
		ASSERTMSG(0, buf);
		return FALSE;
	}

	return TRUE;
}

// 06. 01 이영준 - 단축창 변경
// 프로토콜 변경 및 함수 인자값 추가
//BOOL CQuickManager::AddQuickItem( CPlayer * pPlayer, POSTYPE QuickPos, POSTYPE srcPos, WORD wSrcItemIdx, POSTYPE OldSrcPos, WORD wOldSrcItemIdx)
BOOL CQuickManager::AddQuickItem( CPlayer * pPlayer, POSTYPE QuickPos, POSTYPE srcPos, WORD wSrcItemIdx, POSTYPE SrcQuickPos, POSTYPE OldSrcPos, WORD wOldSrcItemIdx, POSTYPE OldSrcQuickPos )
{
	CInventorySlot* pSlot = (CInventorySlot*)pPlayer->GetSlot( eItemTable_Inventory );
	if( GetItemKind( wSrcItemIdx ) == eYOUNGYAK_ITEM )
	{
		if( pSlot->CheckItemLockForItemIdx( wSrcItemIdx ) == FALSE )
			return FALSE;
	}

	WORD SheetNum = QuickPos / TABCELL_QUICK_NUM;
	WORD PosNum = QuickPos % TABCELL_QUICK_NUM + 1;

	//기존에 있으면 지움
	if(wOldSrcItemIdx != 0)
	{
		//ChangeV(pPlayer, OldSrcPos, wOldSrcItemIdx, 0);
		//무조건 0으로 만들어주는것이 아니라 해당 시트만 삭제한다
		DeleteQuickPosition(OldSrcQuickPos, SheetNum);
		ChangeV(pPlayer, OldSrcPos, wOldSrcItemIdx, OldSrcQuickPos);
	}
	
	//퀵포지션 정보 갱신
	UpdateQuickPosition(SrcQuickPos, SheetNum, PosNum);
	return ChangeV(pPlayer, srcPos, wSrcItemIdx, SrcQuickPos);
	//return ChangeV(pPlayer, srcPos, wSrcItemIdx, QuickPos);
}

BOOL CQuickManager::SetQuickItem( CPlayer * pPlayer, POSTYPE QuickPos, POSTYPE srcPos, WORD wSrcItemIdx )
{
	CInventorySlot* pSlot = (CInventorySlot*)pPlayer->GetSlot( eItemTable_Inventory );
	if( GetItemKind( wSrcItemIdx ) == eYOUNGYAK_ITEM )
	{
		if( pSlot->CheckItemLockForItemIdx( wSrcItemIdx ) == FALSE )
			return FALSE;
	}
	
	return ChangeV(pPlayer, srcPos, wSrcItemIdx, QuickPos);
}

// 06. 01 이영준 - 단축창 변경
// 프로토콜 변경 및 함수 인자값 추가
//BOOL CQuickManager::RemQuickItem(CPlayer * pPlayer, POSTYPE SrcPos, WORD wSrcItemIdx)
BOOL CQuickManager::RemQuickItem(CPlayer * pPlayer, WORD SrcPos, WORD wSrcItemIdx, POSTYPE QuickPos, POSTYPE SrcQuickPos)
{
	//무조건 0으로 만들어주는것이 아니라 해당 시트만 삭제한다
	WORD SheetNum = QuickPos / TABCELL_QUICK_NUM;
	DeleteQuickPosition(SrcQuickPos, SheetNum);
	return ChangeV(pPlayer, SrcPos, wSrcItemIdx, SrcQuickPos);
	//return ChangeV(pPlayer, SrcPos, wSrcItemIdx, 0);
}

// 06. 01 이영준 - 단축창 변경
// 프로토콜 변경 및 함수 인자값 추가
//BOOL CQuickManager::MoveQuickItem(CPlayer * pPlayer, POSTYPE FromSrcPos, WORD wFromSrcItemIdx, POSTYPE FromQuickPos, POSTYPE ToSrcPos, WORD wToSrcItemIdx, POSTYPE ToQuickPos )
BOOL CQuickManager::MoveQuickItem(CPlayer * pPlayer, POSTYPE FromSrcPos, WORD wFromSrcItemIdx, POSTYPE FromQuickPos, POSTYPE FromSrcQuickPos, POSTYPE ToSrcPos, WORD wToSrcItemIdx, POSTYPE ToQuickPos, POSTYPE ToSrcQuickPos, bool DeleteToSrcQuickPos)
{
	WORD ToSheetNum = ToQuickPos / TABCELL_QUICK_NUM;
	WORD ToPosNum = ToQuickPos % TABCELL_QUICK_NUM + 1;
	WORD FromSheetNum = FromQuickPos / TABCELL_QUICK_NUM;
	WORD FromPosNum = FromQuickPos % TABCELL_QUICK_NUM + 1;

	//퀵포지션 정보 갱신
	if(FromSheetNum != ToSheetNum)
		DeleteQuickPosition(FromSrcQuickPos, FromSheetNum);
	UpdateQuickPosition(FromSrcQuickPos, ToSheetNum, ToPosNum);
	if(!ChangeV(pPlayer, FromSrcPos, wFromSrcItemIdx, FromSrcQuickPos))
	//if(!ChangeV(pPlayer, FromSrcPos, wFromSrcItemIdx, ToQuickPos))
	{
		return FALSE;
	}

	if( wToSrcItemIdx == 0 ) return TRUE;

	//퀵포지션 정보 갱신
	if(FromSheetNum != ToSheetNum)
		DeleteQuickPosition(ToSrcQuickPos, ToSheetNum);
	if(!DeleteToSrcQuickPos)
		UpdateQuickPosition(ToSrcQuickPos, FromSheetNum, FromPosNum);
	if(!ChangeV(pPlayer, ToSrcPos, wToSrcItemIdx, ToSrcQuickPos))
	//if(!ChangeV(pPlayer, ToSrcPos, wToSrcItemIdx, FromQuickPos))
	{
		//rollback
		//퀵포지션 정보 갱신
		if(FromSheetNum != ToSheetNum)
			DeleteQuickPosition(FromSrcQuickPos, ToSheetNum);
		UpdateQuickPosition(FromSrcQuickPos, FromSheetNum, FromPosNum);
		if(!ChangeV(pPlayer, FromSrcPos, wFromSrcItemIdx, FromSrcQuickPos))
		//if(!ChangeV(pPlayer, FromSrcPos, wFromSrcItemIdx, FromQuickPos))
			ASSERT(0);
		return FALSE;
	}
	return TRUE;
}

/*
	QUICKITEM quickItem;
	MSG_QUICK_ADD_ACK msg;



	// 무공,아이템 정보에도 넣음
	eQUICKICON_KIND quickGroup = GetIconKind(pmsg->wIconIdx);
	if(quickGroup == eITEM_ICON)
	{
		eITEM_KINDBIT bits = GetItemKind(pmsg->wIconIdx);
		if(bits & eEQUIP_ITEM || bits & eYOUNGYAK_ITEM)
		{
			CIconTable * pTable = NULL;
			switch(GetTableIdxPosition(pmsg->wSrcAbsPosition))
			{
			case eItemTable_Inventory:
				{
					pTable = pPlayer->GetIconTable(eItemTable_Inventory);
				}
				break;
			case eItemTable_Weared:
				{
					pTable = pPlayer->GetIconTable(eItemTable_Weared);
				}
				break;
			case eItemTable_MunpaWarehouse:
				{
					pTable = pPlayer->GetIconTable(eItemTable_Weared);
				}
				break;
			default:
				{
					ASSERT(0);
					return;
				}
			}
			ITEMBASE * item = pTable->GetItemInfoAbs(pmsg->wSrcAbsPosition);
			if(item->QuickPosition != 0)			// 중복 등록 불가
			{
				RemQuickItem(pPlayer, GetTableIdxPosition(pmsg->wSrcAbsPosition), item->QuickPosition);
				//goto QI_ADD_FAILED;
			}
			item->QuickPosition = pmsg->wQuickAbsPosition;
		}
		else 
		{
			goto QI_ADD_FAILED;
		}
	}
	else
	{
		// 무공
		MUGONGBASE * pMugong = pPlayer->GetMugongBase(pmsg->wSrcAbsPosition);
		if(pMugong->QuickPosition != 0)				
		{
			// 중복 등록 =>. 이전거 지움
			RemQuickItem(pPlayer, GetTableIdxPosition(pmsg->wSrcAbsPosition), pMugong->QuickPosition);
			//goto QI_ADD_FAILED;
		}	
		if(pMugong->Sung == 0)
		{
			// 0성짜리 무공은 등록 불가
			goto QI_ADD_FAILED;
		}
		pMugong->QuickPosition = pmsg->wQuickAbsPosition;
	}


	quickItem.dwIconIdx = pmsg->wIconIdx;
	quickItem.eIGKinds = quickGroup;
	quickItem.wAbsPostion = pmsg->wSrcAbsPosition;
	pPlayer->SetQuickItem(pmsg->wQuickAbsPosition,&quickItem);

	msg.Category = MP_QUICK;
	msg.Protocol = MP_QUICK_ADD_ACK;
	msg.dwObjectID = pmsg->dwObjectID;
	msg.wIconIdx = pmsg->wIconIdx;
	msg.wSrcAbsPosition = pmsg->wSrcAbsPosition;
	msg.wQuickAbsPosition = pmsg->wQuickAbsPosition;


	pPlayer->SendMsg(&msg, sizeof(msg));
	
	return;

QI_ADD_FAILED:
	MSG_WORD msgWord;
	msgWord.Category = MP_QUICK;
	msgWord.Protocol = MP_QUICK_ADD_NACK;
	msgWord.dwObjectID = pmsg->dwObjectID;
	msgWord.wData = 1;
	pPlayer->SendMsg(&msgWord, sizeof(msgWord));
}

void CQuickManager::RemQuickItem(CPlayer * pPlayer, MSG_QUICK_REM_SYN * pmsg)
{
	RemQuickItem(pPlayer, pmsg->wTableIdx, pmsg->AbsPosition);
}
void CQuickManager::RemQuickItem(CPlayer * pPlayer, WORD tableIdx, WORD absPosition)
{
	// 무공,아이템 정보에도 넣음
	WORD wSrcAbsPosition = 0;
	QUICKITEM itemOut;
	if(!pPlayer->GetQuickPostion(absPosition,&itemOut))
		return;
	wSrcAbsPosition = itemOut.wAbsPostion;

	switch(itemOut.eIGKinds)
	{
	case eITEM_ICON:
		{
			
			CIconTable * pTable = pPlayer->GetIconTable(tableIdx);
			if( !pTable ) return ;
			ITEMBASE * item = pTable->GetItemInfoAbs(itemOut.wAbsPostion);
			item->QuickPosition = 0;
			CharacterItemUpdate(pPlayer->GetID(), item->dwDBIdx, item->wIconIdx, item->Durability, item->Position, item->QuickPosition);
		}
		break;
	case eMUGONG_ICON:
		{
			MUGONGBASE * pMugong = pPlayer->GetMugongBase(itemOut.wAbsPostion);
			pMugong->QuickPosition = 0;
			CharacterMugongUpdate(pMugong);
		}
		break;
	default:
		{
			//not implement
			//특수
			//ASSERT(0);
			return;
		}
	}


	PureRemQuickItemAbs(pPlayer, absPosition);
	
	MSG_QUICK_REM_ACK msg;
	msg.Category = MP_QUICK;
	msg.Protocol = MP_QUICK_REM_ACK;
	msg.dwObjectID = pPlayer->GetID();
	msg.wQuickAbsPosition = absPosition;
	msg.wSrcAbsPosition = wSrcAbsPosition;

	pPlayer->SendMsg(&msg, sizeof(msg));
}

void CQuickManager::MoveQuickItem(CPlayer * pPlayer, MSG_QUICK_MOVE_SYN * pmsg)
{	

	QUICKITEM FromQuickItem, ToQuickItem;
	if(!pPlayer->GetQuickPostion(pmsg->wFromAbsPosition,&FromQuickItem))
	{
		ASSERT(0);
	}
	if(!pPlayer->GetQuickPostion(pmsg->wToAbsPosition,&ToQuickItem))
	{
		ASSERT(0);
	}


	PureUpdateQuickItem(pPlayer, pmsg->wToAbsPosition, FromQuickItem.dwIconIdx, FromQuickItem.eIGKinds, FromQuickItem.wAbsPostion);
	PureUpdateQuickItem(pPlayer, pmsg->wFromAbsPosition, ToQuickItem.dwIconIdx, ToQuickItem.eIGKinds, ToQuickItem.wAbsPostion);

	MSG_QUICK_MOVE_ACK msg;
	msg.Category = MP_QUICK;
	msg.Protocol = MP_QUICK_MOVE_ACK;
	msg.dwObjectID = pPlayer->GetID();
	msg.wFromAbsPosition= pmsg->wFromAbsPosition;
	msg.wToAbsPosition = pmsg->wToAbsPosition;

	pPlayer->SendMsg(&msg, sizeof(msg));



	// 아이템혹은 무공의 QPOSITION을 바꿔줘야 함
	// DB는 안바꿔주나?

	int tableIdx = 0;
	switch((tableIdx = GetTableIdxPosition(FromQuickItem.wAbsPostion)))
	{
	case eItemTable_Inventory:
	case eItemTable_Weared:
		{
			CIconTable * pFromTable = pPlayer->GetIconTable(tableIdx);
			ITEMBASE * FromItem = pFromTable->GetItemInfoAbs(FromQuickItem.wAbsPostion);
			if(FromItem&&FromItem->dwDBIdx!=0)
				FromItem->QuickPosition = pmsg->wToAbsPosition;
			else
			{
				ASSERT(0);
			}
		}
		break;
	case eItemTable_Mugong:
		{
			MUGONGBASE * FromMugong = pPlayer->GetMugongBase(FromQuickItem.wAbsPostion);
			if(FromMugong && FromMugong->dwDBIdx != 0)
				FromMugong->QuickPosition = pmsg->wToAbsPosition;
			else
			{
				ASSERT(0);
			}
		}
		break;
	default:
		{
			//ASSERT(0);
			return ;

		}
	}

	if(ToQuickItem.dwIconIdx != 0)
	switch((tableIdx = GetTableIdxPosition(ToQuickItem.wAbsPostion)))
	{
	case eItemTable_Inventory:
	case eItemTable_Weared:
		{
			CIconTable * pToTable = pPlayer->GetIconTable(tableIdx);
			ITEMBASE * ToItem = pToTable->GetItemInfoAbs(ToQuickItem.wAbsPostion);
			if(ToItem&&ToItem->dwDBIdx!=0)
				ToItem->QuickPosition = pmsg->wFromAbsPosition;
			else
			{
				ASSERT(0);
			}
		}
		break;
	case eItemTable_Mugong:
		{
			MUGONGBASE * ToMugong = pPlayer->GetMugongBase(ToQuickItem.wAbsPostion);
			if(ToMugong && ToMugong->dwDBIdx != 0)
				ToMugong->QuickPosition = pmsg->wFromAbsPosition;
			else
			{
				ASSERT(0);
			}
		}
		break;
	default:
		{
			ASSERT(0);
		}
	}

}
void CQuickManager::UpdateSrcPosition( CPlayer * pPlayer, POSTYPE QAbsPos, POSTYPE srcAbsPos )
{
	QUICKITEM * itemOut = pPlayer->GetQuickItemInfo((QAbsPos-QUICK_STARTPOSITION)/MAX_QUICKITEMPERSHEET_NUM, 
							(QAbsPos-QUICK_STARTPOSITION)%MAX_QUICKITEMPERSHEET_NUM);

	itemOut->wAbsPostion = srcAbsPos;

	MSG_QUICK_UPD_ACK msg;
	msg.Category			= MP_QUICK;
	msg.Protocol			= MP_QUICK_UPD_ACK;
	msg.dwObjectID			= pPlayer->GetID();
	msg.wAbsPosition		= QAbsPos;
	msg.wSrcPosition		= srcAbsPos;

	pPlayer->SendMsg( &msg, sizeof(msg) );
}

void CQuickManager::PureUpdateQuickItem(CPlayer * pPlayer, WORD whatQuickAbsPos, DWORD dwIconIdx, WORD eIGKinds, WORD srcAbsPos, WORD flag)
{
	QUICKITEM * itemOut = pPlayer->GetQuickItemInfo((whatQuickAbsPos-QUICK_STARTPOSITION)/MAX_QUICKITEMPERSHEET_NUM, 
							(whatQuickAbsPos-QUICK_STARTPOSITION)%MAX_QUICKITEMPERSHEET_NUM);

	if(flag & QUB_ICONIDX)
	itemOut->dwIconIdx = dwIconIdx;
	if(flag & QUB_KINDS)
	itemOut->eIGKinds = (eQUICKICON_KIND)eIGKinds;
	if(flag & QUB_SRCPOS)
	itemOut->wAbsPostion = srcAbsPos;
}


void CQuickManager::PureRemQuickItemAbs(CPlayer * pPlayer, WORD abs_pos)
{
	QUICKITEM quickItem;
	quickItem.dwIconIdx = 0;
	quickItem.eIGKinds = (eQUICKICON_KIND)0;
	quickItem.wAbsPostion = 0;
	pPlayer->SetQuickItem(abs_pos,&quickItem);
}


*/
