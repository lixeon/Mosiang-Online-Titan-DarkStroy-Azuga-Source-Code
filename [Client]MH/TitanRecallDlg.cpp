// TitanRecallDlg.cpp: implementation of the CTitanPartsChangeDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "TitanRecallDlg.h"
#include "Hero.h"
#include "ObjectStateManager.h"
#include "ObjectManager.h"
#include "cWindowManager.h"
#include "WindowIDEnum.h"
#include "ChatManager.h"


CTitanRecallDlg::CTitanRecallDlg()
{
	m_bSuccessRecall = FALSE;
}

CTitanRecallDlg::~CTitanRecallDlg()
{

}

void CTitanRecallDlg::Linking()
{	
	m_pProgressGuagen = (CObjectGuagen*)GetWindowForID(TITAN_RECALL_GUAGE);
	m_pRemaintimeStatic = (cStatic*)GetWindowForID(TITAN_RECALL_TIME);

	SetSuccessTime(7000); //7초 기본 시간 셋팅
}

void CTitanRecallDlg::Render()
{	
	if(GetSuccessProgress() == TRUE)
	{
		//카운트가 완료됬다는 메세지를 보낸다
		MSGBASE msg;
		msg.Category = MP_TITAN;
		msg.Protocol = MP_TITAN_RECALL_SYN;
		msg.dwObjectID = HERO->GetID();
		NETWORK->Send(&msg, sizeof(msg));
		
		InitProgress();
	}

	CProgressBarDlg::Render();
}

BOOL CTitanRecallDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{
	switch(we)
	{			
	case WE_CLOSEWINDOW:
		{			
			if( HERO->GetState() == eObjectState_Society )
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Society);
			
			return TRUE;
		}
		break;
	}

	switch(lId)
	{
	case TITAN_RECALL_CANCEL:
		{
			MSGBASE msg;
			msg.Category = MP_TITAN;
			msg.Protocol = MP_TITAN_RECALL_CANCEL_SYN;		
			msg.dwObjectID = HERO->GetID();
			NETWORK->Send(&msg, sizeof(msg));			
		}
		break;
	}

	return TRUE;
}

BOOL CTitanRecallDlg::GetSuccessRecall()
{
	return m_bSuccessRecall;
}

void CTitanRecallDlg::SetSuccessRecall(BOOL bVal)
{
	m_bSuccessRecall = bVal;
}
