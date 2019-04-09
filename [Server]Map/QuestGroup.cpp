// QuestGroup.cpp: implementation of the CQuestGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestGroup.h"

#include "QuestManager.h"
#include "Quest.h"

#include "Player.h"
#include "ItemManager.h"
#include "MapDBMsgParser.h"
#include "QuestRegenMgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CheckQuestProbability(DWORD dwProbability)
{
	if( dwProbability == 0 )
		return FALSE;
	if( dwProbability != MAX_QUEST_PROBABILITY )
	{
		DWORD nFlag = rand()%MAX_QUEST_PROBABILITY;
		if( nFlag >= dwProbability )
			return FALSE;
	}

	return TRUE;
}

CQuestGroup::CQuestGroup()
{
	m_pPlayer = NULL;
	m_QuestTable.Initialize( MAX_QUEST );
	m_QuestItemTable.Initialize( MAX_QUESTITEM );
	memset( m_QuestEvent, 0, sizeof(GQUESTEVENT)*MAX_QUESTEVENT_PLYER );
	m_dwQuestEventNum = 0;
}

CQuestGroup::CQuestGroup( PLAYERTYPE* pPlayer )
{
	m_pPlayer = pPlayer;
	m_QuestTable.Initialize( MAX_QUEST );
	m_QuestItemTable.Initialize( MAX_QUESTITEM );
	memset( m_QuestEvent, 0, sizeof(GQUESTEVENT)*MAX_QUESTEVENT_PLYER );
	m_dwQuestEventNum = 0;
}

CQuestGroup::~CQuestGroup()
{
	Release();
}

void CQuestGroup::Initialize( PLAYERTYPE* pPlayer )
{
	m_pPlayer = pPlayer;
//	m_QuestTable.Initialize( MAX_QUEST );
//	m_QuestItemTable.Initialize( MAX_QUESTITEM );
	memset( m_QuestEvent, 0, sizeof(GQUESTEVENT)*MAX_QUESTEVENT_PLYER );
	m_dwQuestEventNum = 0;
}

void CQuestGroup::Release()
{
	CQuest* pQuest = NULL;
	m_QuestTable.SetPositionHead();
	while( pQuest = m_QuestTable.GetData() )
	{
		delete pQuest;
		pQuest = NULL;
	}
	m_QuestTable.RemoveAll();

	QUESTITEM* pQuestItem = NULL;
	m_QuestItemTable.SetPositionHead();
	while( pQuestItem = m_QuestItemTable.GetData() )
	{
		delete pQuestItem;
		pQuestItem = NULL;
	}
	m_QuestItemTable.RemoveAll();

	m_pPlayer = NULL;
	memset( m_QuestEvent, 0, sizeof(GQUESTEVENT)*MAX_QUESTEVENT_PLYER );
	m_dwQuestEventNum = 0;
}

void CQuestGroup::CreateQuest( DWORD dwQuestIdx, CQuestInfo* pQuestInfo )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( pQuest == NULL )
	{
		pQuest = new CQuest( m_pPlayer, pQuestInfo );
		m_QuestTable.Add( pQuest, dwQuestIdx );
	}
}

BOOL CQuestGroup::SetMainQuestData( DWORD dwQuestIdx, DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime, BYTE bCheckType, DWORD dwCheckTime )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( pQuest )
		return pQuest->SetMainQuestData( dwSubQuestFlag, dwData, dwTime, bCheckType, dwCheckTime );

	return TRUE;
}

void CQuestGroup::SetSubQuestData( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwData, DWORD dwTime )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( pQuest )
		pQuest->SetSubQuestData( dwSubQuestIdx, dwData, dwTime );
}

void CQuestGroup::SetQuestItem( DWORD dwQuestIdx, DWORD dwItemIdx, DWORD dwItemNum )
{
	QUESTITEM* pQuestItem = NULL;

	// AO´AAo E®AICI°i AO8¸e Ao¿i´U.
	pQuestItem = m_QuestItemTable.GetData( dwItemIdx );
	if(pQuestItem)
	{
		m_QuestItemTable.Remove( pQuestItem->dwItemIdx );
		delete pQuestItem;
		pQuestItem = NULL;
	}

	pQuestItem = new QUESTITEM;
	pQuestItem->dwQuestIdx = dwQuestIdx;
	pQuestItem->dwItemIdx = dwItemIdx;
	pQuestItem->dwItemNum = dwItemNum;
	
	m_QuestItemTable.Add( pQuestItem, pQuestItem->dwItemIdx );
}

