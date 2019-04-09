// QuestManager.cpp: implementation of the CQuestManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#include "MHFile.h"
#include "..\[CC]Quest\QuestScriptLoader.h"
#include "..\[CC]Quest\QuestString.h"
#include "..\[CC]Quest\QuestInfo.h"
#include "..\[CC]Quest\SubQuestInfo.h"
#include "..\[CC]Quest\QuestNpc.h"
#include "..\[CC]Quest\QuestNpcData.h"
#include "Quest.h"
#include "QuestUpdater.h"
#include "ObjectManager.h"
#include "ChatManager.h"
#include "QuestItemManager.h"

//
#include "GameIn.h"
#include "ItemManager.h"
#include "InventoryExDialog.h"
#include "WindowIdEnum.h"
#include "./Interface/cWindowManager.h"
#include "cDivideBox.h"
#include "cMsgBox.h"
#include "QuickManager.h"
#include "Npc.h"
#include "MHMap.h"

#include "MiniMapDlg.h"

#include "MainBarDialog.h"
#include "QuestDialog.h"
#include "StatusIconDlg.h"
#include "NpcScriptDialog.h"

#include "cMsgBox.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CQuestManager);

CQuestManager::CQuestManager()
{
	m_QuestInfoTable.Initialize(MAX_QUEST);
	m_QuestStringTable.Initialize(200);
	m_QuestTable.Initialize(MAX_QUEST);
	m_QuestNpcTable.Initialize(100);

	m_QuestUpdater = new CQuestUpdater;

	memset( m_pNpcArray, 0, sizeof(CNpc*)*32 );
	m_dwMaxNpc = 0;
	
	m_dwLastMsg = 0;
}

CQuestManager::~CQuestManager()
{
	Release();
}

void CQuestManager::Release()
{
	// quest info
	CQuestInfo* pQuestInfo = NULL;
	m_QuestInfoTable.SetPositionHead();
	while( pQuestInfo = m_QuestInfoTable.GetData() )
	{
		delete pQuestInfo;
	}
	m_QuestInfoTable.RemoveAll();

	// quest string
	CQuestString* pQuestString = NULL;
	m_QuestStringTable.SetPositionHead();
	while( pQuestString = m_QuestStringTable.GetData() )
	{
		delete pQuestString;
	}
	m_QuestStringTable.RemoveAll();

	// quest
	CQuest* pQuest = NULL;
	m_QuestTable.SetPositionHead();
	while( pQuest = m_QuestTable.GetData() )
	{
		delete pQuest;
	}
	m_QuestTable.RemoveAll();

	// quest npcinfo
	QUESTNPCINFO* pData = NULL;
	m_QuestNpcTable.SetPositionHead();
	while( pData = m_QuestNpcTable.GetData() )
	{
		delete pData;
	}
	m_QuestNpcTable.RemoveAll();

	// updater
	if( m_QuestUpdater )
	{
		delete m_QuestUpdater;	
		m_QuestUpdater = NULL;
	}

	m_dwLastMsg = 0;
}

BOOL CQuestManager::LoadQuestScript( char* strFileName )
{
	CMHFile file;
#ifdef _FILE_BIN_
	if( !file.Init( strFileName, "rb") )
		return FALSE;
#else
	if( !file.Init( strFileName, "rt") )
		return FALSE;
#endif

	char Token[256];
	DWORD dwQuestNum = 0;
	while( !file.IsEOF() )
	{
		file.GetString( Token );
		if( strcmp(Token,"$QUEST") == 0 )
		{
			dwQuestNum = file.GetDword();
			m_QuestInfoTable.Add( CQuestScriptLoader::LoadQuestInfo( &file, dwQuestNum ), dwQuestNum );
		}
	}

	CreateAllQuestFromScript();

	return TRUE;
}

BOOL CQuestManager::LoadQuestString(char* strFileName)
{
	CMHFile file;
#ifdef _FILE_BIN_
	if( !file.Init( strFileName, "rb") )
		return FALSE;
#else
	if( !file.Init( strFileName, "rt") )
		return FALSE;
#endif

	char Token[256];
	DWORD Key=0;
	DWORD Key_1=0;
	DWORD Key_2=0;
	CQuestString* pQuestString = NULL;
	
	while( !file.IsEOF() )
	{
		file.GetString(Token);

		if(strcmp(Token,"$SUBQUESTSTR") == 0)	
		{
			Key_1 = file.GetDword();
			Key_2 = file.GetDword();
			COMBINEKEY(Key_1, Key_2, Key);
			pQuestString = CQuestScriptLoader::LoadQuestString(&file);
			if(pQuestString)
			{
				m_QuestStringTable.Add(pQuestString, Key);
				pQuestString->SetIndex(Key_1, Key_2);				
			}
		}
	}

	//
	m_QuestItemMGR.QuestItemInfoLoad();

	return TRUE;
}

