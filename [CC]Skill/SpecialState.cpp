// SpecialState.cpp: implementation of the CSpecialState class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SpecialState.h"

#ifdef _MHCLIENT_
#include "StatusIconDlg.h"
#include "ObjectManager.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSpecialState::CSpecialState(CObject* pParent)
{
	m_pParent = pParent;
#ifdef _MHCLIENT_
	m_Effect = NULL;
#endif
}

CSpecialState::~CSpecialState()
{

}

void CSpecialState::Init()
{
	m_StateTime = 0;
	m_wParam1 = 0;
	m_wParam2 = 0;
	m_fParam3 = 0;
#ifdef _MHCLIENT_
	m_StateIcon = 0;
	m_EffectNum = 0;
	m_Effect = NULL;
#endif
}

void CSpecialState::Process(DWORD tickTime)
{
	if(IsInState() == FALSE)
		return;

	if(m_StateTime <= tickTime)
	{
		EndState();
		return;
	}
	
	m_StateTime -= tickTime;
}

void CSpecialState::StartState(DWORD Time,WORD wParam1,WORD wParam2,float fParam3,WORD EffectNum,WORD StateIcon,BOOL bHeroOper)
{
	if(m_StateTime <= Time)
		m_StateTime = Time;
	
	m_wParam1 = wParam1;
	m_wParam2 = wParam2;
	m_fParam3 = fParam3;
	
#ifdef _MHCLIENT_
	m_StateIcon = StateIcon;
	if(m_StateIcon)
		STATUSICONDLG->AddIcon(m_pParent,StateIcon,0,m_StateTime);
	if(m_EffectNum != EffectNum && m_Effect)
	{
		EFFECTMGR->ForcedEndEffect(m_Effect);
		m_Effect = NULL;
	}

	if(m_Effect == NULL)
	{
		if(bHeroOper)
			m_Effect = EFFECTMGR->StartEffectProcess(EffectNum,m_pParent,0,0,0,EFFECT_FLAG_NORMAL,NULL,HERO);
		else
			m_Effect = EFFECTMGR->StartEffectProcess(EffectNum,m_pParent,0,0,0);

		m_EffectNum = EffectNum;
	}
#endif
}

void CSpecialState::EndState()
{
	m_StateTime = 0;
	m_wParam1 = 0;
	m_wParam2 = 0;
	m_fParam3 = 0;

#ifdef _MHCLIENT_
	if(m_StateIcon)
		STATUSICONDLG->RemoveIcon(m_pParent,m_StateIcon);
	if(m_Effect != NULL)
	{
		EFFECTMGR->ForcedEndEffect(m_Effect);
		m_Effect = NULL;
	}
#endif
}