void CQuestGroup::AddQuestEvent( CQuestEvent* pQuestEvent, DWORD dwQuestIdx )
{
	if( m_dwQuestEventNum < MAX_QUESTEVENT_PLYER )
	{
		m_QuestEvent[m_dwQuestEventNum].dwQuestIdx = dwQuestIdx;
		m_QuestEvent[m_dwQuestEventNum].QuestEvent.SetValue( pQuestEvent );
		++m_dwQuestEventNum;
	}
}

BOOL CQuestGroup::IsQuestComplete( DWORD dwQuestIdx )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );

	if( pQuest )
	if( pQuest->IsQuestComplete() )
		return TRUE;

	return FALSE;
}

void CQuestGroup::DeleteQuest( DWORD dwQuestIdx )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( pQuest )
	{
		pQuest->EndQuest( dwQuestIdx, 1, FALSE );

		// quest item delete
		QUESTITEM* pQuestItem = NULL;
		m_QuestItemTable.SetPositionHead();
		cPtrList list;
		while( pQuestItem = m_QuestItemTable.GetData() )
		{
			if( pQuestItem->dwQuestIdx == dwQuestIdx )
			{
				list.AddTail( pQuestItem );
				//m_QuestItemTable.Remove( pQuestItem->dwItemIdx );
				//delete pQuestItem;
				//pQuestItem = NULL;
			}
		}

		PTRLISTPOS pos = list.GetHeadPosition();
		while( pos )
		{
			QUESTITEM* p = (QUESTITEM*)list.GetNext(pos);
			m_QuestItemTable.Remove( p->dwItemIdx );
			delete p;
		}
		list.RemoveAll();

		// item delete
		MSG_EXCHANGE_REMOVEITEM Msg;
		Msg.Category = MP_QUEST;
		Msg.Protocol = MP_QUEST_GIVEITEM_ACK;

		WORD wKind, wType;
		ITEM_TOTALINFO ItemInfo;
		ITEMBASE logitem;
		m_pPlayer->GetItemtotalInfo( &ItemInfo, GETITEM_FLAG_INVENTORY|GETITEM_FLAG_WEAR );
		
		for( int i = 0; i < SLOT_INVENTORY_NUM;	++i )
		{
			ITEMMGR->GetItemKindType( ItemInfo.Inventory[i].wIconIdx, &wKind, &wType );
			if( wKind == eQUEST_ITEM_EQUIP && wType == dwQuestIdx )
			{			
				logitem.Durability = ItemInfo.Inventory[i].Durability;
				logitem.dwDBIdx = ItemInfo.Inventory[i].dwDBIdx;
				logitem.Position = ItemInfo.Inventory[i].Position;
				logitem.wIconIdx = ItemInfo.Inventory[i].wIconIdx;
				// magi82(28)
				logitem.ItemParam = 0;
				logitem.QuickPosition = 0;
				logitem.RareIdx = 0;
			
				if( EI_TRUE == ITEMMGR->DiscardItem( m_pPlayer, ItemInfo.Inventory[i].Position, ItemInfo.Inventory[i].wIconIdx, 1 ) )
				{
					Msg.wAbsPosition = ItemInfo.Inventory[i].Position;
					m_pPlayer->SendMsg( &Msg, sizeof(Msg) );
					
					// Log
					LogItemMoney(m_pPlayer->GetID(), m_pPlayer->GetObjectName(), 0, "",
						eLog_ItemDestroyDeleteQuest, m_pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
						logitem.wIconIdx , logitem.dwDBIdx, logitem.Position, 0, logitem.Durability, m_pPlayer->GetPlayerExpPoint());
				}
			}
		}
		for( i = 0; i < SLOT_WEAR_NUM; ++i )
		{
			ITEMMGR->GetItemKindType( ItemInfo.WearedItem[i].wIconIdx, &wKind, &wType );
			if( wKind == eQUEST_ITEM_EQUIP && wType == dwQuestIdx )
			{			
				logitem.Durability = ItemInfo.Inventory[i].Durability;
				logitem.dwDBIdx = ItemInfo.Inventory[i].dwDBIdx;
				logitem.Position = ItemInfo.Inventory[i].Position;
				logitem.wIconIdx = ItemInfo.Inventory[i].wIconIdx;
				// magi82(28)
				logitem.ItemParam = 0;
				logitem.QuickPosition = 0;
				logitem.RareIdx = 0;

				if( EI_TRUE == ITEMMGR->DiscardItem( m_pPlayer, ItemInfo.WearedItem[i].Position, ItemInfo.WearedItem[i].wIconIdx, 1 ) )
				{
					// Log
					LogItemMoney(m_pPlayer->GetID(), m_pPlayer->GetObjectName(), 0, "",
						eLog_ItemDestroyDeleteQuest, m_pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
						logitem.wIconIdx , logitem.dwDBIdx, logitem.Position, 0, logitem.Durability, m_pPlayer->GetPlayerExpPoint());

					Msg.wAbsPosition = ItemInfo.WearedItem[i].Position;
					m_pPlayer->SendMsg( &Msg, sizeof(Msg) );
				}
			}
		}
	}
}

