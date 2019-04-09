// Friend.cpp: implementation of the CFriend class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Friend.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFriend::CFriend()
{
}

CFriend::~CFriend()
{	
	Clear();
}

void CFriend::Init()
{
	for(int i=0; i<MAX_FRIEND_PAGE; ++i)
		m_pListPos[i] = NULL;
	Clear();
}

void CFriend::Clear()
{
	PTRLISTPOS pos = m_FriendList.GetHeadPosition();
	while( pos )
	{
		FRIEND* pFriend = (FRIEND*)m_FriendList.GetNext( pos );
		SAFE_DELETE(pFriend);
	}
	m_FriendList.RemoveAll();
}

void CFriend::SetInfo(FRIEND* pInfo,BYTE count)
{
	Clear();

	int num;
	if(count > MAX_FRIEND_NUM)
	{
		char buf[128];
		sprintf(buf, "character_idx : %u", HEROID );
		ASSERTMSG(0, buf);
		count = MAX_FRIEND_NUM;
	}
	for(int i=0; i<count; ++i)
	{
		if(pInfo[i].Id == 0)
			break;
		FRIEND* pFriendInfo = new FRIEND;
		pFriendInfo->Id = pInfo[i].Id;
		pFriendInfo->IsLoggIn = pInfo[i].IsLoggIn;
		SafeStrCpy(pFriendInfo->Name, pInfo[i].Name, MAX_NAME_LENGTH+1);
		m_FriendList.AddTail(pFriendInfo);
		
		if( (i % MAX_FRIEND_LIST) == 0 )
		{
			num = i/MAX_FRIEND_LIST;
			m_pListPos[num] = m_FriendList.GetTailPosition();
		}			
	}
}

void CFriend::GetFriendListInfo(FRIEND_LIST* rtInfo, int num)
{
	FRIEND* pFriend;	
	if((num-1) < 0 || (num-1) >= MAX_FRIEND_PAGE)
	{
		char buf[128];
		sprintf(buf, "Friend gotoPage is strange! gotopage : %d", num);
		ASSERTMSG(0, buf);
		return;
	}
	PTRLISTPOS pos = m_pListPos[num-1];
	for(int i=0; i<MAX_FRIEND_LIST; ++i)
	{
		if(pos)
		{ 
			pFriend = (FRIEND*)m_FriendList.GetAt(pos);
			if(pFriend)
			{
				rtInfo->Friend[i].Id = pFriend->Id;
				rtInfo->Friend[i].IsLoggIn = pFriend->IsLoggIn;
				SafeStrCpy(rtInfo->Friend[i].Name, pFriend->Name, MAX_NAME_LENGTH+1);
			
				m_FriendList.GetNext(pos);
			}
		}
		else
			break;
	}
	rtInfo->totalnum = (m_FriendList.GetCount()-1)/MAX_FRIEND_LIST + 1;
}

void CFriend::Add(DWORD PlayerID, char* Name)
{
	if(m_FriendList.GetCount() >= MAX_FRIEND_NUM)
		ASSERT(0);
	
	FRIEND* pInfo = new FRIEND;
	pInfo->Id = PlayerID;
	pInfo->IsLoggIn = TRUE;
	SafeStrCpy(pInfo->Name, Name, MAX_NAME_LENGTH+1);

	m_FriendList.AddTail(pInfo);

	//ptrlistpos fix
	if((m_FriendList.GetCount()-1)%MAX_FRIEND_LIST == 0)
	{
		int num = (m_FriendList.GetCount()-1)/MAX_FRIEND_LIST;
		m_pListPos[num] = m_FriendList.GetTailPosition();
	}
}

BOOL CFriend::Delete(DWORD PlayerID)
{
	PTRLISTPOS pos = m_FriendList.GetHeadPosition();
	FRIEND * pInfo = NULL;
	PTRLISTPOS temppos;
	BOOL bfind = FALSE;

	while(pos)
	{
		pInfo = (FRIEND * )m_FriendList.GetAt(pos);
		if(pInfo && pInfo->Id == PlayerID)
		{
			temppos = pos;
			for(int i=0; i<MAX_FRIEND_PAGE; ++i)
			{
				// 삭제된 녀석을 찾았는데 포지션이 저장된 녀석이면
				if(!bfind && temppos == m_pListPos[i])
				{
					m_FriendList.GetNext(temppos);
					if(temppos)
						m_pListPos[i] = temppos;
					else
					{
						m_pListPos[i] = NULL;
						break;
					}

					bfind = TRUE;					
				}
				// 찾은 후에 뒤에 녀석이 있으면
				else if(bfind && m_pListPos[i])
				{
					temppos = m_pListPos[i];
					m_FriendList.GetNext(temppos);

					if(temppos)
						m_pListPos[i] = temppos;
					else
					{
						m_pListPos[i] = NULL;
						break;
					}			
				}
			}
			m_FriendList.RemoveAt(pos);
			SAFE_DELETE(pInfo);
			return TRUE;
		}
		m_FriendList.GetNext(pos);
	}
	return FALSE;
}

void CFriend::SetLogin(DWORD PlayerID, BOOL val)
{
	PTRLISTSEARCHSTART( m_FriendList, FRIEND* , pInfo)
		if(pInfo->Id == PlayerID)
			pInfo->IsLoggIn = val;
	PTRLISTSEARCHEND
}

BOOL CFriend::IsInFriendList(DWORD PlayerID)
{
	PTRLISTSEARCHSTART( m_FriendList, FRIEND* , pInfo)
		if(pInfo->Id == PlayerID)
			return TRUE;
	PTRLISTSEARCHEND
	return FALSE;
}

BOOL CFriend::IsInFriendList(char* Name)
{
	PTRLISTSEARCHSTART( m_FriendList, FRIEND* , pInfo)
		if(strcmp(pInfo->Name, Name) == 0)
			return TRUE;
	PTRLISTSEARCHEND
	return FALSE;
}

int CFriend::GetFriendNum()
{
	return m_FriendList.GetCount();
}