void CQuestManager::LoadNewbieGuide( char* strFileName )
{
	m_NewbieGuide.LoadNewbieGuide( strFileName );
}

void CQuestManager::LoadQuestNpc( char* strFileName )
{
	CMHFile file;
#ifdef _FILE_BIN_
	if( !file.Init( strFileName, "rb") )
		return;
#else
	if( !file.Init( strFileName, "rt") )
		return;
#endif

	while( !file.IsEOF() )
	{
		QUESTNPCINFO* pData = new QUESTNPCINFO;
		pData->dwObjIdx = 0;
		pData->wMapNum = file.GetWord();
		pData->wNpcKind = file.GetWord();
//		file.GetString( pData->sName );
		SafeStrCpy( pData->sName, file.GetString(), MAX_NAME_LENGTH+1 );
		pData->wNpcIdx = file.GetWord();
		pData->vPos.x = file.GetFloat();
		pData->vPos.y = 0;
		pData->vPos.z = file.GetFloat();
		pData->fDir = file.GetFloat();

		m_QuestNpcTable.Add( pData, pData->wNpcIdx );
	}
}

void CQuestManager::CreateAllQuestFromScript()
{
	CQuestInfo* pQuestInfo = NULL;
	m_QuestInfoTable.SetPositionHead();
	while( pQuestInfo = m_QuestInfoTable.GetData() )
	{
		CQuest* pQuest = new CQuest( pQuestInfo );
		m_QuestTable.Add( pQuest, pQuestInfo->GetQuestIdx() );
	}
}

void CQuestManager::InitializeQuest()
{
	CQuest* pQuest = NULL;
	m_QuestTable.SetPositionHead();
	while( pQuest = m_QuestTable.GetData() )
	{
		pQuest->Initialize();
	}
	m_dwMaxNpc = 0;

	m_dwLastMsg = 0;
}

void CQuestManager::SetMainQuestData( DWORD dwQuestIdx, DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime, BYTE bCheckType, DWORD dwCheckTime )
{
	if( dwQuestIdx == 0 )	// Newbieguide
	{
		m_NewbieGuide.SetFlagData( dwSubQuestFlag, dwData, dwTime );
		return;
	}
	else
	{
		CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
		if( pQuest )
		{
			pQuest->SetMainQuestData( dwSubQuestFlag, dwData, dwTime, bCheckType, dwCheckTime );
			
			if(pQuest->GetEndParam() == 0)
			{
				DWORD Key = 0;
				COMBINEKEY(dwQuestIdx, 0, Key);
				m_QuestUpdater->ProcessQuestAdd(m_QuestStringTable.GetData(Key));
			}
		}
	}
}

void CQuestManager::SetSubQuestData( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwData, DWORD dwTime )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( pQuest )
	{
		pQuest->SetSubQuestData( dwSubQuestIdx, dwData, dwTime );
		AddQuestNpc( pQuest->GetSubQuestInfo( dwSubQuestIdx ) );

		//
		int Key = 0;
		COMBINEKEY( dwQuestIdx, dwSubQuestIdx, Key );
		m_QuestUpdater->ProcessQuestAdd( m_QuestStringTable.GetData(Key) ); 
	}
}

void CQuestManager::StartQuest( DWORD dwQuestIdx, DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( pQuest )
	{
		pQuest->StartQuest( dwSubQuestFlag, dwData, dwTime );

		//
		int Key = 0;
		COMBINEKEY( dwQuestIdx, 0, Key );
		m_QuestUpdater->ProcessQuestAdd( m_QuestStringTable.GetData(Key) ); 

		// title
		CQuestString* pQString = m_QuestStringTable.GetData(Key);
		if( pQString == NULL )
			return;

		cPtrList* p = pQString->GetTitle();
		QString* string = (QString*)p->GetHead();
		
		CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(575), string->Str );
	}
}