void CQuestGroup::EndQuest( DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( pQuest )
	if( pQuest->EndQuest( dwQuestIdx, dwSubQuestIdx ) )
	{
		QUESTMGR->EndQuest( m_pPlayer, dwQuestIdx, dwSubQuestIdx );
	}
}

void CQuestGroup::StartSubQuest( DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( pQuest )
	if( pQuest->StartSubQuest( dwQuestIdx, dwSubQuestIdx ) )
	{
		// questmgr
		QUESTMGR->StartSubQuest( m_pPlayer, dwQuestIdx, dwSubQuestIdx );
	}
}

void CQuestGroup::EndSubQuest( DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( pQuest )	
	if( pQuest->EndSubQuest( dwQuestIdx, dwSubQuestIdx ) )
	{
		// questmgr
		if( dwSubQuestIdx == 0 )
			QUESTMGR->StartQuest( m_pPlayer, dwQuestIdx, dwSubQuestIdx );
		else
			QUESTMGR->EndSubQuest( m_pPlayer, dwQuestIdx, dwSubQuestIdx );
	}
}

void CQuestGroup::ChangeSubQuestValue( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwKind )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( pQuest )
	{
		pQuest->ChangeSubQuestValue( dwSubQuestIdx, dwKind );	

		// questmgr
		QUESTMGR->UpdateSubQuestData( m_pPlayer, dwQuestIdx, dwSubQuestIdx, dwKind );

		// event
		CQuestEvent qe;
		qe.SetValue( eQuestEvent_Count, dwSubQuestIdx, pQuest->GetSubQuestData(dwSubQuestIdx) );
		QUESTMGR->AddQuestEvent( m_pPlayer, &qe, dwQuestIdx );
		//
	}
}

void CQuestGroup::EndOtherQuest( DWORD dwOtherQuestIdx, DWORD dwRepeat )
{
	CQuest* pQuest = m_QuestTable.GetData( dwOtherQuestIdx );
	if( pQuest )
	if( pQuest->EndQuest( dwOtherQuestIdx, dwRepeat ) )
	{
		QUESTMGR->EndQuest( m_pPlayer, dwOtherQuestIdx, dwRepeat );
	}
}

void CQuestGroup::SaveLoginPoint( DWORD dwLoginPointMap )
{
	if( dwLoginPointMap > 2000 )
		return;

	SavePointUpdate( m_pPlayer->GetID(), (WORD)(dwLoginPointMap+2000), (WORD)dwLoginPointMap );

	m_pPlayer->InitMapChangePoint();
	m_pPlayer->SetLoginPoint( (WORD)(dwLoginPointMap+2000) );
}

void CQuestGroup::Process()
{
	if( m_pPlayer == NULL )			return;
	if( m_dwQuestEventNum <= 0 )	return;

	CQuest* pQuest = NULL;
	m_QuestTable.SetPositionHead();
	while( pQuest = m_QuestTable.GetData() )
	{
		for( DWORD i = 0; i < m_dwQuestEventNum; ++i )
		{
			if( !pQuest->IsQuestComplete() )
				pQuest->OnQuestEvent( m_pPlayer, this, &m_QuestEvent[i].QuestEvent, m_QuestEvent[i].dwQuestIdx );
		}
	}

	memset( m_QuestEvent, 0, sizeof(GQUESTEVENT)*MAX_QUESTEVENT_PLYER );
	m_dwQuestEventNum = 0;
}

int CQuestGroup::GetProcessQuestCount()
{
	int count = 0;
	
	CQuest* pQuest = NULL;
	int i = 1;
	m_QuestTable.SetPositionHead();
	while( pQuest = m_QuestTable.GetData() )
	{
		if( pQuest->GetSubQuestFlag() /*SubQuestFlag값이 뭘 의미하는지 불명*/ && !(pQuest->IsQuestComplete()) )
			count++;

		i++;
	}

	return count;
}

