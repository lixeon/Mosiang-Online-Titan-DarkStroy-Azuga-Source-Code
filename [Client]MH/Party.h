// Party.h: interface for the CParty class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARTY_H__F6D978D7_2592_4CB4_B54E_2AF0835E1A20__INCLUDED_)
#define AFX_PARTY_H__F6D978D7_2592_4CB4_B54E_2AF0835E1A20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PartyMember.h"

class CParty  
{	
//	DWORD m_PartyDBIdx;
	CPartyMember m_Member[MAX_PARTY_LISTNUM];	// 마스터는 [0]!!
	
	DWORD m_TacticObjectID;
	BYTE m_Option;			//a distribute way that dropped item and dropped money
	//2008. 5. 21. CBH - 파티 추가 옵션 관련 추가

	PARTY_ADDOPTION m_AddOption;

public:
	CParty();
	virtual ~CParty();
	
	void InitParty(PARTY_INFO* pInfo);
	
//	void SetPartyIdx(DWORD Partyid);
	void SetMaster(PARTY_MEMBER* pMasterInfo);
	void SetMaster(DWORD MasterIdx,char* strName,BOOL bLog,BYTE LifePercent, BYTE ShieldPercent,BYTE NaeRyukPercent);
	DWORD GetMemberID(int num); //파티내에서 멤버 선택
	int GetMemberNum();
//	DWORD GetPartyIdx();
	DWORD GetMasterIdx();

	char* GetMasterName();

	void AddMember(PARTY_MEMBER* pMemberInfo);
	void DelMember(DWORD id);
	void BreakUpResult();

	void LogIn(PARTY_MEMBER* pMemberInfo);
	void LogOut(DWORD PlayerID);
	
	char* GetMemberName(DWORD MemberID);
	
	void SetMemberLifePercent(DWORD PlayerID, DWORD lifePercent);
	BYTE GetMemberLifePercent(DWORD PlayerID);
	
	void SetMemberShieldPercent(DWORD PlayerID, DWORD ShieldPercent);
	BYTE GetMemberShieldPercent(DWORD PlayerID);

	void SetMemberNaeRyukPercent(DWORD PlayerID, DWORD naeryukPercent);
	BYTE GetMemberNaeRyukPercent(DWORD PlayerID);
		
	void SetMemberLevel(DWORD PlayerID, LEVELTYPE lvl);
	LEVELTYPE GetMemberLevel(DWORD PlayerID);

	PARTY_MEMBER* GetPartyMemberInfo(int i);
	
	void Clear();
	
	BOOL IsLogIn(int n)		{	return m_Member[n].IsMemberLog();	}

	void ChangeMaster(DWORD dwPlayerID);
	
	BOOL IsPartyMember(DWORD dwPlayerID);

	DWORD GetTacticObjectID()	{	return m_TacticObjectID;	}
	void SetTacticObjectID(DWORD TObjID)	{	m_TacticObjectID = TObjID;	}
	
	void SetOption(BYTE Option) { m_Option = Option;	}
	//2008. 5. 21. CBH - 파티 추가 옵션 관련 추가
	void SetAddOption(PARTY_ADDOPTION AddOption);


#ifdef _JAPAN_LOCAL_
	BOOL IsHelpPartyMember();
#endif
	
};

#endif // !defined(AFX_PARTY_H__F6D978D7_2592_4CB4_B54E_2AF0835E1A20__INCLUDED_)

