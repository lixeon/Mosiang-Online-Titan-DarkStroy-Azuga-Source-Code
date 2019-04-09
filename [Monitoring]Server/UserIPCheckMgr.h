// UserIPCheckMgr.h: interface for the CUserIPCheckMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERIPCHECKMGR_H__BC37305F_9CD0_4C52_9F1D_948D50952626__INCLUDED_)
#define AFX_USERIPCHECKMGR_H__BC37305F_9CD0_4C52_9F1D_948D50952626__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define IPCHECKMGR USINGTON(CUserIPCheckMgr)

#define	MAX_IPARRAY	100

struct IP
{
	char	sIP[32];
};
struct OPERID
{
	DWORD	dwIdx;
	char	sID[17];
};

class CUserIPCheckMgr  
{
	IP			m_IPArray[MAX_IPARRAY];
	int			m_nMaxIP;

public:
	CUserIPCheckMgr();
	virtual ~CUserIPCheckMgr();

	void	Release();
	BOOL	IsThereIP( char* pIP );
	void	RegistIP( char* pIP );
	void	UnRegistIP( char* pIP );

	BOOL	CheckIP( DWORD dwConnectionIndex );

protected:
	OPERID		m_IDArray[MAX_IPARRAY];

public:
	void		RegistID( DWORD dwConnectionIdx, DWORD dwIdx, char* pName );
	OPERID*		GetID( DWORD dwConnectionIdx );
};

EXTERNGLOBALTON(CUserIPCheckMgr)

#endif // !defined(AFX_USERIPCHECKMGR_H__BC37305F_9CD0_4C52_9F1D_948D50952626__INCLUDED_)
