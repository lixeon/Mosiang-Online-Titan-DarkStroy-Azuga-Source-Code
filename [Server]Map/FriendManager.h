// FriendManager.h: interface for the CFriendManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRIENDMANAGER_H__FB86B469_8BA5_4F58_975C_80109A24DDBE__INCLUDED_)
#define AFX_FRIENDMANAGER_H__FB86B469_8BA5_4F58_975C_80109A24DDBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define FRIENDMGR CFriendManager::GetInstance()
class CFriendManager  
{
	CFriendManager();
public:
	GETINSTANCE(CFriendManager);

	virtual ~CFriendManager();
	void UserLogOut(DWORD PlayerID);
};

#endif // !defined(AFX_FRIENDMANAGER_H__FB86B469_8BA5_4F58_975C_80109A24DDBE__INCLUDED_)
