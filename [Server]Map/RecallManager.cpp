// RecallManager.cpp: implementation of the CRecallManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RecallManager.h"
#include "..\[CC]Header\GameResourceStruct.h"
#include "Monster.h"
#include "RegenManager.h"
#include "AISystem.h"
#include "RegenPrototype.h"
#include "CharMove.h"
#include "ChannelSystem.h"
#include "UserTable.h"
#include "..\[CC]Header\GameResourceManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRecallManager::CRecallManager()
{
}

CRecallManager::~CRecallManager()
{

}

//Â¹Â«ÃÃ¶Â´?Â¸Â®Â°ÃšÃÃ¶Â¸Â¸.. ?ÃšÃÃ– Â»Ã§Â¿Ã«ÂµÃ‡Â´Ã‚ Â°Ã?ÃŒ Â¾Ã†Â´ÃÂ¹Ã‡Â·ÃŽ..
void CRecallManager::DeleteEventRecallMonster()
{
	g_pUserTable->SetPositionUserHead();
	CObject* pObject;
	while( pObject = g_pUserTable->GetUserData() )
	{
		if( pObject->GetObjectKind() & eObjectKind_Monster )
		{
//			if( pObject->GetObjectKind() == eObjectKind_Monster )
			{
				if( ((CMonster*)pObject)->GetMonsterGroupNum() == (WORD)EVENTRECALL_GROUP_ID )
				{
					g_pServerSystem->RemoveMonster( pObject->GetID() );
				}
			}
/*			else if( pObject->GetObjectKind() == eObjectKind_BossMonster )
			{
				if( ((CMonster*)pObject)->GetMonsterGroupNum() == (WORD)EVENTRECALL_GROUP_ID )
				{
					g_pServerSystem->RemoveBossMonster( pObject->GetID() );
				}
			} */
		}
	}
}

