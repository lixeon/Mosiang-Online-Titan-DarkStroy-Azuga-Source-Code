// Npc.cpp: implementation of the CNpc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Npc.h"
#include "GameResourceManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNpc::CNpc()
{
	m_pSInfo = NULL;
}

CNpc::~CNpc()
{

}


void CNpc::InitNpc(NPC_TOTALINFO* pTotalInfo)
{
	memcpy(&m_NpcInfo,pTotalInfo,sizeof(NPC_TOTALINFO));
	memset(m_QuestList, 0, 4*MAX_NPCQUEST_LIST);
	m_QuestCount = 0;
	m_pSInfo = GAMERESRCMNGR->GetNpcInfo(GetNpcKind());
}



void CNpc::Process()
{

	CObject::Process();
}

void CNpc::SetMotionInState(BYTE State)
{
	switch(State)
	{
	case eObjectState_Move:
		{
			ASSERT(0);
		}
		break;
	case eObjectState_None:
		{
			m_ObjectState.State_Start_Motion = eMonsterMotion_Standard;
			m_ObjectState.State_Ing_Motion = eMonsterMotion_Standard;
			m_ObjectState.State_End_Motion = eMonsterMotion_Standard;
			m_ObjectState.State_End_MotionTime = 0;
		}
		break;
	default:
		{
			m_ObjectState.State_Start_Motion = -1;
			m_ObjectState.State_Ing_Motion = -1;
			m_ObjectState.State_End_Motion = -1;
			m_ObjectState.State_End_MotionTime = 0;
		}
		break;
	}
}

void CNpc::OnClicked()
{
	WORD kind = GetNpcKind();

	switch( kind )
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 8:
	case 10:
	case 12:
	case 13:
	case 15:
	case 18:
	case 22:
	case 25:
	case 45:
	case 71:
	case 74:
	case 75:
	case 76:
	case 77:
	case 78:
	case 79:
	case 84:
		ChangeBaseMotion(1);
		ChangeMotion(2,FALSE);
		break;
	}

/*	if( GetNpcKind() <= 5 || GetNpcKind() == 8 || GetNpcKind() == 10 || GetNpcKind() == 12 ||
	    GetNpcKind() == 13 || GetNpcKind() == 15 || GetNpcKind() == 25 ) 
	{
		ChangeBaseMotion(1);
		ChangeMotion(2,FALSE);
	}	
	*/
}


void CNpc::AddQuestIndex(int Idx)
{
	if(Idx <= 0)		return;

	m_QuestList[m_QuestCount] = Idx;
	++m_QuestCount;
}

void CNpc::Die(CObject* pAttacker,BOOL bFatalDamage,BOOL bCritical, BOOL bDecisive)
{
	if(GetNpcJob() == BOMUL_ROLE)
	{
		ChangeBaseMotion(1);
		ChangeMotion(2,FALSE);
	}
}