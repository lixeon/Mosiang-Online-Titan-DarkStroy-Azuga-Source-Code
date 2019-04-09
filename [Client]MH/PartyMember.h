// PartyMember.h: interface for the CPartyMember class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARTYMEMBER_H__52175D0F_A4F2_41D9_BF75_58E721259163__INCLUDED_)
#define AFX_PARTYMEMBER_H__52175D0F_A4F2_41D9_BF75_58E721259163__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPartyMember  
{
	PARTY_MEMBER m_MemberInfo;
		
public:
	CPartyMember();
	virtual ~CPartyMember();

	void SetInfo(DWORD PlayerIDx,char* Name,BOOL bLog,BYTE LifePercent,BYTE ShieldPercent,BYTE NaeRyukPercent);
	void SetInfo(PARTY_MEMBER* pInfo);
	void SetLog(BOOL bLog);
	void SetLifePercent(BYTE LifePercent);
	void SetShieldPercent(BYTE ShieldPercent);
	void SetNaeRyukPercent(BYTE NaeRyukPercent);
	void SetLevel(LEVELTYPE lvl);

	DWORD GetMemberID()			{
		return DWORD(m_MemberInfo.dwMemberID);
	}
	char* GetMemberName()		{	return m_MemberInfo.Name;			}
	BOOL IsMemberLog()			{	return m_MemberInfo.bLogged;		}
	BYTE GetLifePercent()		{	return m_MemberInfo.LifePercent;	}
	BYTE GetShieldPercent()		{	return m_MemberInfo.ShieldPercent;	}
	BYTE GetNaeRyukPercent()	{	return m_MemberInfo.NaeRyukPercent;	}
	LEVELTYPE GetLevel()		{	return m_MemberInfo.Level;			}

	PARTY_MEMBER* GetMemberInfo()	{	return &m_MemberInfo;	}

	void Clear()				{	m_MemberInfo.dwMemberID = 0;		}
};

#endif // !defined(AFX_PARTYMEMBER_H__52175D0F_A4F2_41D9_BF75_58E721259163__INCLUDED_)
