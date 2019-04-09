// Party.cpp: implementation of the CParty class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Party.h"
#include "PartyManager.h"
#include "GameIn.h"
#include "ObjectManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CParty::CParty()
{
//	m_PartyDBIdx = 0;
	m_TacticObjectID = 0;
	m_Option = 0;
	//2008. 5. 21. CBH - 파티 추가 옵션 관련 추가
	memset(&m_AddOption, 0, sizeof(PARTY_ADDOPTION));
}

CParty::~CParty()
{
	
}

void CParty::InitParty(PARTY_INFO* pInfo)
{
	m_TacticObjectID = 0;
	m_Option = 0;
	memset(&m_AddOption, 0, sizeof(PARTY_ADDOPTION));

	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
		m_Member[n].Clear();

	HERO->SetPartyIdx(pInfo->PartyDBIdx);
	SetMaster(&pInfo->Member[0]);	
	//2008. 5. 22. CBH - 방파 추가 옵션 셋팅
	SetAddOption(pInfo->PartyAddOption);
	//SetOption(pInfo->Option);
	
	for(n=1;n<MAX_PARTY_LISTNUM;++n)
	{
		if(pInfo->Member[n].dwMemberID != 0)
		{
			AddMember(&pInfo->Member[n]);
		}
	}
}
/*
void CParty::SetPartyIdx(DWORD Partyid)
{
	m_PartyDBIdx = Partyid;
	if(HERO)
		HERO->SetPartyIdx(Partyid);
}
*/
void CParty::SetMaster(PARTY_MEMBER* pMasterInfo)
{
	m_Member[0].SetInfo(pMasterInfo);
}
void CParty::SetMaster(DWORD MasterIdx,char* strName,BOOL bLog,BYTE LifePercent, BYTE ShieldPercent,BYTE NaeRyukPercent)
{
	m_Member[0].SetInfo(MasterIdx,strName,bLog,LifePercent,ShieldPercent,NaeRyukPercent);
}
/*
DWORD CParty::GetPartyIdx()
{
	return m_PartyDBIdx;
}
*/
DWORD CParty::GetMasterIdx()
{
//	ASSERT(m_Member[0].GetMemberID());
	return m_Member[0].GetMemberID();
}

char* CParty::GetMasterName()
{
//	ASSERT(m_Member[0].GetMemberID());
	return m_Member[0].GetMemberName();
}


void CParty::AddMember(PARTY_MEMBER* pMemberInfo) 
{
	for(int n=1;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].GetMemberID() == 0)
		{
/*
			m_Member[n].SetInfo(pMemberInfo->dwMemberID,pMemberInfo->Name,
							pMemberInfo->bLogged,
							pMemberInfo->LifePercent,
							pMemberInfo->ShieldPercent,
							pMemberInfo->NaeRyukPercent);*/
			m_Member[n].SetInfo(pMemberInfo);

			return;
		}
	}
//	ASSERT(0);
}

void CParty::DelMember(DWORD id)
{
	for(int n=1;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].GetMemberID() == id)
		{
			m_Member[n].Clear();
			return;
		}
	}
//	ASSERT(0);
}

void CParty::LogIn(PARTY_MEMBER* pMemberInfo)
{
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].GetMemberID() == pMemberInfo->dwMemberID)
		{
/*
			m_Member[n].SetInfo(pMemberInfo->dwMemberID,pMemberInfo->Name,
							pMemberInfo->bLogged,
							pMemberInfo->LifePercent, 
							pMemberInfo->ShieldPercent, 
							pMemberInfo->NaeRyukPercent);*/
			m_Member[n].SetInfo(pMemberInfo);
			return;
		}
	}
//	ASSERT(0);
}

void CParty::LogOut(DWORD PlayerID)
{
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].GetMemberID() == PlayerID)
		{
			m_Member[n].SetLog(FALSE);
			m_Member[n].SetLifePercent(0);
			m_Member[n].SetNaeRyukPercent(0);
			return;
		}
	}
//	ASSERT(0);
}

char* CParty::GetMemberName(DWORD MemberID)
{	
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].GetMemberID() == MemberID)
		{
			return m_Member[n].GetMemberName();
		}
	}

	// 없는 파티원을 찾으려 했다.
//	ASSERT(0);
	return "";
}

DWORD CParty::GetMemberID(int num)
{
//	ASSERT(num < MAX_PARTY_LISTNUM);
	return m_Member[num].GetMemberID();
}

void CParty::SetMemberLifePercent(DWORD PlayerID, DWORD lifePercent)
{
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].GetMemberID() == PlayerID)
		{
			m_Member[n].SetLifePercent((BYTE)lifePercent);
			return;
		}
	}
