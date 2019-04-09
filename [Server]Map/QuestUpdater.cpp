// QuestUpdater.cpp: implementation of the CQuestUpdater class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Player.h"
#include "QuestUpdater.h"
#include "MapDBMsgParser.h"
#include "Quest.h"
#include "QuestGroup.h"
#include "..\[CC]Quest\QuestInfo.h"
#include "..\[CC]Quest\SubQuestInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestUpdater::CQuestUpdater()
{

}

CQuestUpdater::~CQuestUpdater()
{

}

void CQuestUpdater::StartQuest(CPlayer* player, CQuest* pQuest)
{	
	if( pQuest == NULL )	return;
	QuestMainQuestInsert( player->GetID(), pQuest->GetQuestIdx(), pQuest->GetSubQuestFlag(),
						  pQuest->GetQuestTime() );
}

void CQuestUpdater::EndQuest(CPlayer* player, CQuest* pQuest)
{	
	if( pQuest == NULL )	return;
	// 확실하게 끝났다고 1로 셋팅. - RaMa 04.10.14
	// pQuest->IsQuestComplete() 이거 좀 이상한거 같음..	
	
	Quest_EndQuest_New( player->GetID(), pQuest->GetQuestIdx(), pQuest->GetSubQuestFlag(), 1, pQuest->GetQuestTime());

//	QuestSubQuestDeleteAll( player->GetID(), pQuest->GetQuestIdx() );
//	QuestItemDeleteAll( player->GetID(), pQuest->GetQuestIdx() );

//	QuestMainQuestUpdateToDB( player->GetID(), pQuest->GetQuestIdx(), pQuest->GetSubQuestFlag(),		
//		1, pQuest->GetQuestTime() );
}

void CQuestUpdater::DeleteQuest(CPlayer* player, CQuest* pQuest)
{
	if( pQuest == NULL )	return;

	Quest_DeleteQuest_New( player->GetID(), pQuest->GetQuestIdx() );
//	QuestMainQuestDelete( player->GetID(), pQuest->GetQuestIdx() );
//	QuestSubQuestDeleteAll( player->GetID(), pQuest->GetQuestIdx() );
//	QuestItemDeleteAll( player->GetID(), pQuest->GetQuestIdx() );
}

void CQuestUpdater::UpdateQuest( CPlayer* player, DWORD dwFlag, DWORD dwParam, DWORD dwTime )
{
	QuestMainQuestUpdateToDB( player->GetID(), 0, dwFlag, dwParam, dwTime );
}

void CQuestUpdater::StartSubQuest(CPlayer* player, CQuest* pQuest, DWORD sIndex)
{
	if( pQuest == NULL )	return;
	QuestSubQuestInsert( player->GetID(), pQuest->GetQuestIdx(), sIndex,
						 pQuest->GetSubQuestData(sIndex), pQuest->GetSubQuestTime(sIndex) );
}

void CQuestUpdater::EndSubQuest(CPlayer* player, CQuest* pQuest, DWORD sIndex)
{	
	if( pQuest == NULL )	return;
	
	Quest_EndSubQuest_New( player->GetID(), pQuest->GetQuestIdx(), sIndex, pQuest->GetSubQuestFlag(), pQuest->GetQuestTime() );
//	QuestSubQuestDelete(player->GetID(), pQuest->GetQuestIdx(), sIndex);
//	QuestMainQuestUpdateToDB( player->GetID(), pQuest->GetQuestIdx(), pQuest->GetSubQuestFlag(),
//		pQuest->IsQuestComplete(), pQuest->GetQuestTime() );
}

void CQuestUpdater::UpdateSubQuest(CPlayer* player, CQuest* pQuest, DWORD sIndex)
{
	if( pQuest == NULL )	return;
	QuestSubQuestUpdateToDB( player->GetID(), pQuest->GetQuestIdx(), sIndex, 
							 pQuest->GetSubQuestData(sIndex), pQuest->GetSubQuestTime(sIndex) );
}

void CQuestUpdater::GiveQuestItem( CPlayer* pPlayer, DWORD dwItemIdx, DWORD dwItemNum )
{
	QuestItemDelete( pPlayer->GetID(), dwItemIdx );
}

void CQuestUpdater::TakeQuestItem( CPlayer* pPlayer, DWORD dwQuestIdx, DWORD dwItemIdx, DWORD dwItemNum )
{
	QuestItemInsert( pPlayer->GetID(), dwQuestIdx, dwItemIdx, dwItemNum );	
}

void CQuestUpdater::UpdateQuestItem( CPlayer* pPlayer, DWORD dwQuestIdx, DWORD dwItemIdx, DWORD dwItemNum )
{
	QuestItemUpdate( pPlayer->GetID(), dwQuestIdx, dwItemIdx, dwItemNum );
}

void CQuestUpdater::UpdateQuestCheckTime( CPlayer* player, DWORD dwQuestIdx, DWORD dwCheckType, DWORD dwCheckTime )
{
	QuestMainQuestUpdateCheckTime( player->GetID(), dwQuestIdx, dwCheckType, dwCheckTime );
}