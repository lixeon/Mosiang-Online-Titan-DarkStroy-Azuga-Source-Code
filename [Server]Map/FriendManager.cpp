// FriendManager.cpp: implementation of the CFriendManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FriendManager.h"
#include "MapDBMsgParser.h"
#include "UserTable.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFriendManager::CFriendManager()
{

}

CFriendManager::~CFriendManager()
{

}

void CFriendManager::UserLogOut(DWORD PlayerID)
{
	FriendNotifyLogouttoClient(PlayerID);	
}

