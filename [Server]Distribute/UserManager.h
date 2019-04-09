// UserManager.h: interface for the CUserManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERMANAGER_H__E2D3542E_2B08_40A3_8DD4_D5C10FD626A1__INCLUDED_)
#define AFX_USERMANAGER_H__E2D3542E_2B08_40A3_8DD4_D5C10FD626A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CUserManager  
{
	char	CLIENT_VERSION[256];
	BYTE	m_LimitUserLevel;
	BYTE	m_LimitAge;
public:
	CUserManager();
	virtual ~CUserManager();

	void	LoadDistributeDesc();

	BOOL	IsValidUserLevel(BYTE UserLevel);	
	BOOL	IsValidVersion(char * szVersion);
	BYTE	GetLimitAge()				{ return m_LimitAge; }

	void	LoadVersion();
	void	SaveVersion();
	void	SetVersion(char *szV);
	char *	GetVersion();
	
	void	SetUserLevel( BYTE level )	{ m_LimitUserLevel = level; }
	BYTE	GetUserLevel()				{ return m_LimitUserLevel; }
};

extern CUserManager gUserMGR;
#endif // !defined(AFX_USERMANAGER_H__E2D3542E_2B08_40A3_8DD4_D5C10FD626A1__INCLUDED_)