void CQuestGroup::AddCount( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwMaxCount )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( pQuest )
	{
		pQuest->ChangeSubQuestValue( dwSubQuestIdx, eQuestValue_Add );	

		// questmgr
		QUESTMGR->UpdateSubQuestData( m_pPlayer, dwQuestIdx, dwSubQuestIdx, dwMaxCount );

		// event
		CQuestEvent qe;
		qe.SetValue( eQuestEvent_Count, dwSubQuestIdx, pQuest->GetSubQuestData(dwSubQuestIdx) );
		QUESTMGR->AddQuestEvent( m_pPlayer, &qe, dwQuestIdx );
		//
	}
}

void CQuestGroup::AddCountFromWeapon( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwMaxCount, DWORD dwWeaponKind )
{
	WORD wWeapon = m_pPlayer->GetWearedWeapon();
	if( ITEMMGR->GetWeaponKind( wWeapon ) != dwWeaponKind )
		return;

	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( pQuest )
	{
		pQuest->ChangeSubQuestValue( dwSubQuestIdx, eQuestValue_Add );	

		// questmgr
		QUESTMGR->UpdateSubQuestData( m_pPlayer, dwQuestIdx, dwSubQuestIdx, dwMaxCount );

		// event
		CQuestEvent qe;
		qe.SetValue( eQuestEvent_Count, dwSubQuestIdx, pQuest->GetSubQuestData(dwSubQuestIdx) );
		QUESTMGR->AddQuestEvent( m_pPlayer, &qe, dwQuestIdx );
		//
	}
}

void CQuestGroup::AddCountFromQWeapon( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwMaxCount, DWORD dwWeaponKind )
{
	if( m_pPlayer->GetWearedWeapon() != dwWeaponKind )
		return;

	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( pQuest )
	{
		pQuest->ChangeSubQuestValue( dwSubQuestIdx, eQuestValue_Add );	

		// questmgr
		QUESTMGR->UpdateSubQuestData( m_pPlayer, dwQuestIdx, dwSubQuestIdx, dwMaxCount );

		// event
		CQuestEvent qe;
		qe.SetValue( eQuestEvent_Count, dwSubQuestIdx, pQuest->GetSubQuestData(dwSubQuestIdx) );
		QUESTMGR->AddQuestEvent( m_pPlayer, &qe, dwQuestIdx );
		//
	}
}

void CQuestGroup::AddCountFromLevelGap( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwMaxCount, DWORD dwMinLevel, DWORD dwMaxLevel )
{
	int nMin = m_pPlayer->GetLevel() - m_pPlayer->GetKillMonsterLevel();
	int nMax = m_pPlayer->GetKillMonsterLevel() - m_pPlayer->GetLevel();
	if( nMin > 0 && (DWORD)nMin > dwMinLevel )	return;
	if( nMax > 0 && (DWORD)nMax > dwMaxLevel )	return;

	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( pQuest )
	{
		pQuest->ChangeSubQuestValue( dwSubQuestIdx, eQuestValue_Add );	

		// questmgr
		QUESTMGR->UpdateSubQuestData( m_pPlayer, dwQuestIdx, dwSubQuestIdx, dwMaxCount );

		// event
		CQuestEvent qe;
		qe.SetValue( eQuestEvent_Count, dwSubQuestIdx, pQuest->GetSubQuestData(dwSubQuestIdx) );
		QUESTMGR->AddQuestEvent( m_pPlayer, &qe, dwQuestIdx );
		//
	}
}

void CQuestGroup::AddCountFromMonLevel( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwMaxCount, DWORD dwMinLevel, DWORD dwMaxLevel )
{
	WORD wMonLevel = m_pPlayer->GetKillMonsterLevel();
	if( wMonLevel < dwMinLevel )	return;
	if( wMonLevel > dwMaxLevel )	return;

	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( pQuest )
	{
		pQuest->ChangeSubQuestValue( dwSubQuestIdx, eQuestValue_Add );	

		// questmgr
		QUESTMGR->UpdateSubQuestData( m_pPlayer, dwQuestIdx, dwSubQuestIdx, dwMaxCount );

		// event
		CQuestEvent qe;
		qe.SetValue( eQuestEvent_Count, dwSubQuestIdx, pQuest->GetSubQuestData(dwSubQuestIdx) );
		QUESTMGR->AddQuestEvent( m_pPlayer, &qe, dwQuestIdx );
		//
	}
}

