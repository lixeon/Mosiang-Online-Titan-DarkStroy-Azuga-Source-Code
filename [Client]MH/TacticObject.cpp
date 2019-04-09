// TacticObject.cpp: implementation of the CTacticObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TacticObject.h"
#include "TacticStartInfo.h"
#include "TacticManager.h"

#include "ObjectManager.h"
#include "MoveManager.h"
#include "ObjectStateManager.h"
#include "PartyManager.h"

#include "Gamein.h"
#include "Hero.h"

#include "ChatManager.h"
#include "SkillManager_Client.h"

#define TACTICSTART_MARGIN_OF_ERROR 75

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTacticObject::CTacticObject()
{

}

CTacticObject::~CTacticObject()
{

}

void CTacticObject::InitTacticObject(TACTIC_TOTALINFO* pTotalInfo)
{
	memcpy(&m_TotalInfo,pTotalInfo,sizeof(TACTIC_TOTALINFO));
	m_TotalInfo.StartTime = gCurTime - m_TotalInfo.StartTime;		// ¼­¹ö¿¡¼­ ElapsedTimeÀ» º¸³»ÁÖ±â ¶§¹®¿¡ ÄÁ¹öÆÃ ÇÑ´Ù.
	m_pTacticInfo = TACTICMGR->GetTacticStartInfo(m_TotalInfo.TacticId);

	m_pTacticInfo->GetTacticMemberPosition(m_TacticMemberPosition,&m_TotalInfo);	
	m_bExcuted = FALSE;
	m_bFailed = FALSE;
}

void CTacticObject::TacticStartEffect()
{
	int n;
	CObject* pObject = OBJECTMGR->GetObject(m_TotalInfo.JoinedPlayerIDs[0]);	// ½ÃÀüÀÚ¸¦ Ã£´Â´Ù.
	ASSERT(pObject);
	if(pObject==NULL)
		MOVEMGR->SetAngle(pObject,DIRTODEG(m_TotalInfo.Direction),0);
	
	//////////////////////////////////////////////////////////////////////////
	// Áø¹ý ÀÚ¸® Ç¥½Ã ÀÌÆåÆ®¸¦ »Ñ¸°´Ù.
	WORD EffNum;
	/*
	if(PARTYMGR->GetTacticObjectID() == GetID())
		EffNum = TSE_POS_START_SELF;
	else
		EffNum = TSE_POS_START_OTHER;
	*/
	EffNum = m_pTacticInfo->GetTacticStartEffectNum();

	DWORD dwFlag = 0;
	if(pObject)
	{
		if(PARTYMGR->IsPartyMember(pObject->GetID()))
			dwFlag |= EFFECT_FLAG_HEROATTACK;
	}

	m_hTacticPositionEffect = EFFECTMGR->StartEffectProcess(EffNum,this,NULL,0,0,dwFlag);

	//////////////////////////////////////////////////////////////////////////
	// Áø¹ý »óÅÂ·Î ¸¸µç´Ù.
	for(n=0;n<m_pTacticInfo->GetTacticPlayerNum();++n)
	{
		if(m_TotalInfo.JoinedPlayerIDs[n] == 0)
			continue;

		pObject = OBJECTMGR->GetObject(m_TotalInfo.JoinedPlayerIDs[n]);
		if(pObject == NULL)
			continue;

		StartJoinTacticEffect(pObject,n);
	}
}

void CTacticObject::StartJoinTacticEffect(CObject* pObject,int Position)
{
	//MOVEMGR->SetPosition(pObject,&m_TacticMemberPosition[n]);
	m_TotalInfo.JoinedPlayerIDs[Position] = pObject->GetID();
	OBJECTSTATEMGR->StartObjectState(pObject,eObjectState_Tactic);
	
	if(GetID() == PARTYMGR->GetTacticObjectID())
	{
		//YH2DO ÀÓ½Ã·Î ³Ö¾î³ù´Ù°¡ ÀÌ»óÇØ¼­ »©¹ö¸²
		//EFFECTMGR->StartEffectProcess(1,pObject,NULL,0,0);
	}
}
void CTacticObject::EndJoinTacticEffect(CObject* pObject,int Position)
{
	OBJECTSTATEMGR->EndObjectState(pObject,eObjectState_Tactic);
}

