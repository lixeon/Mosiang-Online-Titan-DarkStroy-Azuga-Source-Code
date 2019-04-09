// SkillObjectTerminator_Time.cpp: implementation of the CSkillObjectTerminator_Time class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillInfo.h"
#include "SkillObjectTerminator_Time.h"

#ifdef _MAPSERVER_
#include "SkillManager_Server.h"
#endif

#ifdef _MHCLIENT_
#include "SkillManager_Client.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillObjectTerminator_Time::CSkillObjectTerminator_Time(SKILLINFO* pSkillInfo)
: CSkillObjectTerminator(pSkillInfo)
{
	m_LimitTime = pSkillInfo->Duration;

}

CSkillObjectTerminator_Time::~CSkillObjectTerminator_Time()
{

}

void CSkillObjectTerminator_Time::Init(CSkillInfo* pSkillInfo,WORD SkillLevel)
{
	m_AddionalTime = pSkillInfo->GetSkillInfo()->SkillAdditionalTime[SkillLevel-1];
}

BOOL CSkillObjectTerminator_Time::CheckTerminate(SKILLOBJECT_INFO* pSkillObjectInfo)
{
	////////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공변환
	// 지속시간 증가
	SKILLOPTION* pOption = SKILLMGR->GetSkillOption(pSkillObjectInfo->Option);

	if(pOption)
	{
		m_AddionalTime += pOption->Duration;
	}
	////////////////////////////////////////////////////////////////////////////

	DWORD ElapsedTime = gCurTime - pSkillObjectInfo->StartTime;
	if(ElapsedTime >= m_LimitTime + m_AddionalTime)
		return TRUE;
	else
		return FALSE;
}