void CQuestManager::EndQuest( DWORD dwQuestIdx, DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( pQuest )
	{
		// remove icon
		if( pQuest->IsTimeCheck() )
			STATUSICONDLG->RemoveQuestTimeIcon( HERO, eStatusIcon_QuestTime );

		pQuest->EndQuest( dwSubQuestFlag, dwData, dwTime );

		//
		DWORD Key = 0;
		COMBINEKEY(dwQuestIdx, 0, Key);
		if(dwSubQuestFlag == 0)
			m_QuestUpdater->CompleteQuestDelete(m_QuestStringTable.GetData(Key));

		stTIME stime;
		stime.value = dwTime;
		
		char buf[16] = { 0, };
		sprintf(buf, "20%02d.%02d.%02d", stime.GetYear(), stime.GetMonth(), stime.GetDay());
		
		JOURNALINFO pItem;
		pItem.Kind = eJournal_Quest;
		pItem.Param = dwQuestIdx;
		pItem.Param_2 = 0;				// SubQuestIdx가 0만 기록(메인퀘스트만 기록)
		pItem.Param_3 = 1;				// bCompleted
		strncpy(pItem.RegDate, buf, 11);
		m_QuestUpdater->JournalItemAdd(&pItem);
		
		
		// title
		CQuestString* pQString = m_QuestStringTable.GetData(Key);
		if( pQString == NULL )
			return;
		cPtrList* p = pQString->GetTitle();
		QString* string = (QString*)p->GetHead();
		
		CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(576), string->Str );		
	}
}

void CQuestManager::StartSubQuest( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwSubQuestFlag, DWORD dwSubData, DWORD dwSubTime )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( pQuest )
	{
		pQuest->StartSubQuest( dwSubQuestIdx, dwSubQuestFlag, dwSubData, dwSubTime );
		AddQuestNpc( pQuest->GetSubQuestInfo( dwSubQuestIdx ) );

		//
		int Key = 0;

		COMBINEKEY( dwQuestIdx, dwSubQuestIdx, Key );
		m_QuestUpdater->ProcessQuestAdd( m_QuestStringTable.GetData(Key) );

		if( gCurTime - m_dwLastMsg > 10000 )
		{
			m_dwLastMsg = gCurTime;
			CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(580));
		}
		if( !GAMEIN->GetQuestDialog()->IsActive() )
		{
			GAMEIN->GetMainInterfaceDialog()->SetAlram( OPT_QUESTDLGICON, TRUE );
		}
	}
}

void CQuestManager::EndSubQuest( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwSubQuestFlag, DWORD dwSubData, DWORD dwSubTime )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( pQuest )
	{
		pQuest->EndSubQuest( dwSubQuestIdx, dwSubQuestFlag, dwSubData, dwSubTime );

		//
		int Key = 0;
		COMBINEKEY( dwQuestIdx, dwSubQuestIdx, Key );
		m_QuestUpdater->ProcessQuestDelete(m_QuestStringTable.GetData(Key));
	}
}

void CQuestManager::UpdateSubQuestData( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwMaxCount, DWORD dwSubData, DWORD dwSubTime )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( pQuest )
	{
		pQuest->UpdateSubQuestData( dwSubQuestIdx, dwMaxCount, dwSubData, dwSubTime );

		//
		m_QuestUpdater->UpdateSubQuestData();
	}
}

void CQuestManager::QuestItemAdd( DWORD dwItemIdx, DWORD dwCount )
{
	QUEST_ITEM_INFO* pData = m_QuestItemMGR.GetQuestItem( dwItemIdx );
	if( !pData ) return;

	m_QuestUpdater->QuestItemAdd( pData, dwCount );

	// itemname
	CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(578), pData->ItemName );
}

void CQuestManager::QuestItemDelete( DWORD dwItemIdx )
{
	// itemname
	QUEST_ITEM_INFO* pData = m_QuestItemMGR.GetQuestItem( dwItemIdx );
	if( !pData ) return;
	CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(579), pData->ItemName );

	m_QuestUpdater->QuestItemDelete( dwItemIdx );
}

void CQuestManager::QuestItemUpdate( DWORD dwType, DWORD dwItemIdx, DWORD dwCount )
{
	m_QuestUpdater->QuestItemUpdate( dwType, dwItemIdx, dwCount);

	// itemname
	QUEST_ITEM_INFO* pData = m_QuestItemMGR.GetQuestItem( dwItemIdx );
	if( !pData ) return;
	CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(578), pData->ItemName );
}

BOOL CQuestManager::IsQuestComplete( DWORD dwQuestIdx )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( pQuest )
	{
		if( pQuest->IsQuestComplete() )
			return TRUE;
		else
			return FALSE;
	}

	return FALSE;
}


void CQuestManager::DeleteQuest( DWORD dwQuestIdx, BOOL bCheckTime )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( pQuest )
	{
		if( pQuest->IsQuestComplete() )
			return;
		//
		int Key = 0;
		COMBINEKEY( dwQuestIdx, 0, Key );
		CQuestString* pQString = m_QuestStringTable.GetData(Key);
		if( pQString == NULL )	return;
		cPtrList* p = pQString->GetTitle();
		QString* string = (QString*)p->GetHead();		

		if( pQuest->IsTimeCheck() && bCheckTime )
		{
			CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(1151), string->Str );
			STATUSICONDLG->RemoveQuestTimeIcon( HERO, eStatusIcon_QuestTime );
		}
		else
			CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(577), string->Str );

		pQuest->Initialize();
		m_QuestUpdater->DeleteQuest( dwQuestIdx );		
	}
}