//es
void CRecallManager::EventRecall(CObject * pRecaller, WORD MonsterKind, BYTE cbMonsterCount, BYTE cbChannel, VECTOR3* pPos, WORD wRadius, WORD DropItemID, DWORD dwDropRatio )
{
	// ÂºÃ±Â¾Ã® ?Ã–Â´Ã‚ AddPrototype
	// IDÂ»Ã½Â¼Âº
	DWORD dwID, dwSubID;
	DWORD dwChannelID;
	
	VECTOR3 vRecallPos;
	
	for( int i = 0 ; i < cbMonsterCount ; ++i )
	{
		if( cbChannel == 0 )
		{
			for( DWORD j = 0 ; j < CHANNELSYSTEM->GetChannelCount() ; ++j )
			{
				dwChannelID = CHANNELSYSTEM->GetChannelID( j );
				if( dwChannelID )
				{
					
					dwID = g_pAISystem->GeneraterMonsterID();
					dwSubID = g_pAISystem->GeneraterMonsterID();
					
					if( wRadius != 0 )
					{
						vRecallPos.x = pPos->x + (rand()%(wRadius*2)) - wRadius;
						vRecallPos.z = pPos->z + (rand()%(wRadius*2)) - wRadius;
					}
					else
					{
						vRecallPos.x = pPos->x;
						vRecallPos.z = pPos->z;
					}
					
					if( vRecallPos.x < 0 )				vRecallPos.x = 0;
					else if( vRecallPos.x > 51199 )		vRecallPos.x = 51199;

					if( vRecallPos.z < 0 )				vRecallPos.z = 0;
					else if( vRecallPos.z > 51199 )		vRecallPos.z = 51199;
		
					/*
					if( MonsterKind < 1000 ) //ÂºÂ¸Â½ÂºÂ¸Ã·?ÃŒ Â¾Ã†Â´ÃÂ¸Ã©
						REGENMGR->RegenObject( dwID, dwSubID, dwChannelID, eObjectKind_Monster, MonsterKind, &vRecallPos, EVENTRECALL_GROUP_ID, DropItemID, dwDropRatio, FALSE, TRUE );
					else
						REGENMGR->RegenObject( dwID, dwSubID, dwChannelID, eObjectKind_BossMonster, MonsterKind, &vRecallPos, EVENTRECALL_GROUP_ID, DropItemID, dwDropRatio, FALSE, TRUE );
					*/

					/*
					switch(MonsterKind)
					{
						// º¸½º¸÷ Á¾·ù
					case 	eBOSSKIND_EMPERIORTOMB :
					case	eBOSSKIND_EMPERIORTOMB_EVENT :


						wObjectKind_Monster = eObjectKind_BossMonster;	break;
					}
					
					REGENMGR->RegenObject( dwID, dwSubID, dwChannelID, wObjectKind_Monster, MonsterKind, &vRecallPos, EVENTRECALL_GROUP_ID, DropItemID, dwDropRatio, FALSE, TRUE );
					*/

					// ¸ó½ºÅÍ ¸®½ºÆ®¿¡ ObjectKind Ãß°¡ 05.12.22 ÀÌ¿µÁØ
					BASE_MONSTER_LIST* pList = GAMERESRCMNGR->GetMonsterListInfo(MonsterKind);
					
					if(pList)
					{
						REGENMGR->RegenObject( dwID, dwSubID, dwChannelID, pList->ObjectKind, pList->MonsterKind, &vRecallPos, EVENTRECALL_GROUP_ID, DropItemID, dwDropRatio, FALSE, TRUE );					
					}
				}
			}
		}
		else if( cbChannel > 0 )
		{
			dwChannelID = CHANNELSYSTEM->GetChannelID( cbChannel - 1 );
			
			if( dwChannelID )
			{
				
				dwID = g_pAISystem->GeneraterMonsterID();
				dwSubID = g_pAISystem->GeneraterMonsterID();
				
				if( wRadius != 0 )
				{
					vRecallPos.x = pPos->x + (rand()%(wRadius*2)) - wRadius;
					vRecallPos.z = pPos->z + (rand()%(wRadius*2)) - wRadius;
				}
				else
				{
					vRecallPos.x = pPos->x;
					vRecallPos.z = pPos->z;
				}
				
				if( vRecallPos.x < 0 )				vRecallPos.x = 0;
				else if( vRecallPos.x > 51199 )		vRecallPos.x = 51199;

				if( vRecallPos.z < 0 )				vRecallPos.z = 0;
				else if( vRecallPos.z > 51199 )		vRecallPos.z = 51199;
					
				/*
				if( MonsterKind < 1000 ) //ÂºÂ¸Â½ÂºÂ¸Ã·?ÃŒ Â¾Ã†Â´ÃÂ¸Ã©
					REGENMGR->RegenObject( dwID, dwSubID, dwChannelID, eObjectKind_Monster, MonsterKind, &vRecallPos, EVENTRECALL_GROUP_ID, DropItemID, dwDropRatio, FALSE, TRUE );
				else
					REGENMGR->RegenObject( dwID, dwSubID, dwChannelID, eObjectKind_BossMonster, MonsterKind, &vRecallPos, EVENTRECALL_GROUP_ID, DropItemID, dwDropRatio, FALSE, TRUE );
				*/
				/*
				WORD wObjectKind_Monster = eObjectKind_Monster;
				
				switch(MonsterKind)
				{
				// º¸½º¸÷ Á¾·ù
				case 	eBOSSKIND_EMPERIORTOMB :
				case	eBOSSKIND_EMPERIORTOMB_EVENT :
					
					
					wObjectKind_Monster = eObjectKind_BossMonster;	break;
				}
				
				REGENMGR->RegenObject( dwID, dwSubID, dwChannelID, wObjectKind_Monster, MonsterKind, &vRecallPos, EVENTRECALL_GROUP_ID, DropItemID, dwDropRatio, FALSE, TRUE );
				*/
				// ¸ó½ºÅÍ ¸®½ºÆ®¿¡ ObjectKind Ãß°¡ 05.12.22 ÀÌ¿µÁØ
				BASE_MONSTER_LIST* pList = GAMERESRCMNGR->GetMonsterListInfo(MonsterKind);

				if(pList)
				{
					REGENMGR->RegenObject( dwID, dwSubID, dwChannelID, pList->ObjectKind, pList->MonsterKind, &vRecallPos, EVENTRECALL_GROUP_ID, DropItemID, dwDropRatio, FALSE, TRUE );					
				}
			}
		}
	}
}

int CRecallManager::RecallRange( CObject* pRecaller, DWORD dwGridID, VECTOR3 * vPos, WORD wRecallMonsterKind, DWORD wRecallNum, DWORD dwRange, int GroupID,BOOL bRandomPos /* = TRUE */, BOOL bEventRegen /* = FALSE */, WORD ObjectKind /* = eObjectKind_Monster */ )
{
	VECTOR3 vRecallPos;

	float dx = 0, dz = 0;
	int ran = 0;

	int regencount = 0;
	WORD wObjectKind;

	for( WORD i = 0 ; i < wRecallNum ; ++i)
	{
		if(dwRange)
		{		
			ran = rand();
			dx = float(ran%dwRange) * (ran%2?1:-1);
			ran = rand();
			dz = float(ran%dwRange) * (ran%2?1:-1);
			vRecallPos.x = vPos->x + dx;
			vRecallPos.z = vPos->z + dz;
		}
		else
		{
			vRecallPos.x = vPos->x;
			vRecallPos.z = vPos->z;
		}

		DWORD dwID = g_pAISystem->GeneraterMonsterID();
		DWORD dwSubID;
		if(ObjectKind == eObjectKind_Npc)
			dwSubID = 0;
		else 
			dwSubID = g_pAISystem->GeneraterMonsterID();
		wObjectKind = ObjectKind;
		CMonster* pMonster = REGENMGR->RegenObject(dwID, dwSubID, dwGridID, wObjectKind, 
						wRecallMonsterKind, &vRecallPos, RECALL_GROUP_ID,
						0,100,bRandomPos,bEventRegen);
		if(pMonster)
		{
			++regencount;
			pMonster->SetSuryunGroup(GroupID);
		}
	}

	return regencount;
}