void CQuestGroup::GiveQuestItem( PLAYERTYPE* pPlayer, DWORD dwQuestIdx, DWORD dwItemIdx, DWORD dwItemNum )
{
	QUESTITEM* pQuestItem = m_QuestItemTable.GetData( dwItemIdx );
	if( pQuestItem )
	{
		// Au½ºÆ®¾ÆAIAU; npcCNA× AU°æ¿i´A npc¿IAC ´eE­AI¶§ ≫O
		// AIºI¸| AU °æ¿i´A ¾ø8¹C·I ¹≪v°C ´U ¾ø¾Ø´U...
		m_QuestItemTable.Remove( pQuestItem->dwItemIdx );
		QUESTMGR->GiveQuestItem( pPlayer, pQuestItem->dwItemIdx, 0 );
		delete pQuestItem;

/*		if( pQuestItem->dwItemNum >= dwItemNum )
			pQuestItem->dwItemNum -= dwItemNum;
		else
			pQuestItem->dwItemIdx = 0;

		if( pQuestItem->dwItemNum > 0 )
			QUESTMGR->UpdateQuestItem( pPlayer, dwQuestIdx, pQuestItem->dwItemIdx, pQuestItem->dwItemNum );
		else
		{
			m_QuestItemTable.Remove( pQuestItem->dwItemIdx );
			QUESTMGR->GiveQuestItem( pPlayer, pQuestItem->dwItemIdx, 0 );
			delete pQuestItem;
		}
		*/
	}
}

void CQuestGroup::TakeQuestItem( PLAYERTYPE* pPlayer, DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwItemIdx, DWORD dwItemNum, DWORD dwProbability )
{
	if( CheckQuestProbability(dwProbability) == FALSE )
		return;

	ChangeSubQuestValue( dwQuestIdx, dwSubQuestIdx, eQuestValue_Add );

	// ±ax¿¡ Au½ºÆ® ¾ÆAIAUAI AO´AAo...
	QUESTITEM* pQuestItem = m_QuestItemTable.GetData( dwItemIdx );
	if( pQuestItem )
	{
		pQuestItem->dwItemNum += dwItemNum;
        DWORD dwValue = GetSubQuestValue( dwQuestIdx, dwSubQuestIdx );
		if( pQuestItem->dwItemNum != dwValue )
		{
			if( pQuestItem->dwItemNum > dwValue )
				pQuestItem->dwItemNum = dwValue;
		}
		if( pQuestItem->dwItemNum > 100 )
			pQuestItem->dwItemNum = 100;
		QUESTMGR->UpdateQuestItem( pPlayer, dwQuestIdx, pQuestItem->dwItemIdx, pQuestItem->dwItemNum );
	}
	else	// ¾ø´U¸e
	{
		pQuestItem = new QUESTITEM;
		pQuestItem->dwQuestIdx = dwQuestIdx;
		pQuestItem->dwItemIdx = dwItemIdx;
		pQuestItem->dwItemNum = dwItemNum;
		m_QuestItemTable.Add( pQuestItem, pQuestItem->dwItemIdx );

		QUESTMGR->TakeQuestItem( pPlayer, pQuestItem->dwQuestIdx, pQuestItem->dwItemIdx, pQuestItem->dwItemNum );
	}
}

void CQuestGroup::TakeQuestItemFromQWeapon( PLAYERTYPE* pPlayer, DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwItemIdx, DWORD dwItemNum, DWORD dwProbability, DWORD dwWeaponKind )
{
	if( pPlayer->GetWearedWeapon() != dwWeaponKind )
		return;
	if( CheckQuestProbability(dwProbability) == FALSE )
		return;

	ChangeSubQuestValue( dwQuestIdx, dwSubQuestIdx, eQuestValue_Add );

	// ±ax¿¡ Au½ºÆ® ¾ÆAIAUAI AO´AAo...
	QUESTITEM* pQuestItem = m_QuestItemTable.GetData( dwItemIdx );
	if( pQuestItem )
	{
		int nValue = GetSubQuestValue( dwQuestIdx, dwSubQuestIdx );

		pQuestItem->dwItemNum = nValue;
		QUESTMGR->UpdateQuestItem( pPlayer, dwQuestIdx, pQuestItem->dwItemIdx, pQuestItem->dwItemNum );
	}
	else	// ¾ø´U¸e
	{
		pQuestItem = new QUESTITEM;
		pQuestItem->dwQuestIdx = dwQuestIdx;
		pQuestItem->dwItemIdx = dwItemIdx;
		pQuestItem->dwItemNum = dwItemNum;
		m_QuestItemTable.Add( pQuestItem, pQuestItem->dwItemIdx );

		QUESTMGR->TakeQuestItem( pPlayer, pQuestItem->dwQuestIdx, pQuestItem->dwItemIdx, pQuestItem->dwItemNum );
	}
}

