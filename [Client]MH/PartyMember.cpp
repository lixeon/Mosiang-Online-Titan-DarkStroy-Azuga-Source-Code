// PartyMember.cpp: implementation of the CPartyMember class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartyMember.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPartyMember::CPartyMember()
{
	m_MemberInfo.dwMemberID = 0;
	m_MemberInfo.Name[0] = 0;
	m_MemberInfo.bLogged = FALSE;
	m_MemberInfo.LifePercent = 0;
}

CPartyMember::~CPartyMember()
{

}

void CPartyMember::SetInfo(DWORD PlayerIDx,char* Name,BOOL bLog,BYTE LifePercent,BYTE ShieldPercent,BYTE NaeRyukPercent)
{
	m_MemberInfo.dwMemberID = PlayerIDx;
	strcpy(m_MemberInfo.Name,Name);
	m_MemberInfo.bLogged = bLog ? true : false;
	m_MemberInfo.LifePercent = LifePercent;
	m_MemberInfo.NaeRyukPercent = NaeRyukPercent;
}

void CPartyMember::SetInfo(PARTY_MEMBER* pInfo)
{
	m_MemberInfo.dwMemberID = pInfo->dwMemberID;
	strcpy(m_MemberInfo.Name, pInfo->Name);
	m_MemberInfo.bLogged = pInfo->bLogged ? true : false;
	m_MemberInfo.LifePercent = pInfo->LifePercent;
	m_MemberInfo.NaeRyukPercent = pInfo->NaeRyukPercent;
	m_MemberInfo.Level = pInfo->Level;
}

void CPartyMember::SetLog(BOOL bLog)
{
	m_MemberInfo.bLogged = bLog ? true : false;
}

void CPartyMember::SetLifePercent(BYTE LifePercent)
{
	if(m_MemberInfo.LifePercent != LifePercent)
	{
		m_MemberInfo.LifePercent = LifePercent;
	}
	
}

void CPartyMember::SetShieldPercent(BYTE ShieldPercent)
{
	if(m_MemberInfo.ShieldPercent != ShieldPercent)
	{
		m_MemberInfo.ShieldPercent = ShieldPercent;
	}
	
}

void CPartyMember::SetNaeRyukPercent(BYTE NaeRyukPercent)
{
	if(m_MemberInfo.NaeRyukPercent != NaeRyukPercent)
	{
		m_MemberInfo.NaeRyukPercent = NaeRyukPercent;
	}
}

void CPartyMember::SetLevel(LEVELTYPE lvl)
{
	m_MemberInfo.Level = lvl;
}