void CRecallManager::RecallObject(CMonster * pRecaller, RECALLINFO * pRecallInfo)
{
	//RECALLINFO * pRecallInfo = ((SMONSTER_LIST*)pRecaller->GetSMonsterList())->RecallInfoClass.GetRecallInfo(0);
	switch(pRecallInfo->RecallType)
	{
	case RECALL_GROUP:
		{
			REGENMGR->RegenGroup(pRecallInfo->RecallID, pRecaller->GetGridID());
		}
		break;
	case RECALL_RECALLERRANGE:
		{
			VECTOR3 vRecallPos;
			pRecaller->GetPosition(&vRecallPos);
			RecallRange(pRecaller, pRecaller->GetGridID(), &vRecallPos, (WORD)pRecallInfo->RecallID, 
				pRecallInfo->RecallMonsterNum, pRecallInfo->RecallRange, 0 );
		}
		break;
	case RECALL_RECALLERPOSITION:
		{
			VECTOR3 vRecallPos;
			pRecaller->GetPosition(&vRecallPos);
			RecallRange(pRecaller, pRecaller->GetGridID(), &vRecallPos, (WORD)pRecallInfo->RecallID, 1, 0, 0 );
		}
		break;
	case RECALL_TARGETRANGE:
		{
			VECTOR3 vRecallPos;
			if(!pRecaller->GetTObject()) return;
			pRecaller->GetTObject()->GetPosition(&vRecallPos);
			RecallRange(pRecaller, pRecaller->GetGridID(), &vRecallPos, (WORD)pRecallInfo->RecallID, 
				pRecallInfo->RecallMonsterNum, pRecallInfo->RecallRange, 0 );
		}
		break;
	}
}

void CRecallManager::QuestRecall( WORD wMonsterKind, BYTE bMonsterCount, DWORD dwChannel, VECTOR3* pMonsterPos, WORD wRadius )
{
	DWORD dwID, dwSubID;	
	VECTOR3 vRecallPos;
	
	for( BYTE i = 0; i < bMonsterCount; ++i )
	{
		if( dwChannel )
		{
			dwID = g_pAISystem->GeneraterMonsterID();
			dwSubID = g_pAISystem->GeneraterMonsterID();

			if( wRadius )
			{
				vRecallPos.x = pMonsterPos->x + (rand()%(wRadius*2)) - wRadius;
				vRecallPos.z = pMonsterPos->z + (rand()%(wRadius*2)) - wRadius;
			}
			else
			{
				vRecallPos.x = pMonsterPos->x;
				vRecallPos.z = pMonsterPos->z;
			}

			if( vRecallPos.x < 0 )				vRecallPos.x = 0;
			else if( vRecallPos.x > 51199 )		vRecallPos.x = 51199;
			if( vRecallPos.z < 0 )				vRecallPos.z = 0;
			else if( vRecallPos.z > 51199 )		vRecallPos.z = 51199;
	
			REGENMGR->RegenObject( dwID, dwSubID, dwChannel, eObjectKind_Monster, wMonsterKind, &vRecallPos, QUESTRECALL_GROUP_ID, 0, 100, FALSE, TRUE );
		}
	}
}

void CRecallManager::DeleteQuestRecallMonster( DWORD dwChannel )
{
	CObject* pObject = NULL;
	CMonster* pMonster = NULL;

	g_pUserTable->SetPositionUserHead();	
	while( pObject = g_pUserTable->GetUserData() )
	{
		if( pObject->GetObjectKind() & eObjectKind_Monster )
		{
			pMonster = (CMonster*)pObject;
			if( pMonster->GetMonsterGroupNum() == (WORD)QUESTRECALL_GROUP_ID &&
				pMonster->GetBattleID() == dwChannel )
				g_pServerSystem->RemoveMonster( pObject->GetID() );
		}
	}
}