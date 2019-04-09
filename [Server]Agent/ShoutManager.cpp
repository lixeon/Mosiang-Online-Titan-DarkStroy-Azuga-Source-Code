// ShoutManager.cpp: implementation of the CShoutManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ShoutManager.h"
#include "Network.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShoutManager::CShoutManager()
{
	m_MsgPool = new CMemoryPoolTempl<SHOUTBASE>;
	m_MsgPool->Init( 100, 50, "CShoutManager" );

	m_lastbrodtime = 0;

}

CShoutManager::~CShoutManager()
{
	SHOUTBASE* pMsg = NULL;
	PTRLISTPOS pos = m_MsgList.GetHeadPosition();
	while( pos )
	{
		pMsg = (SHOUTBASE*)m_MsgList.GetNext( pos );
		if( pMsg )
		{
//			m_MsgList.RemoveAt( pos );
			m_MsgPool->Free( pMsg );
		}
		
//		m_MsgList.GetNext( pos );
	}

	m_MsgList.RemoveAll();

	if( m_MsgPool )
		delete m_MsgPool;
}


BOOL CShoutManager::AddShoutMsg( SHOUTBASE* pSBase, SHOUTRECEIVE* pSReceive )
{
	DWORD MsgCount = 0;
	SHOUTBASE* pMsg = NULL;

	PTRLISTPOS pos = m_MsgList.GetHeadPosition();
	while( pos )
	{
		pMsg = (SHOUTBASE*)m_MsgList.GetNext(pos);
		if( pMsg->CharacterIdx == pSBase->CharacterIdx )
			++MsgCount;

		if( MsgCount >= 3 )
			break;
	}

	if( MsgCount < 3 )
	{
		pSReceive->Count = (BYTE)(MsgCount+1);
		pSReceive->Time = (m_MsgList.GetCount()/3)*5;
	}
	else
	{
		pSReceive->Count = 0;
		return FALSE;
	}

	SHOUTBASE* pShoutBase = m_MsgPool->Alloc();
	memcpy( pShoutBase, pSBase, sizeof(SHOUTBASE) );
	m_MsgList.AddTail( pShoutBase );
	
	return TRUE;
}


void CShoutManager::AddShoutMsg( SHOUTBASE* pSBase )
{
	SHOUTBASE* pShoutBase = m_MsgPool->Alloc();
	memcpy( pShoutBase, pSBase, sizeof(SHOUTBASE) );
	m_MsgList.AddTail( pShoutBase );
}


void CShoutManager::Process()
{
	if( (gCurTime - m_lastbrodtime) < 5000 )
		return;

	DWORD Count = 0;
	SEND_SHOUTBASE msg;	
	PTRLISTPOS pos = m_MsgList.GetHeadPosition();
	if( !pos )		return;	


	cPtrList list;

	while( pos && msg.Count < 3 )
	{
		SHOUTBASE* pMsg = (SHOUTBASE*)m_MsgList.GetNext( pos );
		if( !pMsg )
			break;

		memcpy( &msg.ShoutMsg[msg.Count], pMsg, sizeof(SHOUTBASE) );
		++msg.Count;

//		m_MsgList.RemoveAt( pos );

		list.AddTail( pMsg );

//		m_MsgPool->Free( pMsg );
//		m_MsgList.GetNext( pos );
	}

	pos = list.GetHeadPosition();
	while( pos )
	{
		SHOUTBASE* p = (SHOUTBASE*)list.GetNext(pos);
		m_MsgList.Remove( p );
		m_MsgPool->Free( p );
	}
	list.RemoveAll();


	// Send	
	if( msg.Count > 0 )
	{
		msg.Category = MP_CHAT;
		msg.Protocol = MP_CHAT_SHOUT_SENDALL;

		USERINFO * info = NULL;
		g_pUserTable->SetPositionUserHead();
		while(info = (USERINFO *)g_pUserTable->GetUserData())
		{
			SEND_SHOUTBASE msgTemp = msg;
			g_Network.Send2User( info->dwConnectionIndex, (char*)&msgTemp, msgTemp.GetSize() );
		}
	}

	m_lastbrodtime = gCurTime;
}
