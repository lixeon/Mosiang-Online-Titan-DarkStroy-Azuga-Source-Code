// ServerSetMng.h: interface for the CServerSetMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERSETMNG_H__9059DEBB_297D_498B_A74A_94846B527852__INCLUDED_)
#define AFX_SERVERSETMNG_H__9059DEBB_297D_498B_A74A_94846B527852__INCLUDED_

#include "ServerSet.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SERVERSETMNG USINGTON(CServerSetMng)

class CServerSetMng  
{	
protected:
	CServerSet	m_ServerSet[MAX_SERVERSET];
	int			m_nMaxServerSet;
	
public:
	DWORD		m_dwTotalUserCount;

public:
	CServerSetMng();
	virtual ~CServerSetMng();

	BOOL	Init( char* filename );
	BOOL	ReadMASList( char* filename );
	void	Release();

	CServerSet*	GetServerSet( int setindex );
	CServerSet*	GetServerSet( char* setname );
	int			GetMaxServerSet()					{ return m_nMaxServerSet; }
	DWORD		GetTotalUserCount();
	
	void		WriteClientExeVersion( char* setname, char* version );
	void		WriteUserCount( SYSTEMTIME time );
	BOOL		IsThereOnServer( char* setname );

public:	// server operate func
	void	DoServerSetOn( char* setname );
	void	DoServerSetOff( char* setname );
	void	DoServerSetRefresh( char* setname );

	void	DoQueryClientExeVersion();
	void	DoChangeClientExeVersion( char* setname, char* version );
	void	DoQueryMaxUser();
	void	DoChangeMaxUser( char* setname, DWORD maxuser );
	void	DoQueryUserLevel();
	void	DoChangeUserLevel( char* setname, int userlevel );	

	void	DoQueryUserCount();
	void	DoSendMessage( char* setname, char* pmsg );

	void	DoAutoPatch( char* setname );
	void	DoAssertMsgBox( char* setname, BOOL bState );
	void	DoAllUserDisconnect( char* setname );
	void	DoServerSetOffAfterUserDiscon( char* setname );

	void	DoMSStateAll( char* setname );
	void	DoQueryAutoPatchVersion( char* setname );

	void	DoQueryChannelInfo( char* setname );
};

EXTERNGLOBALTON(CServerSetMng)

#endif // !defined(AFX_SERVERSETMNG_H__9059DEBB_297D_498B_A74A_94846B527852__INCLUDED_)
