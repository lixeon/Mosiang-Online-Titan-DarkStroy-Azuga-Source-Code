// WantedInfo.h: interface for the CWantedInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WANTEDINFO_H__E9AACD67_134E_4604_9D8A_56E9BF6B6725__INCLUDED_)
#define AFX_WANTEDINFO_H__E9AACD67_134E_4604_9D8A_56E9BF6B6725__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWantedInfo  
{
	WANTEDINFO m_WantedInfo;
public:
	CWantedInfo();
	virtual ~CWantedInfo();

	void InitWantedInfo(WANTEDINFO * pInfo);

	WANTEDTYPE GetWantedIDX() { return m_WantedInfo.WantedIDX;	}

	DWORD GetWantedChrIDX() { return m_WantedInfo.WantedChrIDX;	}
	char* GetWantedChrName() { return m_WantedInfo.WantedChrName; }

	DWORD GetRegistChrIDX() { return m_WantedInfo.RegistChrIDX;	}

	void SetTotalPrize(MONEYTYPE TotalPrize);
	MONEYTYPE GetTotalPrize() { return m_WantedInfo.TotalPrize;	}
	
	int GetTotalVolunteerNum();
	void IncreVolunteerNum();
	void DecreVolunteerNum();
};

#endif // !defined(AFX_WANTEDINFO_H__E9AACD67_134E_4604_9D8A_56E9BF6B6725__INCLUDED_)