BOOL CQuestManager::IsNpcRelationQuest( DWORD dwNpcIdx )
{
	CQuest* pQuest = NULL;
	m_QuestTable.SetPositionHead();
	while( pQuest = m_QuestTable.GetData() )
	{
		if( pQuest->IsNpcRelationQuest( dwNpcIdx ) )
			return TRUE;
	}
	return FALSE;
}

DWORD CQuestManager::GetNpcScriptPage( DWORD dwNpcIdx )
{
	DWORD dwPage = 0;
	CQuest* pQuest = NULL;
	m_QuestTable.SetPositionHead();
	while( pQuest = m_QuestTable.GetData() )
	{
		dwPage = pQuest->GetNpcScriptPage( dwNpcIdx );
		if( dwPage )
			return dwPage;
	}
	return 0;
}

void CQuestManager::SendNpcTalkEvent( DWORD dwNpcIdx, DWORD dwNpcScriptIdx )
{
	SEND_QUEST_IDX Msg;
	Msg.Category = MP_QUEST;
	Msg.Protocol = MP_QUESTEVENT_NPCTALK;
	Msg.dwObjectID = HERO->GetID();
	Msg.MainQuestIdx = (WORD)dwNpcIdx;
	Msg.SubQuestIdx = (WORD)dwNpcScriptIdx;
	NETWORK->Send( &Msg, sizeof(Msg) );
}

void CQuestManager::SendNpcTalkEventForEvent( DWORD dwNpcIdx, DWORD dwNpcScriptIdx )
{
	SEND_QUEST_IDX Msg;
	Msg.Category = MP_QUEST;
	Msg.Protocol = MP_QUESTEVENT_NPCTALK_SYN;
	Msg.dwObjectID = HERO->GetID();
	Msg.MainQuestIdx = (WORD)dwNpcIdx;
	Msg.SubQuestIdx = (WORD)dwNpcScriptIdx;
	NETWORK->Send( &Msg, sizeof(Msg) );
}

void CQuestManager::SetNpcData( CNpc* pNpc )
{
	if( m_dwMaxNpc > 31 )
		return;
	m_pNpcArray[m_dwMaxNpc] = pNpc;
	++m_dwMaxNpc;
}

void CQuestManager::ProcessNpcMark()
{
	if( HERO == NULL )	return;

	for( DWORD i = 0; i < m_dwMaxNpc; ++i )
	{
		if( m_pNpcArray[i] )
		{
			m_pNpcArray[i]->RemoveAllObjectEffect();
			if( GAMEIN->GetMiniMapDialog() )
				GAMEIN->GetMiniMapDialog()->ShowQuestMarkIcon( m_pNpcArray[i], eQM_NONE );
		}
	}

	DWORD dwNpcIdx = 0;
	DWORD dwNpcMarkType = 0;
	CQuest* pQuest = NULL;
	CObject* pObject = NULL;

	////======


	//desc.Position = Pos;
	////======

	m_QuestTable.SetPositionHead();
	while( pQuest = m_QuestTable.GetData() )
	{
		for( DWORD i = 0; i < m_dwMaxNpc; ++i )
		{
			if( dwNpcMarkType = pQuest->GetNpcMarkType( m_pNpcArray[i]->GetNpcUniqueIdx() ) )
			{
				if( dwNpcMarkType == 1 )		// !
				{
					pObject = OBJECTMGR->GetObject( m_pNpcArray[i]->GetID() );
					if( pObject )
					{
						VECTOR3 Pos;

						NPC_LIST* pNpcList = GAMERESRCMNGR->GetNpcInfo(((CNpc*)pObject)->GetNpcKind());
						Pos.x = Pos.z = 0.f;
						Pos.y = pNpcList->Tall - 135.f;		//135 임의 조절 값 by 기획팀 유정씨.

						if( pObject->IsNpcMark( NPCMARK_1_ID ) )
						{
//							m_pNpcArray[i]->RemoveObjectEffect( NPCMARK_1_ID );
						}
						else if( pObject->IsNpcMark( NPCMARK_2_ID ) )
						{
							m_pNpcArray[i]->RemoveObjectEffect( NPCMARK_2_ID );

							OBJECTEFFECTDESC desc(FindEffectNum("eff_NpcMark1.beff"));
							desc.Position = Pos;
							
							pObject->AddObjectEffect( NPCMARK_1_ID, &desc, 1, HERO );
							
							if( GAMEIN->GetMiniMapDialog() )
								GAMEIN->GetMiniMapDialog()->ShowQuestMarkIcon( pObject, eQM_EXCLAMATION );
						}
						else
						{
							OBJECTEFFECTDESC desc(FindEffectNum("eff_NpcMark1.beff"));
							desc.Position = Pos;

							pObject->AddObjectEffect( NPCMARK_1_ID, &desc, 1, HERO );

							if( GAMEIN->GetMiniMapDialog() )
								GAMEIN->GetMiniMapDialog()->ShowQuestMarkIcon( pObject, eQM_EXCLAMATION );
						}
					}
				}
				else if( dwNpcMarkType == 2 )	// ?
				{
					pObject = OBJECTMGR->GetObject( m_pNpcArray[i]->GetID() );
					if( pObject )
					{
						VECTOR3 Pos;

						NPC_LIST* pNpcList = GAMERESRCMNGR->GetNpcInfo(((CNpc*)pObject)->GetNpcKind());
						Pos.x = Pos.z = 0.f;
						Pos.y = pNpcList->Tall - 135.f;		//135 임의 조절 값 by 기획팀 유정씨.

						if( pObject->IsNpcMark( NPCMARK_1_ID ) )
						{
//							m_pNpcArray[i]->RemoveObjectEffect( NPCMARK_1_ID );
						}
						else if( pObject->IsNpcMark( NPCMARK_2_ID ) )
						{
						}
						else
						{
							OBJECTEFFECTDESC desc(FindEffectNum("eff_NpcMark2.beff"));
							desc.Position = Pos;

							pObject->AddObjectEffect( NPCMARK_2_ID, &desc, 1, HERO );

							if( GAMEIN->GetMiniMapDialog() )
								GAMEIN->GetMiniMapDialog()->ShowQuestMarkIcon( pObject, eQM_QUESTION );
						}
					}
				}
			}
		}
	}
}

void CQuestManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch(Protocol)
	{
	case MP_QUEST_FULL:
		{
			GAMEIN->GetNpcScriptDialog()->SetActive( FALSE );
			WINDOWMGR->MsgBox( MBI_QUEST_FULL, MBT_OK, CHATMGR->GetChatMsg(1689) );
		}
		break;
	case MP_QUEST_MAINDATA_LOAD:
		{
			SEND_MAINQUEST_DATA* pmsg = (SEND_MAINQUEST_DATA*)pMsg;
			if(pmsg->wCount==0)		break;
			
			for(int i=0; i<pmsg->wCount; i++)
			{
				SetMainQuestData( pmsg->QuestList[i].QuestIdx, pmsg->QuestList[i].state.value, pmsg->QuestList[i].EndParam,
					pmsg->QuestList[i].Time, pmsg->QuestList[i].CheckType, pmsg->QuestList[i].CheckTime );
			}
			
			ProcessNpcMark();
		}
		break;
	case MP_QUEST_SUBDATA_LOAD:
		{
			SEND_SUBQUEST_DATA* pmsg = (SEND_SUBQUEST_DATA*)pMsg;
			if(pmsg->wCount==0)		break;
			
			for(int i=0; i<pmsg->wCount; i++)
			{
				SetSubQuestData( pmsg->QuestList[i].QuestIdx, pmsg->QuestList[i].SubQuestIdx,
								 pmsg->QuestList[i].state, pmsg->QuestList[i].time );
			}

			ProcessNpcMark();
		}
		break;	
	case MP_QUEST_ITEM_LOAD:
		{
			SEND_QUESTITEM* pmsg = (SEND_QUESTITEM*)pMsg;
			if(pmsg->wCount==0)		break;

			for(int i=0; i<pmsg->wCount; i++)
			{			
				m_QuestUpdater->QuestItemAdd(m_QuestItemMGR.GetQuestItem(pmsg->ItemList[i].ItemIdx),
					pmsg->ItemList[i].Count);			
			}
		}
		break;
	case MP_QUEST_START_ACK:
		{
			SEND_QUEST_IDX* pmsg = (SEND_QUEST_IDX*)pMsg;
			StartQuest( pmsg->MainQuestIdx, pmsg->dwFlag, pmsg->dwData, pmsg->dwTime );
		}		
		break;
	case MP_SUBQUEST_START_ACK:
		{
			SEND_QUEST_IDX* pmsg = (SEND_QUEST_IDX*)pMsg;		
			StartSubQuest( pmsg->MainQuestIdx, pmsg->SubQuestIdx, pmsg->dwFlag, pmsg->dwData, pmsg->dwTime );

			ProcessNpcMark();
		}
		break;
	case MP_QUEST_END_ACK:
		{
			SEND_QUEST_IDX* msg = (SEND_QUEST_IDX*)pMsg;
			EndQuest( msg->MainQuestIdx, msg->dwFlag, msg->dwData, msg->dwTime );

			QuestAbandonClose();

			ProcessNpcMark();
		}
		break;
	case MP_SUBQUEST_END_ACK:
		{
			SEND_QUEST_IDX* msg = (SEND_QUEST_IDX*)pMsg;
			EndSubQuest( msg->MainQuestIdx, msg->SubQuestIdx, msg->dwFlag, msg->dwData, msg->dwTime );

			QuestAbandonClose();

			ProcessNpcMark();
		}
		break;
	case MP_SUBQUEST_UPDATE:
		{
			SEND_SUBQUEST_UPDATE* msg = (SEND_SUBQUEST_UPDATE*)pMsg;
			UpdateSubQuestData( msg->wQuestIdx, msg->wSubQuestIdx, msg->dwMaxCount, msg->dwData, msg->dwTime );
		}
		break;	
	case MP_QUESTITEM_GIVE:
		{
			SEND_QUESTITEM_IDX* pmsg = (SEND_QUESTITEM_IDX*)pMsg;
			QuestItemDelete( pmsg->dwItemIdx );
		}
		break;
	case MP_QUESTITEM_TAKE:
		{
			SEND_QUESTITEM_IDX* pmsg = (SEND_QUESTITEM_IDX*)pMsg;
			QuestItemAdd( pmsg->dwItemIdx, pmsg->dwItemNum );

			// magi82(4) - Titan(071023) 타이탄일때 이펙트 처리(퀘스트아이템 획득)
//			if( HERO->InTitan() == TRUE )
//				EFFECTMGR->StartHeroEffectProcess(eEffect_Titan_GetItem_Quest);
//			else
//				EFFECTMGR->StartHeroEffectProcess(eEffect_GetItem_Quest);
//			ITEMMGR->ItemDropEffect( pmsg->dwItemIdx );
		}
		break;
	case MP_QUESTITEM_UPDATE:
		{
			SEND_QUESTITEM_IDX* pmsg = (SEND_QUESTITEM_IDX*)pMsg;
			QuestItemUpdate( eQItem_SetCount, pmsg->dwItemIdx, pmsg->dwItemNum );

			// magi82(4) - Titan(071023) 타이탄일때 이펙트 처리(퀘스트아이템 획득)
//			if( HERO->InTitan() == TRUE )
//				EFFECTMGR->StartHeroEffectProcess(eEffect_Titan_GetItem_Quest);
//			else
//				EFFECTMGR->StartHeroEffectProcess(eEffect_GetItem_Quest);
//			ITEMMGR->ItemDropEffect( pmsg->dwItemIdx );
		}
		break;
	case MP_QUEST_DELETE_ACK:
		{
			SEND_QUEST_IDX* pmsg = (SEND_QUEST_IDX*)pMsg;
			DeleteQuest( pmsg->MainQuestIdx, (BOOL)pmsg->dwFlag );

			ProcessNpcMark();
		}
		break;
	case MP_QUEST_DELETE_CONFIRM_ACK:
		{
			SEND_QUEST_IDX_SIMPLE* pmsg = (SEND_QUEST_IDX_SIMPLE*)pMsg;
			
			m_QuestUpdater->DeleteQuest(pmsg->MainQuestIdx);
		}
		break;
	case MP_QUEST_TAKEITEM_ACK:	// 
		{
			ITEMOBTAINARRAY * pmsg = (ITEMOBTAINARRAY *)pMsg;		
			CItem* pItem = NULL;
			for( int i = 0; i < pmsg->ItemNum; ++i )
			{
				if( pItem = GAMEIN->GetInventoryDialog()->GetItemForPos( pmsg->GetItem(i)->Position ) )
				{
					pItem->SetItemBaseInfo(pmsg->GetItem(i));
				}
				else
				{
					pItem = ITEMMGR->MakeNewItem(pmsg->GetItem(i),"MP_QUEST_TAKEITEM_ACK");
					BOOL rt = GAMEIN->GetInventoryDialog()->AddItem(pItem);
					if(!rt)
					{
						ASSERT(0);
					}
				}
			}
			// 
			cDialog* pDlg = WINDOWMGR->GetWindowForID( DBOX_DIVIDE_INV );
			if( pDlg )
			{
				((cDivideBox*)pDlg)->ExcuteDBFunc( 0 );	
			}

			QUICKMGR->RefreshQickItem();

			if( !pItem ) return;
			if( pItem->GetItemKind() == eQUEST_ITEM_EQUIP )
				CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(656), pItem->GetItemInfo()->ItemName );
			else
				CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(607), pItem->GetItemInfo()->ItemName );

			// magi82(4) - Titan(071023) 타이탄일때 이펙트 처리(퀘스트아이템 획득)
