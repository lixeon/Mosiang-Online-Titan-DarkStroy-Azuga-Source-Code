// GMPowerList.h: interface for the CGMPowerList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GMPOWERLIST_H__1298A9AD_BA4C_43FC_AB80_75DB880CA4A1__INCLUDED_)
#define AFX_GMPOWERLIST_H__1298A9AD_BA4C_43FC_AB80_75DB880CA4A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define GMINFO	USINGTON(CGMPowerList)

/*
struct GM_POWER
{
	char	GM_ID[MAX_NAME_LENGTH+1];
	DWORD	dwUserID;
	int		nPower;
};
*/

struct GM_INFO
{
	DWORD	dwConnectionIndex;
	DWORD	dwGMIndex;
	char	szGMID[MAX_NAME_LENGTH+1];
	int		nPower;
};



class CGMPowerList  
{
protected:
	cPtrList m_listGPL;	//gm power list
	
	//MonitorCheat
	BOOL	m_bMonitorCheat;
	
	//event cheat
	BOOL	m_bEvent1Start;

public:
	CGMPowerList();
	virtual ~CGMPowerList();

	void Init();

	void AddGMList( DWORD dwConnectionIdx, int nPower, DWORD dwIndex, char* strID );
	void RemoveGMList( DWORD dwConnectionIdx );
	void Release();

	int GetGMPower( DWORD dwConnectionIdx );
	GM_INFO* GetGMInfo( DWORD dwConnectionIdx );

	void SendGMLoginSuccess( DWORD dwObjectID, int nPower );
	void SendGMLoginFail( DWORD dwObjectID );

	void SetEventCheat( BOOL bStart )	{ m_bEvent1Start = bStart; }
	BOOL IsEventStarted()				{ return m_bEvent1Start; }
};

EXTERNGLOBALTON(CGMPowerList);
#endif // !defined(AFX_GMPOWERLIST_H__1298A9AD_BA4C_43FC_AB80_75DB880CA4A1__INCLUDED_)
