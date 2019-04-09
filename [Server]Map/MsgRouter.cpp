// MsgRouter.cpp: implementation of the CMsgRouter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MsgRouter.h"
#include "UserTable.h"
#include "AIDefine.h"
#include "Object.h"
#include "Monster.h"
#include "StateMachinen.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgRouter::CMsgRouter()
{
	m_pMsgList = NULL;
}

CMsgRouter::~CMsgRouter()
{

}

void CMsgRouter::InitMsgRouter()
{
	m_MsgPool.Init(100, 10, "MsgRouterPool");
	m_pMsgList = new cLooseLinkedList<MSG_OBJECT>;

}
void CMsgRouter::ReleaseMsgRouter()
{
	m_MsgPool.Release();
	if(m_pMsgList)
	{
		m_pMsgList->DeleteAll();
		delete m_pMsgList;
		m_pMsgList = NULL;
	}
}

void CMsgRouter::SendMsg(WORD msgID, DWORD src, DWORD dest, DWORD delay, DWORD dwData)
{
	MSG_OBJECT * pMsg = m_MsgPool.Alloc();
	pMsg->msgID			= msgID;
	pMsg->src			= src;
	pMsg->dest			= dest;
	pMsg->start			= gCurTime;
	pMsg->startDelay	= delay;
	pMsg->endDelay		= delay + 500;
	pMsg->dwData		= dwData;

	m_pMsgList->AddTail(pMsg);
}

void CMsgRouter::MsgLoop()
{
	MSG_OBJECT * obj = NULL;
	POS pos = m_pMsgList->GetFirstPos();
	while(pos)
	{
		obj = m_pMsgList->GetNextPos(pos);
		if(obj->start + obj->startDelay < gCurTime) 
		{
			DeliveryMsg(obj);
		}
	}

	
	pos = m_pMsgList->GetFirstPos();
	while(pos)
	{
		obj = m_pMsgList->GetAtPos(pos);
		if(obj->start + obj->endDelay < gCurTime) 
		{
			m_pMsgList->DeletePos(pos);
			m_MsgPool.Free(obj);
		}
		else
			m_pMsgList->GetNextPos(pos);
	}

}

void CMsgRouter::DeliveryMsg(MSG_OBJECT * obj)
{
	if(obj->dest == 0)
	{
		// send to all
		CObject * pSrcObj = g_pUserTable->FindUser(obj->src);
		if(!pSrcObj) return;

		CObject * pDestObj;
		g_pUserTable->SetPositionUserHead();
		while(pDestObj = (CObject*)g_pUserTable->GetUserData())
		{
			if(pDestObj != pSrcObj)
			{
				if(pSrcObj->GetBattleID() == pDestObj->GetBattleID() && pDestObj->GetObjectKind() & eObjectKind_Monster)
				{
					DeliveryMsgTo(obj, pSrcObj, pDestObj);
				}
			}
		}
	}
	else
	{
		CObject * pSrcObj = g_pUserTable->FindUser(obj->src);
		if(!pSrcObj) return;
		CObject * pDestObj = g_pUserTable->FindUser(obj->dest);
		if(!pDestObj) return;

		DeliveryMsgTo(obj, pSrcObj, pDestObj);
	}
}

void CMsgRouter::DeliveryMsgTo(MSG_OBJECT * obj, CObject * pSrcObj, CObject * pDestObj)
{
	if(pSrcObj->GetObjectKind() & eObjectKind_Monster)
	{
		//GSTATEMACHINE.SubProcess(pSrcObj, pDestObj, obj);
		GSTATEMACHINE.RecvMsg(pSrcObj, pDestObj, obj);
	}
}