//			if( HERO->InTitan() == TRUE )
//				EFFECTMGR->StartHeroEffectProcess(eEffect_Titan_GetItem_Quest);
//			else
//				EFFECTMGR->StartHeroEffectProcess(eEffect_GetItem_Quest);
//			ITEMMGR->ItemDropEffect( pItem->GetItemIdx() );
		}
		break;
	case MP_QUEST_TAKEMONEY_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(608), AddComma(pmsg->dwData) );
			// magi82(4) - Titan(071023) 타이탄일때 이펙트 처리(돈 획득)
//			if( HERO->InTitan() == TRUE )
//				EFFECTMGR->StartHeroEffectProcess(eEffect_Titan_GetMoney);
//			else
//				EFFECTMGR->StartHeroEffectProcess(eEffect_GetMoney);
		}
		break;
	case MP_QUEST_TAKEEXP_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(609), pmsg->dwData );
		}
		break;
	case MP_QUEST_TAKESEXP_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(610), pmsg->dwData );
		}
		break;
	case MP_QUEST_GIVEITEM_ACK:
		{
			MSG_EXCHANGE_REMOVEITEM* pmsg = (MSG_EXCHANGE_REMOVEITEM*)pMsg;
			CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos( pmsg->wAbsPosition ); 
			WORD wTableIdx = ITEMMGR->GetTableIdxForAbsPos( pmsg->wAbsPosition );
			const ITEMBASE* pItemInfo = ITEMMGR->GetItemInfoAbsIn( HERO, pmsg->wAbsPosition );
			ITEMMGR->DeleteItemofTable( wTableIdx, pmsg->wAbsPosition );
			QUICKMGR->RefreshQickItem();

			if( !pItem ) return;
			CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(657), pItem->GetItemInfo()->ItemName );
		}
		break;
	case MP_QUEST_GIVEMONEY_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(608), AddComma(pmsg->dwData) );			
		}
		break;
	case MP_QUEST_TIME_LIMIT:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			QuestTimeLimit( pmsg->dwData1, pmsg->dwData2 );
		}
		break;
	case MP_QUEST_REGIST_CHECKTIME:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

			RegistCheckTime( pmsg->dwData1, (BYTE)pmsg->dwData2, pmsg->dwData3 );			
		}
		break;
	case MP_QUEST_UNREGIST_CHECKTIME:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			UnRegistCheckTime( pmsg->dwData );
		}
		break;

	case MP_QUESTEVENT_NPCTALK_ACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			QuestEventAck( pmsg->dwData1 );
		}
		break;
	case MP_QUESTEVENT_NPCTALK_NACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			QuestEventNack( pmsg->dwData1, pmsg->dwData2 );
		}
		break;

	case MP_QUEST_EXECUTE_ERROR:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			GAMEIN->GetNpcScriptDialog()->EndDialog();
			GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);;
			WINDOWMGR->MsgBox( MBI_QUESTMSG, MBT_OK, CHATMGR->GetChatMsg(1306) );
		}
		break;
	default:
		break;
	}
}

