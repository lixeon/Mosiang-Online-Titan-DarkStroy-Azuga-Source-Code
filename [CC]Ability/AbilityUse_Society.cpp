// AbilityUse_Society.cpp: implementation of the CAbilityUse_Society class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AbilityUse_Society.h"

#ifdef _MHCLIENT_
#include "ObjectManager.h"
#include "SocietyActManager.h"
#include "ChatManager.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAbilityUse_Society::CAbilityUse_Society()
{

}

CAbilityUse_Society::~CAbilityUse_Society()
{

}

void CAbilityUse_Society::Act(CObject* pObject, CAbilityInfo* pAbilityInfo)
{
#ifdef _MHCLIENT_
	
	if( pObject == HERO )
	{
		if( HERO->InTitan() )
			return;

		if( HERO->CanSocietyAction( gCurTime ) == FALSE )
		{
			return;
		}
	}

	// 06. 01 새해 맞이 격식 인사 - 이영준
	/*
	if(pAbilityInfo->GetIdx() == 307)
	{
		EFFECTMGR->StartPlayerEffectProcess( (CPlayer*)pObject , FindEffectNum("m_ba_081.beff") );
		CHATMGR->AddMsg( CTC_GENERALCHAT, "[%s]: %s", (CPlayer*)pObject->GetObjectName(), CHATMGR->GetChatMsg(589) );
		OBJECTMGR->ShowChatBalloon( (CPlayer*)pObject, CHATMGR->GetChatMsg(589) );
	}
	*/

	int nStartMotion	= pAbilityInfo->GetInfo()->Ability_effect_Param1;
	int nIngMotion		= pAbilityInfo->GetInfo()->Ability_effect_Param2;
	int nEndMotion		= pAbilityInfo->GetInfo()->Ability_effect_Param3;

	pObject->StartSocietyAct( nStartMotion, nIngMotion, nEndMotion, TRUE );

	if( pObject == HERO )
		SOCIETYACTMGR->SendSocietyActMsg( pAbilityInfo->GetIdx() );

#endif
}