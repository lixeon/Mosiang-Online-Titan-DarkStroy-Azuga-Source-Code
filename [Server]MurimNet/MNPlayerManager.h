// MNPlayerManager.h: interface for the CMNPlayerManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MNPLAYERMANAGER_H__1DAE812D_374A_4727_B72E_C87D9389CCC5__INCLUDED_)
#define AFX_MNPLAYERMANAGER_H__1DAE812D_374A_4727_B72E_C87D9389CCC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <HashTable.h>


class CMNPlayer;


class CMNPlayerManager  
{
protected:

//all connected player list
//all connected player total num
	CMemoryPool<CMNPlayer>*		m_mpPlayerPool;
	CYHHashTable<CMNPlayer>		m_htPlayer;

	int		m_nTotalPlayer;
	DWORD	m_dwMaxPlayer;

public:

	CMNPlayerManager();
	virtual ~CMNPlayerManager();

	void Init( DWORD dwMaxPlayer );
	void Release();

//player add
//player delete
	CMNPlayer* AddPlayer( DWORD dwPlayerID, DWORD dwAgentNum, DWORD UniqueIDinAgent, DWORD dwBackMapNum );
	void DeletePlayer( DWORD dwPlayerID );
	void DeleteAllPlayers();

	CMNPlayer* FindPlayer( DWORD dwPlayerID );
};

#endif // !defined(AFX_MNPLAYERMANAGER_H__1DAE812D_374A_4727_B72E_C87D9389CCC5__INCLUDED_)