void CQuestGroup::TakeQuestItemFromWeapon( PLAYERTYPE* pPlayer, DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwItemIdx, DWORD dwItemNum, DWORD dwProbability, DWORD dwWeaponKind )
{
	WORD wWeapon = pPlayer->GetWearedWeapon();
	if( ITEMMGR->GetWeaponKind( wWeapon ) != dwWeaponKind )
		return;

	if( CheckQuestProbability(dwProbability) == FALSE )
		return;

	ChangeSubQuestValue( dwQuestIdx, dwSubQuestIdx, eQuestValue_Add );

	// ±ax¿¡ Au½ºÆ® ¾ÆAIAUAI AO´AAo...
	QUESTITEM* pQuestItem = m_QuestItemTable.GetData( dwItemIdx );
	if( pQuestItem )
	{
		int nValue = GetSubQuestValue( dwQuestIdx, dwSubQuestIdx );

        pQuestItem->dwItemNum = nValue;
		QUESTMGR->UpdateQuestItem( pPlayer, dwQuestIdx, pQuestItem->dwItemIdx, pQuestItem->dwItemNum );
	}
	else	// ¾ø´U¸e
	{
		pQuestItem = new QUESTITEM;
		pQuestItem->dwQuestIdx = dwQuestIdx;
		pQuestItem->dwItemIdx = dwItemIdx;
		pQuestItem->dwItemNum = dwItemNum;
		m_QuestItemTable.Add( pQuestItem, pQuestItem->dwItemIdx );

		QUESTMGR->TakeQuestItem( pPlayer, pQuestItem->dwQuestIdx, pQuestItem->dwItemIdx, pQuestItem->dwItemNum );
	}
}

void CQuestGroup::GiveItem( PLAYERTYPE* pPlayer, WORD wItemIdx, DURTYPE dwItemNum )
{
	MSG_EXCHANGE_REMOVEITEM Msg;
	Msg.Category = MP_QUEST;
	Msg.Protocol = MP_QUEST_GIVEITEM_ACK;

	int i = 0;
	ITEM_TOTALINFO ItemInfo;
	ITEMBASE logitem;
	pPlayer->GetItemtotalInfo( &ItemInfo, GETITEM_FLAG_INVENTORY|GETITEM_FLAG_WEAR );
	for( i = 0; i < SLOT_INVENTORY_NUM;	++i )
	{
		if( ItemInfo.Inventory[i].wIconIdx == wItemIdx )
		{
			logitem.Durability = ItemInfo.Inventory[i].Durability;
			logitem.dwDBIdx = ItemInfo.Inventory[i].dwDBIdx;
			logitem.Position = ItemInfo.Inventory[i].Position;
			logitem.wIconIdx = ItemInfo.Inventory[i].wIconIdx;
			// magi82(28)
			logitem.ItemParam = 0;
			logitem.QuickPosition = 0;
			logitem.RareIdx = 0;
			
			if( EI_TRUE == ITEMMGR->DiscardItem( pPlayer, ItemInfo.Inventory[i].Position, wItemIdx, dwItemNum ) )
			{

				// Log
				LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
					eLog_ItemDestroyGiveQuest, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
					logitem.wIconIdx , logitem.dwDBIdx, logitem.Position, 0, logitem.Durability, pPlayer->GetPlayerExpPoint());
				
				Msg.wAbsPosition = ItemInfo.Inventory[i].Position;
				pPlayer->SendMsg( &Msg, sizeof(Msg) );
			}
		}
	}
	for( i = 0; i < SLOT_WEAR_NUM; ++i )
	{
		if( ItemInfo.WearedItem[i].wIconIdx == wItemIdx )
		{
			logitem.Durability = ItemInfo.Inventory[i].Durability;
			logitem.dwDBIdx = ItemInfo.Inventory[i].dwDBIdx;
			logitem.Position = ItemInfo.Inventory[i].Position;
			logitem.wIconIdx = ItemInfo.Inventory[i].wIconIdx;
			// magi82(28)
			logitem.ItemParam = 0;
			logitem.QuickPosition = 0;
			logitem.RareIdx = 0;
			
			if( EI_TRUE == ITEMMGR->DiscardItem( pPlayer, ItemInfo.WearedItem[i].Position, wItemIdx, dwItemNum ) )
			{
				// Log
				LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
					eLog_ItemDestroyGiveQuest, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
					logitem.wIconIdx , logitem.dwDBIdx, logitem.Position, 0, logitem.Durability, pPlayer->GetPlayerExpPoint());

				Msg.wAbsPosition = ItemInfo.WearedItem[i].Position;
				pPlayer->SendMsg( &Msg, sizeof(Msg) );
			}
		}
	}
}

