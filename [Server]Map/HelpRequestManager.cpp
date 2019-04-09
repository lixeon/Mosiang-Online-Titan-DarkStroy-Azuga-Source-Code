// HelpRequestManager.cpp: implementation of the CHelpRequestManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HelpRequestManager.h"
#include "Monster.h"
#include "Player.h"
#include "AISystem.h"
#include "RegenPrototype.h"
#include "AIManager.h"
#include "cMonsterSpeechManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHelpRequestManager::CHelpRequestManager()
{

}

CHelpRequestManager::~CHelpRequestManager()
{

}

// 데미지를 받을 때에 호출
void CHelpRequestManager::MonsterRequestProcess(CMonster * pMonster)
{
	CRegenObject * pORInfo = GROUPMGR->GetRegenObject(pMonster->GetMonsterGroupNum(), pMonster->GetGridID(), pMonster->GetID());
	if(!pORInfo) return;/*소환된 몬스터*/
	switch(pORInfo->GetCurHelpType())
	{
	case HELPREQUEST_NONE: { } break;
	case HELPREQUEST_ONETIMEIFHP50: 
		{
			if( pMonster->GetLife() < pMonster->GetMaxLife()/2 )
			{
				// 도움 요청
				//g_pAISystem->SendMsg(eMK_HelpShout, pMonster->GetID(), pMonster->GetID(),0,0);
				g_pAISystem->SendMsg(eMK_HelpRequest, pMonster->GetID(), 0, 0, 0);
				// HelpType 초기화
				pORInfo->SetCurHelpType(HELPREQUEST_NONE);
			}
		}
		break;
	case HELPREQUEST_ALWAYSIFHP30: 
		{
			if( pMonster->GetLife() < pMonster->GetMaxLife()*0.3 )
			{
				// 도움 요청
				//g_pAISystem->SendMsg(eMK_HelpShout, pMonster->GetID(), pMonster->GetID(),0,0);
				g_pAISystem->SendMsg(eMK_HelpRequest, pMonster->GetID(), 0, 0, 0);
			}
		}
		break;
	case HELPREQUEST_DIE: 
		{ 
			// taiyo ok?
			if(pMonster->GetLife() == 0)
			{
				// 도움 요청
				//g_pAISystem->SendMsg(eMK_HelpShout, pMonster->GetID(), pMonster->GetID(),0,0);
				g_pAISystem->SendMsg(eMK_HelpRequest, pMonster->GetID(), 0, 0, 0);
				
			}
		} 
		break;
	case HELPREQUEST_ALWAYS: 
		{
			// 항상 도움 요청
			//g_pAISystem->SendMsg(eMK_HelpShout, pMonster->GetID(), pMonster->GetID(),0,0);
			g_pAISystem->SendMsg(eMK_HelpRequest, pMonster->GetID(), 0, 0, 0);
		} 
		break;
	}
}

//SW050901
void CHelpRequestManager::SetHelperMonster(CMonster* pAsker, CMonster * pHelper, CPlayer * pTargeter)
{
	if( pAsker == NULL && pHelper == NULL )
		return;

	BYTE objectKind = pHelper->GetObjectKind();
	if( (objectKind != eObjectKind_Monster) || (objectKind != eObjectKind_TitanMonster) )
		return;

	if( pHelper->GetGridID() != ((CObject*)pTargeter)->GetGridID() )
		return;

	if ( pHelper->m_stateParam.stateOld == eMA_PERSUIT ) return;

	if( pHelper->m_stateParam.stateCur == eMA_REST || pHelper->m_stateParam.stateCur == eMA_RUNAWAY )
		return;

	if( CAIManager::IsTargetChange(pTargeter, pHelper) )
	{
//		pHelper->SetLastAttackPlayer( pTargeter );
//		pHelper->SetTObject( pTargeter );
//		GSTATEMACHINE.SetState(pHelper, eMA_ATTACK);

		//SW060703 펫버프 추가
		//2008. 6. 9. CBH - 보스 졸개가 벽에 붙는 현상 떄문에 보스 졸개를 일반몹으로
		//수정 후 루루팻에 영향 받는 버그 수정
		if( FALSE == g_pServerSystem->GetMap()->IsMapKind(eBossMap) )
		{
			if( (objectKind == eObjectKind_Monster) || (objectKind == eObjectKind_TitanMonster) )	//일반몬스터
			{
				BOOL bNoForeAtk = FALSE;
				pTargeter->GetPetManager()->GetPetBuffResultRt( ePB_NoForeAtkMonster, & bNoForeAtk );
				if( bNoForeAtk )
				{
					return;
				}
			}
		}

		//순서 변경
		pHelper->SetLastAttackPlayer( pTargeter );
		GSTATEMACHINE.SetState(pHelper, eMA_ATTACK);
		pHelper->SetTObject( pTargeter );

		MonSpeechInfo* pTemp = MON_SPEECHMGR->GetCurStateSpeechIndex( pAsker->GetMonsterKind(), eMon_Speech_Help );
		if( pTemp )
			pAsker->AddSpeech( pTemp->SpeechType, pTemp->SpeechIndex );

	}
}

