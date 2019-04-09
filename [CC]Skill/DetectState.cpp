#include "stdafx.h"
#include ".\detectstate.h"

CDetectState::CDetectState(CObject* pParent)
: CSpecialState(pParent)
{
}

CDetectState::~CDetectState(void)
{
}

void CDetectState::StartState(DWORD Time,WORD wParam1,WORD wParam2,float fParam3,WORD EffectNum,WORD StateIcon,BOOL bHeroOper)
{
#ifdef _MAPSERVER_
	if(m_pParent->GetSingleSpecialState(eSingleSpecialState_Detect))
		return;

	m_pParent->SetSingleSpecialState(eSingleSpecialState_DetectItem, true);
	m_pParent->SendSingleSpeicalStateAck(eSingleSpecialState_DetectItem, true);
#endif

	CSpecialState::StartState(Time,wParam1,wParam2,fParam3,EffectNum,StateIcon,bHeroOper);
}

void CDetectState::EndState()
{
#ifdef _MAPSERVER_
	if( m_pParent->GetSingleSpecialState(eSingleSpecialState_DetectItem) )
	{
		m_pParent->SetSingleSpecialStateUsedTime(eSingleSpecialState_DetectItem, gCurTime);
		m_pParent->SetSingleSpecialState(eSingleSpecialState_DetectItem, false);
		m_pParent->SendSingleSpeicalStateAck(eSingleSpecialState_DetectItem, false);
	}
#endif

	CSpecialState::EndState();
}