void CQuestGroup::GiveMoney( PLAYERTYPE* pPlayer, DWORD dwMoney )
{
	MONEYTYPE money = pPlayer->SetMoney( dwMoney, MONEY_SUBTRACTION );

	// network send
	MSG_DWORD Msg;
	Msg.Category = MP_QUEST;
	Msg.Protocol = MP_QUEST_GIVEMONEY_ACK;
	Msg.dwObjectID = pPlayer->GetID();
	Msg.dwData = money;
	pPlayer->SendMsg(&Msg, sizeof(Msg));
}

BOOL CQuestGroup::TakeItem( PLAYERTYPE* pPlayer, WORD wItemIdx, DWORD dwItemNum, DWORD dwProbability )
{
	if( CheckQuestProbability(dwProbability) == FALSE )
		return TRUE;

	if( !ITEMMGR->GetItemInfo( wItemIdx ) )	return TRUE;

	if( ITEMMGR->ObtainItemFromQuest( pPlayer, wItemIdx, dwItemNum ) == 1 )	return FALSE;

	return TRUE;
}

void CQuestGroup::TakeMoney( PLAYERTYPE* pPlayer, DWORD dwMoney )
{
	MONEYTYPE money = pPlayer->SetMoney( dwMoney, MONEY_ADDITION );

	// log
	InsertLogMoney( eMoneyLog_GetFromQuest, pPlayer->GetID(), pPlayer->GetMoney(), dwMoney, 0, 0 );

	// network send
	MSG_DWORD Msg;
	Msg.Category = MP_QUEST;
	Msg.Protocol = MP_QUEST_TAKEMONEY_ACK;
	Msg.dwObjectID = pPlayer->GetID();
	Msg.dwData = money;
	pPlayer->SendMsg(&Msg, sizeof(Msg));
}

void CQuestGroup::TakeExp( PLAYERTYPE* pPlayer, DWORD dwExp )
{
	pPlayer->SetPlayerExpPoint( pPlayer->GetPlayerExpPoint() + dwExp );
	//log
	InsertLogExp( eExpLog_GetbyQuest, pPlayer->GetID(), pPlayer->GetLevel(), dwExp, pPlayer->GetPlayerExpPoint(),
		0, 0, pPlayer->GetPlayerAbilityExpPoint() );

	// network send
	MSG_DWORD Msg;
	Msg.Category = MP_QUEST;
	Msg.Protocol = MP_QUEST_TAKEEXP_ACK;
	Msg.dwObjectID = pPlayer->GetID();
	Msg.dwData = dwExp;
	pPlayer->SendMsg(&Msg, sizeof(Msg));
}

void CQuestGroup::TakeSExp( PLAYERTYPE* pPlayer, DWORD dwSExp )
{
	pPlayer->AddAbilityExp( dwSExp );
	//log
	InsertLogExp( eExpLog_SExpGetbyQuest, pPlayer->GetID(), pPlayer->GetLevel(), dwSExp, pPlayer->GetPlayerExpPoint(),
		0, 0, pPlayer->GetPlayerAbilityExpPoint() );

	// network send
	MSG_DWORD Msg;
	Msg.Category = MP_QUEST;
	Msg.Protocol = MP_QUEST_TAKESEXP_ACK;
	Msg.dwObjectID = pPlayer->GetID();
	Msg.dwData = dwSExp;
	pPlayer->SendMsg(&Msg, sizeof(Msg));
}

void CQuestGroup::TakeMoneyPerCount( PLAYERTYPE* pPlayer, DWORD dwItemIdx, DWORD dwMoney )
{
	QUESTITEM* pQuestItem = m_QuestItemTable.GetData( dwItemIdx );
	if( pQuestItem )
	{
		if( pQuestItem->dwItemNum > 0 )		// Item이 하나라도 있다면...
		{
			MONEYTYPE total = pQuestItem->dwItemNum * dwMoney;

			// quest item 지움
			m_QuestItemTable.Remove( pQuestItem->dwItemIdx );
			QUESTMGR->GiveQuestItem( pPlayer, pQuestItem->dwItemIdx, 0 );
			delete pQuestItem;

			// money 계산
			MONEYTYPE money = pPlayer->SetMoney( total, MONEY_ADDITION );
			
			MSG_DWORD Msg;
			Msg.Category = MP_QUEST;
			Msg.Protocol = MP_QUEST_TAKEMONEY_ACK;
			Msg.dwObjectID = pPlayer->GetID();
			Msg.dwData = money;
			pPlayer->SendMsg(&Msg, sizeof(Msg));
		}
		else
		{
		}
	}
	else	// Item이 없으면...
	{
	}
}

