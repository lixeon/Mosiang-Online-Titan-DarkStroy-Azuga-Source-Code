// UngijosikManager.cpp: implementation of the CUngijosikManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UngijosikManager.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"
#include "GameIn.h"

#include "CharStateDialog.h"
#include "GameEventManager.h"

#include "MHCamera.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CUngijosikManager);
CUngijosikManager::CUngijosikManager()
{

}

CUngijosikManager::~CUngijosikManager()
{

}

void CUngijosikManager::ToggleHero_UngiMode()
{	
	if( !HERO ) return;

	MSGBASE msg;
	msg.Category = MP_UNGIJOSIK;
	if(OBJECTSTATEMGR->GetObjectState(HERO) == eObjectState_Ungijosik)
	{		
		if( HERO->GetEngineObject()->GetCurMotion() == eMotion_Ungijosik_Start )
			return;

		if(OBJECTSTATEMGR->IsEndStateSetted(HERO) == TRUE)	// if(Is Closing Ungijosik)
			return;

		Toggle_UngiMode(HERO, FALSE);
		msg.Protocol = MP_UNGIJOSIK_END;
//KES CHARSTATEDLG 031016
		if(GAMEIN->GetCharStateDialog())
			GAMEIN->GetCharStateDialog()->SetUngiMode( FALSE );
		
		DWORD time = HERO->GetEngineObject()->GetAnimationTime(eMotion_Ungijosik_End);
		CAMERA->SetCharState( eCS_Normal, time );
//		
	}
	else
	{
		// 상태가 None 일때만 할 수 있다.
		if(HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal)
		{
			return;
		}

		//SW070127 타이탄
		//타이탄 탑승시 운기 불가
		if( HERO->InTitan() )
		{
			return;
		}

		Toggle_UngiMode(HERO, TRUE);
		msg.Protocol = MP_UNGIJOSIK_START;
//KES CHARSTATEDLG 031016
		if(GAMEIN->GetCharStateDialog())
			GAMEIN->GetCharStateDialog()->SetUngiMode( TRUE );
		DWORD time = HERO->GetEngineObject()->GetAnimationTime(eMotion_Ungijosik_Start);
		CAMERA->SetCharState( eCS_Sit, time );
//		
		GAMEEVENTMGR->AddEvent(eGameEvent_Ability, 401);
	}
	msg.dwObjectID = HEROID;
	
	NETWORK->Send(&msg, sizeof(msg));
}

void CUngijosikManager::Toggle_UngiMode(CObject* pObject, BOOL bUngi)
{
	if(bUngi == TRUE)
	{
		OBJECTSTATEMGR->StartObjectState(pObject, eObjectState_Ungijosik);
	}
	else 
	{
		DWORD time = pObject->GetEngineObject()->GetAnimationTime(eMotion_Ungijosik_End);
		OBJECTSTATEMGR->EndObjectState(pObject, eObjectState_Ungijosik,time);
	}
}

void CUngijosikManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{

	switch(Protocol)
	{
	case MP_UNGIJOSIK_START:
		{
			CObject* pObject;
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			pObject = OBJECTMGR->GetObject(pmsg->dwObjectID);
			ASSERT(pObject);
			if( pObject == NULL )
			{
				break;	//KES.. NULL이 되는 경우가 있다(버그) 그래도 뻑은 막자.
			}
			Toggle_UngiMode(pObject, TRUE);
		}
		break;
	case MP_UNGIJOSIK_END:
		{
			CObject* pObject;
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			pObject = OBJECTMGR->GetObject(pmsg->dwObjectID);
			ASSERT(pObject);
			if( pObject == NULL )
			{
				break;	//KES.. NULL이 되는 경우가 있다(버그) 그래도 뻑은 막자.
			}
			Toggle_UngiMode(pObject, FALSE);
		}
	}
}