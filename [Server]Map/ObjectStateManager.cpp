// ObjectStateManager.cpp: implementation of the CObjectStateManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ObjectStateManager.h"
#include "Object.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CObjectStateManager::CObjectStateManager()
{

}

CObjectStateManager::~CObjectStateManager()
{

}

void CObjectStateManager::InitObjectState(CObject* pObject)
{
	pObject->SetState(eObjectState_None);
}

BOOL CObjectStateManager::StartObjectState(CObject* pObject,BYTE State,DWORD dwParam)
{
	switch(pObject->GetState())
	{
	case eObjectState_Die:
		{
			// 죽거나 게임을 끝내는건 언제나 가능하다.
			if( State != eObjectState_Exit )
			{
				char buf[256];
				sprintf(buf, "StartObjectState(NowState : %d, ChangeState : %d)", pObject->GetState(), State);
				ASSERTMSG(0, buf);
			}
		}
		return FALSE;
	case eObjectState_Ungijosik:
	case eObjectState_Exchange:
	case eObjectState_StreetStall_Owner:
	case eObjectState_StreetStall_Guest:
	case eObjectState_Deal:
	case eObjectState_Move:
	case eObjectState_TiedUp:
	case eObjectState_Tactic:
		{
			// 죽거나 게임을 끝내는건 언제나 가능하다.
			if( State != eObjectState_Die &&
				State != eObjectState_Exit )
			{
				char buf[256];
				sprintf(buf, "StartObjectState(NowState : %d, ChangeState : %d)", pObject->GetState(), State);
				ASSERTMSG(0, buf);
			}
		}
		break;
	default:
		break;
	}

//	if( pObject->GetState() == eObjectState_Die )
//		return;	
		
//	if(eObjectState_Die == State)
//	{
//		BASEOBJECT_INFO info;
//		pObject->GetBaseObjectInfo(&info);
//		g_Console.LOG(4,"%s(%d) Killed",info.ObjectName,info.dwObjectID);
//	}

	pObject->OnStartObjectState(State,dwParam);
	pObject->SetState(State);

	return TRUE;
}

void CObjectStateManager::EndObjectState(CObject* pObject,BYTE State, DWORD EndStateCount)
{	
	if(pObject->GetState() != State) 
	{
		// 죽음 상태를 서버에서 먼저 바꾸기 때문에 여기 들어올수 있다.
		if(pObject->GetState() != eObjectState_Die)
		{
			char temp[256];
			sprintf(temp,"EndObjectState... But CurState and State is different  Cur:%d,  State:%d",pObject->GetState(),State);
			ASSERTMSG(0,temp);
		}
		return;
	}
	if(EndStateCount == 0)	// 지금 즉시 끝낸다
	{
		pObject->SetState(eObjectState_None);
		pObject->OnEndObjectState(State);
	}
	else
	{
		pObject->m_ObjectState.State_End_Time = gCurTime + EndStateCount;
		pObject->m_ObjectState.bEndState = TRUE;
	}
}

BYTE CObjectStateManager::GetObjectState(CObject* pObject)
{
	return pObject->GetState();
}

void CObjectStateManager::StateProcess(CObject* pObject)
{
	pObject->StateProcess();

	if(pObject->m_ObjectState.bEndState)
	{
		if(pObject->m_ObjectState.State_End_Time < gCurTime)
		{
//			if(pObject->m_BaseObjectInfo.ObjectState == eObjectState_Die)
//			{
//				int a=0;
//			}
			EndObjectState(pObject,pObject->m_BaseObjectInfo.ObjectState);
		}
	}
}