void CQuestGroup::RegenMonster( DWORD dwRegenType )
{
	QUESTREGENMGR->MonsterRegen( m_pPlayer, dwRegenType );
}

void CQuestGroup::MapChange( WORD wCurMap, WORD wReturnMap )
{
	m_pPlayer->SetDieInSpecialMap( TRUE, wReturnMap );
}

void CQuestGroup::ChangeStage( BYTE bStage )
{
	BYTE stage = m_pPlayer->GetStage();
	if( stage == bStage )	return;

	switch( bStage )
	{
	case eStage_Hwa:
		{
			if( stage != eStage_Normal )	return;
		}
		break;
	case eStage_Hyun:
		{
			if( stage != eStage_Hwa )	return;
		}
		break;
	case eStage_Geuk:
		{
			if( stage != eStage_Normal )	return;
		}
		break;
	case eStage_Tal:
		{
			if( stage != eStage_Geuk )	return;
		}
		break;
	}
	
	m_pPlayer->SetStage( bStage );
}

void CQuestGroup::RegistCheckTime( DWORD dwQuestIdx, DWORD dwSubQuestIdx, BYTE bType, DWORD dwDay, DWORD dwHour, DWORD dwMin )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( !pQuest )	return;

	BOOL bStart = TRUE;
	stTIME curTime, regTime;
	curTime.value = GetCurTime();
	if( bType == 1 )		// 고정 시간
	{		
		if( curTime.GetHour() > dwHour )
		{
			regTime.SetTime( 0, 0, dwDay, 0, 0, 0 );
			curTime += regTime;
			regTime.SetTime( curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(), dwHour, dwMin, 0 );
		}
		else if( curTime.GetHour() == dwHour )
		{
			if( curTime.GetMinute() < dwMin )
			{
				regTime.SetTime( curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(), dwHour, dwMin, 0 );
			}
			else
			{
				regTime.SetTime( 0, 0, dwDay, 0, 0, 0 );
				curTime += regTime;
				regTime.SetTime( curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(), dwHour, dwMin, 0 );
			}
		}
		else
		{
			regTime.SetTime( curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(), dwHour, dwMin, 0 );
		}
	}
	else if( bType == 2 )	// 일정 시간후
	{
		regTime.SetTime( 0, 0, dwDay, dwHour, dwMin, 0 );
		curTime += regTime;
		regTime = curTime;

//		regTime.SetTime( curTime.GetYear(), curTime.GetMonth(), curTime.GetDay()+dwDay, curTime.GetHour()+dwHour, curTime.GetMinute()+dwMin, 0 );
//		regTime += curTime;
	}
	else if( bType == 3 )	// 일정 시간후 실패
	{
		regTime.SetTime( 0, 0, dwDay, dwHour, dwMin, 0 );
		curTime += regTime;
		regTime = curTime;

//		regTime.SetTime( curTime.GetYear(), curTime.GetMonth(), curTime.GetDay()+dwDay, curTime.GetHour()+dwHour, curTime.GetMinute()+dwMin, 0 );
//		regTime += curTime;
	}
	else if( bType == 4 )	// 고정 시간에 실패
	{
		regTime.SetTime( curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(), dwHour, dwMin, 0 );
	}
	else
	{
		regTime.value = 0;
		bStart = FALSE;
	}

	pQuest->RegistCheckTime( bStart, bType, regTime );	
}

DWORD CQuestGroup::GetSubQuestValue( DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( !pQuest )	return -1;

	return pQuest->GetSubQuestData( dwSubQuestIdx );
}


#ifdef _JAPAN_LOCAL_
void CQuestGroup::ChangeSubAttr( WORD dwSubAttr )
{	
	MSG_DWORD Msg;
	Msg.Category = MP_CHAR;
	if( m_pPlayer->ChangeSubCharAttr( dwSubAttr ) )
		Msg.Protocol = MP_CHAR_CHANGE_SUBATTR_ACK;
	else
		Msg.Protocol = MP_CHAR_CHANGE_SUBATTR_NACK;
	Msg.dwData = MAKEDWORD(m_pPlayer->GetMainCharAttr(),m_pPlayer->GetSubCharAttr());

	m_pPlayer->SendMsg( &Msg, sizeof(Msg) );
}
#endif
