// FriendManager.h: interface for the CFriendManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRIENDMANAGER_H__1AA217FD_0FAF_47AE_8E52_BD5B39AD6C71__INCLUDED_)
#define AFX_FRIENDMANAGER_H__1AA217FD_0FAF_47AE_8E52_BD5B39AD6C71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define FRIENDMGR USINGTON(CFriendManager)

#include "Friend.h"

class CFriendManager  
{
	DWORD m_RequestPlayerID;
	char m_RequestPlayerName[MAX_NAME_LENGTH+1];	

	CFriend m_Friend;
	BOOL m_bFriendInit;

public:
	CFriendManager();
	virtual ~CFriendManager();

//	//MAKESINGLETON(CFriendManager);
	
	void NetworkMsgParse(BYTE Protocol,void* pMsg);

//Ä£±¸	
	void Init();
	void SetFriendInit(BOOL val) { m_bFriendInit = val; }
	BOOL IsFriendInit() { return m_bFriendInit;	}

	void LogInNotify();
	void SetRequestPlayer(DWORD FromPlayerID, char* Name);
	void AddDelFriendSyn(char* pFriendName, BYTE Protocol);
	void FriendInviteAccept();
	void FriendInviteDeny();
	void FriendListSyn(WORD SelectedPage);
	void DelFriendSynbyID(DWORD FriendID,BOOL bLast);
	void AddFriendSynbyID(DWORD TargetID, char* TargetName, char* FromName);

	DWORD GetRequestPlayerID() { return m_RequestPlayerID;	}
	char* GetRequestPlayerName() { return m_RequestPlayerName;	}
	
	void GetFriendListInfo(FRIEND_LIST* rtInfo, int pagenum);
};

BOOL FriendFunc(char* cheat);
EXTERNGLOBALTON(CFriendManager)
#endif // !defined(AFX_FRIENDMANAGER_H__1AA217FD_0FAF_47AE_8E52_BD5B39AD6C71__INCLUDED_)
