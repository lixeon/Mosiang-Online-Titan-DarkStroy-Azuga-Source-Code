// SkillDelayManager.cpp: implementation of the CSkillDelayManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillDelayManager.h"
#include "MHFile.h"
#include "../[CC]Skill/DelayGroup.h"
#include "ObjectManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CSkillDelayManager)

CSkillDelayManager::CSkillDelayManager()
{
	m_htPrimereSkill.Initialize( 10 );

//	m_dwSkillStartTime	= 0;
//	m_dwSkillDelay		= 0;
//	m_dwCharacterID		=0;
}

CSkillDelayManager::~CSkillDelayManager()
{
	m_htPrimereSkill.SetPositionHead();
	PRIMERESKILL* pPrimereSkill = NULL;

	while( pPrimereSkill = m_htPrimereSkill.GetData() )
	{
		delete pPrimereSkill;
	}

	m_htPrimereSkill.RemoveAll();
}


void CSkillDelayManager::LoadSkillUseInfo()
{
	CMHFile file;

//	if( !file.Init( ".\\Resource\\PremierSkill.bin", "rb" ) )
	if( !file.Init( "./Resource/Client/PremierSkill.bin", "rb" ) )
	{
		return;
	}

	while(1)
	{
		if(file.IsEOF())
			break;

		DWORD dwGroupNum	= file.GetDword();
		DWORD dwSkillIndex	= file.GetDword();
		DWORD dwDelay		= file.GetDword();
		
		PRIMERESKILL* pPrimereSkill = new PRIMERESKILL;
		pPrimereSkill->dwSkillIndex = dwSkillIndex;
		pPrimereSkill->dwDelay		= dwDelay;
		m_htPrimereSkill.Add( pPrimereSkill, dwSkillIndex );
	}

	file.Release();
}

void CSkillDelayManager::AddSkillDelay( DWORD dwSkillIndex )
{
	PRIMERESKILL* pPrimereSkill = m_htPrimereSkill.GetData( dwSkillIndex );

	if( pPrimereSkill == NULL ) return;

//	m_dwSkillStartTime	= gCurTime;
//	m_dwSkillDelay		= pPrimereSkill->dwDelay;

	//여기서 나머지 스킬도 빨갛게 만들어주자.
	m_htPrimereSkill.SetPositionHead();
	while( PRIMERESKILL* p = m_htPrimereSkill.GetData() )
	{
		if( p->dwSkillIndex != dwSkillIndex )
		{
			HERO->GetDelayGroup()->AddDelay(
				CDelayGroup::eDK_Skill,(WORD)p->dwSkillIndex,
				pPrimereSkill->dwDelay);	//p의 스킬에 pPrimere의 딜레이를 적용한다.
		}
	}
}


void CSkillDelayManager::ContinueSkillDelay( DWORD dwDelayTime, DWORD dwRemainDelay )
{
	
/*
	if( gCurTime - m_dwSkillStartTime < m_dwSkillDelay )
	{
		DWORD dwRemainDelay = m_dwSkillDelay - ( gCurTime - m_dwSkillStartTime );

		m_htPrimereSkill.SetPositionHead();
		while( PRIMERESKILL* p = m_htPrimereSkill.GetData() )
		{
			HERO->GetDelayGroup()->AddDelay(
				CDelayGroup::eDK_Skill,p->dwSkillIndex,
				dwRemainDelay );	//p의 스킬에 pPrimere의 딜레이를 적용한다.
		}
	}
*/

	//검사
	if( dwRemainDelay > dwDelayTime )
		dwRemainDelay = dwDelayTime;

	m_htPrimereSkill.SetPositionHead();
	while( PRIMERESKILL* p = m_htPrimereSkill.GetData() )
	{
		HERO->GetDelayGroup()->AddDelay(
			CDelayGroup::eDK_Skill, (WORD)p->dwSkillIndex,
			dwDelayTime, dwDelayTime - dwRemainDelay );	//p의 스킬에 pPrimere의 딜레이를 적용한다.
	}
}

/*
BOOL CSkillDelayManager::CanExcuteSkill( DWORD dwSkillIndex )
{
	PRIMERESKILL* pPrimereSkill = m_htPrimereSkill.GetData( dwSkillIndex );

	if( pPrimereSkill == NULL ) return TRUE;

	if( gCurTime - m_dwSkillStartTime > m_dwSkillDelay )
	{
		//여기서 나머지 스킬도 빨갛게 만들어주자.
		m_htPrimereSkill.SetPositionHead();
		while( PRIMERESKILL* p = m_htPrimereSkill.GetData() )
		{
			if( p->dwSkillIndex != dwSkillIndex )
			{
				HERO->GetDelayGroup()->AddDelay(
					CDelayGroup::eDK_Skill,p->dwSkillIndex,
					pPrimereSkill->dwDelay);	//p의 스킬에 pPrimere의 딜레이를 적용한다.
			}
		}

		m_dwSkillStartTime	= gCurTime;
		m_dwSkillDelay		= pPrimereSkill->dwDelay;
		return TRUE;
	}

	return FALSE;
}
*/