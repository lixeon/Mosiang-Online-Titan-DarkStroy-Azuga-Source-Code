// PetBase.cpp: implementation of the CPetBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PetBase.h"
#include "ObjectManager.h"
#include "MOTIONDESC.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPetBase::CPetBase()
{

}

CPetBase::~CPetBase()
{

}


float CPetBase::DoGetMoveSpeed()
{
	float speed=0;
	if( m_pMasterPlayer )
		speed = m_pMasterPlayer->GetMoveSpeed();

	return speed;
}



void CPetBase::OnClicked()
{
	//
}


void CPetBase::SetMotionInState(BYTE State)
{
	m_MoveInfo.KyungGongIdx = eMA_WALKAROUND;
	
	switch(State)
	{
	case eObjectState_Move:
		{
			switch(m_MoveInfo.KyungGongIdx)
			{
			case eMA_WALKAROUND:
				{
					m_ObjectState.State_Start_Motion = GetMotionIDX(eMonsterMotionN_Walk);
					m_ObjectState.State_Ing_Motion = GetMotionIDX(eMonsterMotionN_Walk);
					m_ObjectState.State_End_Motion = GetMotionIDX(eMonsterMotionN_Walk);
				}
				break;
			case eMA_RUNAWAY:
				{
					m_ObjectState.State_Start_Motion = GetMotionIDX(eMonsterMotionN_Run);
					m_ObjectState.State_Ing_Motion = GetMotionIDX(eMonsterMotionN_Run);
					m_ObjectState.State_End_Motion = GetMotionIDX(eMonsterMotionN_Run);
				}
				break;
			case eMA_PERSUIT:
				{
					m_ObjectState.State_Start_Motion = GetMotionIDX(eMonsterMotionN_Run);
					m_ObjectState.State_Ing_Motion = GetMotionIDX(eMonsterMotionN_Run);
					m_ObjectState.State_End_Motion = GetMotionIDX(eMonsterMotionN_Run);
				}
				break;
			}
			m_ObjectState.State_End_MotionTime = 0;
			
			ChangeMotion(m_ObjectState.State_Start_Motion,FALSE);
			if(m_ObjectState.State_Ing_Motion != -1)
				ChangeBaseMotion(m_ObjectState.State_Ing_Motion);
		}
		break;
	case eObjectState_None:
		{
			m_ObjectState.State_Start_Motion = GetMotionIDX(eMonsterMotionN_Stand);
			m_ObjectState.State_Ing_Motion = GetMotionIDX(eMonsterMotionN_Stand);
			m_ObjectState.State_End_Motion = GetMotionIDX(eMonsterMotionN_Stand);
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