//	ASSERT(0);
}

BYTE CParty::GetMemberLifePercent(DWORD PlayerID)
{
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].GetMemberID() == PlayerID)
		{
			return (m_Member[n].GetLifePercent());
		}
	}
//	ASSERT(0);
	return 0;
}

void CParty::SetMemberShieldPercent(DWORD PlayerID, DWORD ShieldPercent)
{
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].GetMemberID() == PlayerID)
		{
			m_Member[n].SetShieldPercent((BYTE)ShieldPercent);
			return;
		}
	}
//	ASSERT(0);
}

BYTE CParty::GetMemberShieldPercent(DWORD PlayerID)
{
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].GetMemberID() == PlayerID)
		{
			return (m_Member[n].GetShieldPercent());
		}
	}
//	ASSERT(0);
	return 0;
}

void CParty::SetMemberNaeRyukPercent(DWORD PlayerID, DWORD naeryukPercent)
{
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].GetMemberID() == PlayerID)
		{
			m_Member[n].SetNaeRyukPercent((BYTE)naeryukPercent);
			return;
		}
	}
//	ASSERT(0);
}

void CParty::SetMemberLevel(DWORD PlayerID, LEVELTYPE lvl)
{
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].GetMemberID() == PlayerID)
		{
			m_Member[n].SetLevel(lvl);
			return;
		}
	}
//	ASSERT(0);
}

BYTE CParty::GetMemberNaeRyukPercent(DWORD PlayerID)
{
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].GetMemberID() == PlayerID)
		{
			return (m_Member[n].GetNaeRyukPercent());
		}
	}
//	ASSERT(0);
	return 0;
}

LEVELTYPE CParty::GetMemberLevel(DWORD PlayerID)
{
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].GetMemberID() == PlayerID)
		{
			return (m_Member[n].GetLevel());
		}
	}
//	ASSERT(0);
	return 0;
}


void CParty::BreakUpResult()
{
	for(int n=0; n<MAX_PARTY_LISTNUM; ++n) 
	{			
		if(m_Member[n].GetMemberID() != 0)	
		{
			m_Member[n].SetInfo(0,"",0,0,0,0);
		} //if
	} //for	
}

void CParty::Clear()
{
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		m_Member[n].Clear();
	}
}

void CParty::ChangeMaster(DWORD dwPlayerID)
{
	for(int n=1;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].GetMemberID() == dwPlayerID)
		{
			//swap
			CPartyMember temp;
			memcpy(&temp,&m_Member[0],sizeof(CPartyMember));
			memcpy(&m_Member[0],&m_Member[n],sizeof(CPartyMember));
			memcpy(&m_Member[n],&temp,sizeof(CPartyMember));
			break;
		}		
	}
}

BOOL CParty::IsPartyMember(DWORD dwPlayerID)
{
//	ASSERT(dwPlayerID);
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].GetMemberID() == dwPlayerID)
			return TRUE;
	}
	return FALSE;
}

PARTY_MEMBER* CParty::GetPartyMemberInfo(int i)
{
//	ASSERT(i < MAX_PARTY_LISTNUM);
	return m_Member[i].GetMemberInfo();
}

int CParty::GetMemberNum()
{
	int num=0;
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].GetMemberID() != 0)
			++num;
	}
	return num;
}

//2008. 5. 21. CBH - 파티 추가 옵션 관련 추가
void CParty::SetAddOption(PARTY_ADDOPTION AddOption)
{
	m_AddOption = AddOption;
	m_Option = AddOption.bOption;
}

#ifdef _JAPAN_LOCAL_

BOOL CParty::IsHelpPartyMember()
{


	if( HERO == NULL ) return FALSE;
	if( HERO->GetPartyIdx() == 0 ) return FALSE; 

	for( int i = 0 ; i< MAX_PARTY_LISTNUM ; ++i )
	{
		if( m_Member[i].IsMemberLog() && m_Member[i].GetMemberID() != HERO->GetID() )
		{

			CPlayer* pOther = (CPlayer*)OBJECTMGR->GetObject( m_Member[i].GetMemberID() );
			if( pOther == NULL ) continue;
			
			if( HERO->WhatIsAttrRelation( pOther->GetMainCharAttr() ) == eCAR_Need_Help )
			{
				VECTOR3 pMyPos, pOtherPos;
				HERO->GetPosition( &pMyPos );
				pOther->GetPosition( &pOtherPos );
				float dist = roughGetLength( pOtherPos.x - pMyPos.x, pOtherPos.z - pMyPos.z );
				if( dist <= 3000.0f )
					return TRUE;
			}
		}
	}

	return FALSE;
}
#endif
