// MouseEventThrower.cpp: implementation of the CMouseEventThrower class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MouseEventThrower.h"
#include "MouseEventReceiver.h"
#include "../Engine/GraphicEngine.h"
//#include "Object.h"
//#include "interface\cWindowManager.h"

//#include "ScreenText.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMouseEventThrower::CMouseEventThrower()
{
	m_pCurrentReceiver = NULL;
	m_pDefaultReceiver = NULL;
}

CMouseEventThrower::~CMouseEventThrower()
{

}

void CMouseEventThrower::SetDefaultReceiver(CMouseEventReceiver* pDefaultReceiver)
{
	m_pDefaultReceiver = pDefaultReceiver;
}
void CMouseEventThrower::SetCurrentReceiver(CMouseEventReceiver* pCurrentReceiver)
{
	m_pCurrentReceiver = pCurrentReceiver;
}
void CMouseEventThrower::SetReceiverToDefault()
{
	ASSERT(m_pDefaultReceiver);
	m_pCurrentReceiver = m_pDefaultReceiver;
}

void CMouseEventThrower::Process(CMouse& Mouse)
{
//	static DWORD LastTick = 0;
//	DWORD CurTick = gCurTime;
//	if(CurTick - LastTick < 300)
//		return;

	DWORD MouseEvent = MOUSEEVENT_NONE;
	if(Mouse.LButtonClick() || Mouse.LButtonDoubleClick())		MouseEvent = MOUSEEVENT_LCLICK;
	else if(Mouse.RButtonClick()|| Mouse.RButtonDoubleClick())	MouseEvent = MOUSEEVENT_RCLICK;

	
	//////////////////////////////////////////////////////////////////////////
	// 마우스가 캐릭터 위에 있는가?
//	CObjectBase* pObjectBase = GetSelectedObject(Mouse.GetMouseX(),Mouse.GetMouseY());
	/*
	if(pObjectBase)
	{
		if( pObjectBase->GetEngineObjectType() == eEngineObjectType_Character ||
			pObjectBase->GetEngineObjectType() == eEngineObjectType_Monster ||
			pObjectBase->GetEngineObjectType() == eEngineObjectType_Npc )
		{
			CObject* pObject = (CObject*)pObjectBase;
			// 이름 출력
			CScreenText * scText = new CScreenText;
			scText->SetText(&pObject->GetCurPosition(), pObject->GetObjectName());
			WINDOWMGR->AddText(scText);
		}
	}
	*/
	

	if(MouseEvent != MOUSEEVENT_NONE)
	{
		//CObjectBase* pObjectBase = GetSelectedObject(Mouse.GetMouseX(),Mouse.GetMouseY());

/*		if(pObjectBase)
		{
			if(pObjectBase->GetEngineObjectType() == eEngineObjectType_Character)
			{
				m_pCurrentReceiver->OnClickPlayer(MouseEvent,(CPlayer*)pObjectBase);					
			}
			else if(pObjectBase->GetEngineObjectType() == eEngineObjectType_Monster)
			{
				m_pCurrentReceiver->OnClickMonster(MouseEvent,(CMonster*)pObjectBase);
			}
			else if(pObjectBase->GetEngineObjectType() == eEngineObjectType_Npc)
			{
				m_pCurrentReceiver->OnClickNpc(MouseEvent,(CNpc*)pObjectBase);
			}
			else if(pObjectBase->GetEngineObjectType() == eEngineObjectType_Effect)
			{
				ASSERT(0);
			}
			else
			{
				ASSERT(0);
			}
		}
		else
		{	*/		
			VECTOR3* TargetPos = GetPickedPosition(Mouse.GetMouseX(),Mouse.GetMouseY());
			if(!TargetPos)
				return;

			m_pCurrentReceiver->OnClickGround(MouseEvent,TargetPos);
//		}
	}
}