//
#include "GameIn.h"
#include "QuestTotalDialog.h"

void CQuestManager::AddStringTest()
{
	CQuestString* pQuestString = NULL;
}

void CQuestManager::ViewToggle()
{
	GAMEIN->GetQuestTotalDialog()->QuestListView();
}

void CQuestManager::AddJournalTest()
{
	CQuestString* pQuestString = NULL;
	char buf[16] = { 0, };
	sprintf(buf, "2004.08.06");	
	
	JOURNALINFO item;
	item.Kind = eJournal_Quest;
	item.bSaved = FALSE;
	item.Param = 1;
	strcpy(item.RegDate, buf);
		m_QuestUpdater->JournalItemAdd(&item);
	item.Param = 2;
	item.Param_2 = 0;
	m_QuestUpdater->JournalItemAdd(&item);
	item.Param = 3;
	m_QuestUpdater->JournalItemAdd(&item);

}

void CQuestManager::QuestAbandon()
{
	DWORD QuestIdx = m_QuestUpdater->GetSelectedQuestID();
	m_QuestUpdater->CloseMsgBox();	

	if( IsQuestComplete(QuestIdx) )
		return;

	SEND_QUEST_IDX msg;

	msg.Category = MP_QUEST;
	msg.Protocol = MP_QUEST_DELETE_SYN;
	msg.dwObjectID = HERO->GetID();
	msg.MainQuestIdx = (WORD)QuestIdx;

	NETWORK->Send(&msg, sizeof(msg));

	//test
	m_QuestUpdater->DeleteQuest(QuestIdx);
}

