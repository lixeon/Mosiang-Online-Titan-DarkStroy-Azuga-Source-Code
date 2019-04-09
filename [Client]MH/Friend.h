// Friend.h: interface for the CFriend class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRIEND_H__2970C43A_3B55_4C72_82AE_F16EC7D8587A__INCLUDED_)
#define AFX_FRIEND_H__2970C43A_3B55_4C72_82AE_F16EC7D8587A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "PtrList.h"
class CFriend  
{
	cPtrList m_FriendList;
	PTRLISTPOS m_pListPos[MAX_FRIEND_PAGE];
	
public:
	CFriend();
	virtual ~CFriend();
	
	void Clear();
	void SetInfo(FRIEND* pInfo,BYTE count);
	void Init();

	void Add(DWORD PlayerID, char* Name);
	BOOL Delete(DWORD PlayerID);

	void GetFriendListInfo(FRIEND_LIST* rtInfo, int num);
	void SetLogin(DWORD PlayerID, BOOL val);
	BOOL IsInFriendList(DWORD PlayerID);
	BOOL IsInFriendList(char* Name);
	int GetFriendNum();
};

#endif // !defined(AFX_FRIEND_H__2970C43A_3B55_4C72_82AE_F16EC7D8587A__INCLUDED_)
