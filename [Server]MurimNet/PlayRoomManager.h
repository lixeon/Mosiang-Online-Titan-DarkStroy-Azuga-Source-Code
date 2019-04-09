// PlayRoomManager.h: interface for the CPlayRoomManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLAYROOMMANAGER_H__67DAA240_0730_45E6_93B1_A696207BE6BF__INCLUDED_)
#define AFX_PLAYROOMMANAGER_H__67DAA240_0730_45E6_93B1_A696207BE6BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <HashTable.h>
#include "../4DyuchiGXGFunc/global.h"
#include "PlayRoom.h"

class CMNPlayer;


class CPlayRoomManager  
{
protected:

	CMemoryPool<CPlayRoom>		m_mpPlayRoom;
	CYHHashTable<CPlayRoom>		m_htPlayRoom;
	INDEXCR_HANDLE				m_hIndexCreator;
	
	DWORD		m_dwTotalPlayRoom;
	DWORD		m_dwMaxPlayRoom;
	
	CPlayRoom*	m_pDefaultPlayRoom;


public:
	
	CPlayRoomManager();
	virtual ~CPlayRoomManager();

	void Init( DWORD dwMaxPlayRoom );
	void Release();

	CPlayRoom* CreatePlayRoom( PLAYROOM_CREATE_INFO* pPlayRoomInfo );
	void DeletePlayRoom( DWORD dwPlayRoomIndex );
	void DeletePlayRoom( CPlayRoom* pPlayRoom );
	void DeleteAllPlayRoom();

/////
	CPlayRoom*	GetPlayRoom( DWORD dwPlayRoomIndex );

////

	BOOL EnterPlayRoom( CMNPlayer* pMNPlayer, DWORD dwPlayRoomIndex );
	void ExitPlayRoom( CMNPlayer* pMNPlayer );
	
	void SendMsg_PlayRoomList( CMNPlayer* pMNPlayer );

};

#endif // !defined(AFX_PLAYROOMMANAGER_H__67DAA240_0730_45E6_93B1_A696207BE6BF__INCLUDED_)