void CQuestManager::QuestAbandon( DWORD dwQuestIdx )
{
	if( IsQuestComplete(dwQuestIdx) )
		return;

	SEND_QUEST_IDX msg;

	msg.Category = MP_QUEST;
	msg.Protocol = MP_QUEST_DELETE_SYN;
	msg.dwObjectID = HERO->GetID();
	msg.MainQuestIdx = (WORD)dwQuestIdx;

	NETWORK->Send(&msg, sizeof(msg));

	//test
	m_QuestUpdater->DeleteQuest( dwQuestIdx );
}

void CQuestManager::QuestAbandonClose()
{
	m_QuestUpdater->CloseMsgBox();
}

void CQuestManager::AddQuestNpc( CSubQuestInfo* pSubQuestInfo )
{
	if( !pSubQuestInfo ) return;

	cPtrList* pNpcList = pSubQuestInfo->GetQuestNpcList();
	if( !pNpcList ) return;

	PTRLISTPOS npos = pNpcList->GetHeadPosition();
	while( npos )
	{
		CQuestNpc* pNpc = (CQuestNpc*)pNpcList->GetNext( npos );
		if( pNpc )
		{
			cPtrList* pList = pNpc->GetNpcDataList();
			if( pList )
			{
				PTRLISTPOS pos = pList->GetHeadPosition();
				while( pos )
				{
					CQuestNpcData* pData = (CQuestNpcData*)pList->GetNext( pos );
					if( pData )
					{
						QUESTNPCINFO* pInfo = m_QuestNpcTable.GetData( pData->GetNpcIdx() );
						if( pInfo )
							MAP->AddQuestNpc( pInfo );
					}
				}
			}
		}
	}
}

BOOL CQuestManager::IsQuestStarted( DWORD dwQuestIdx )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( pQuest )
	{
		if( pQuest->IsSubQuestComplete( 0 ) )
			return FALSE;
		else
			return pQuest->CanItemUse( dwQuestIdx );
	}
	return FALSE;
}

char* CQuestManager::GetQuestTitle( DWORD dwQuestIdx )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( !pQuest )	return NULL;

	//
	int Key = 0;
	COMBINEKEY( dwQuestIdx, 0, Key );
	CQuestString* pQString = m_QuestStringTable.GetData(Key);
	if( pQString == NULL )	return NULL;
	cPtrList* p = pQString->GetTitle();
	QString* string = (QString*)p->GetHead();

	return string->Str;
}

void CQuestManager::QuestTimeLimit( DWORD dwQuestIdx, DWORD dwTime )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( !pQuest )	return;

	//
	stTIME time;
	time.value = dwTime;

	int Key = 0;
	COMBINEKEY( dwQuestIdx, 0, Key );
	CQuestString* pQString = m_QuestStringTable.GetData(Key);
	if( pQString == NULL )	return;
	cPtrList* p = pQString->GetTitle();
	QString* string = (QString*)p->GetHead();

	CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(1153), string->Str,
					 time.GetYear()+2000, time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute() );	
}

void CQuestManager::RegistCheckTime( DWORD dwQuestIdx, BYTE bCheckType, DWORD dwCheckTime )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( !pQuest )	return;

	pQuest->RegistCheckTime( bCheckType, dwCheckTime );

	STATUSICONDLG->AddQuestTimeIcon( HERO, eStatusIcon_QuestTime );
}

void CQuestManager::UnRegistCheckTime( DWORD dwQuestIdx )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );
	if( !pQuest )	return;

	pQuest->UnRegistCheckTime();

	STATUSICONDLG->RemoveQuestTimeIcon( HERO, eStatusIcon_QuestTime );
}

void CQuestManager::QuestEventAck( DWORD dwQuestIdx )
{
	WINDOWMGR->MsgBox( MBI_QUESTMSG, MBT_OK, "출석체크가 되었습니다." );
}

void CQuestManager::QuestEventNack( DWORD dwQuestIdx, DWORD dwError )
{
	switch( dwError )
	{
	case 1:
		{
			WINDOWMGR->MsgBox( MBI_QUESTMSG, MBT_OK, "이미 출석체크가 되어 협행을 진행할 수 없습니다." );
		}
		break;
	case 2:
		{
			WINDOWMGR->MsgBox( MBI_QUESTMSG, MBT_OK, "홈페이지 출석체크가 되어 있지 않아 협행을 진행할 수 없습니다." );
		}
		break;
	}
}