int CTacticObject::CheckJoin(CHero* pHero)
{
	if(m_pTacticInfo->IsTacticableState(pHero) == FALSE)
		return -1;

	// ÁÂÇ¥ °Ë»ç
	int pos = -1;
	float dist;
	VECTOR3 heropos = pHero->GetCurPosition();
	for(int n=0 ; n<m_pTacticInfo->GetTacticPlayerNum() ;++n)
	{
		dist = CalcDistanceXZ(&heropos,&m_TacticMemberPosition[n]);
		if(dist <= TACTICSTART_MARGIN_OF_ERROR)
		{
			pos = n;
			break;
		}
	}
	
	return pos;
}

void CTacticObject::ExcuteTactic()
{
	if(m_bExcuted == TRUE)
		return;

	CObject* pObject;
	//////////////////////////////////////////////////////////////////////////
	// Áø¹ý »óÅÂ¸¦ ÇØÁ¦ÇÑ´Ù.
	for(int n=0;n<m_pTacticInfo->GetTacticPlayerNum();++n)
	{
		if(m_TotalInfo.JoinedPlayerIDs[n] == 0)
			continue;

		pObject = OBJECTMGR->GetObject(m_TotalInfo.JoinedPlayerIDs[n]);
		if(pObject == NULL)
			continue;

		EndJoinTacticEffect(pObject,n);

		//////////////////////////////////////////////////////////////////
		if(pObject == HERO && n == 0)
		{
			SKILLMGR->ExcuteTacticSkillSYN(HERO,&m_TotalInfo,m_pTacticInfo->GetTacticPlayerNum());
		}
		//////////////////////////////////////////////////////////////////
	}
	if(m_hTacticPositionEffect)
		EFFECTMGR->ForcedEndEffect(m_hTacticPositionEffect);

	m_bExcuted = TRUE;
}

void CTacticObject::SetMotionInState(BYTE State)
{
	m_ObjectState.State_Start_Motion = -1;
	m_ObjectState.State_Ing_Motion = -1;
	m_ObjectState.State_End_Motion = -1;
}

void CTacticObject::OnFailedTactic(DWORD TacticFailedReason,BOOL bMsg)
{
	// ÆÄÆ¼¿¡µµ º¸³»ÁÖ°í ÁÖº¯¾ÖµéÇÑÅ×µµ º¸³»±â ¶§¹®¿¡ µÎ ¹ø µé¾î¿Ã ¼ö ÀÖ´Ù.
	if(m_bFailed == TRUE)
		return;

	m_bFailed = TRUE;
	CObject* pObject;
	//////////////////////////////////////////////////////////////////////////
	// Áø¹ý »óÅÂ¸¦ ÇØÁ¦ÇÑ´Ù.
	for(int n=0;n<m_pTacticInfo->GetTacticPlayerNum();++n)
	{
		if(m_TotalInfo.JoinedPlayerIDs[n] == 0)
			continue;

		pObject = OBJECTMGR->GetObject(m_TotalInfo.JoinedPlayerIDs[n]);
		if(pObject == NULL)
			continue;

		EndJoinTacticEffect(pObject,n);
	}
	if(m_hTacticPositionEffect)
		EFFECTMGR->ForcedEndEffect(m_hTacticPositionEffect);
	
	if( bMsg )
	{
		ySWITCH(TacticFailedReason)
			yCASE(eTFR_Naeryuk)
				CHATMGR->AddMsg( CTC_TOWHOLE, CHATMGR->GetChatMsg(694));
			yCASE(eTFR_TimeOver)
				CHATMGR->AddMsg( CTC_TOWHOLE, CHATMGR->GetChatMsg(695));
			yCASE(eTFR_MemberDie)
				CHATMGR->AddMsg( CTC_TOWHOLE, CHATMGR->GetChatMsg(696));
		yENDSWITCH
	}
}

void CTacticObject::SetPosition(VECTOR3* pPos)
{
}
void CTacticObject::GetPosition(VECTOR3* pPos)
{
	*pPos = m_TotalInfo.Pos;
	GetFieldHeight(pPos);
}
void CTacticObject::SetAngle(float AngleRad)
{
}
float CTacticObject::GetAngle()
{
	return DIRTORAD(m_TotalInfo.Direction);
}

void CTacticObject::ChangeMotion(int motion,BOOL bRoop)
{
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		DWORD id = m_TotalInfo.JoinedPlayerIDs[n];
		if(id == 0)
			break;
		CObject* pObj = OBJECTMGR->GetObject(id);
		if(pObj)
			pObj->ChangeMotion(motion,bRoop);
	}
}

void CTacticObject::ChangeBaseMotion(int motion)
{
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		DWORD id = m_TotalInfo.JoinedPlayerIDs[n];
		if(id == 0)
			break;
		CObject* pObj = OBJECTMGR->GetObject(id);
		if(pObj)
			pObj->ChangeBaseMotion(motion);
	}
}