// MsgTable.cpp: implementation of the CMsgTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MsgTable.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgTable g_MsgTable;

#define MAX_MSGTABLE	500

CMsgTable::CMsgTable()
{
	m_mpMsg.Init( MAX_MSGTABLE, 100, "MsgTable" );			//max msg... confirm
	m_htMsg.Initialize( MAX_MSGTABLE );
	m_ICHandle = ICCreate();
	ICInitialize( m_ICHandle, MAX_MSGTABLE * 2 );	//max = 1000
}

CMsgTable::~CMsgTable()
{
	m_htMsg.RemoveAll();
	ICRelease( m_ICHandle );
}


BOOL CMsgTable::AddMsg( TESTMSG* pMsg, DWORD* pdwKeyOut )
{
	if( !pMsg )			return FALSE;

	MSG_CHAT* pMsgChat	= m_mpMsg.Alloc();
	if( !pMsgChat )		return FALSE;

	pMsgChat->Category		= pMsg->Category;
	pMsgChat->Protocol		= pMsg->Protocol;
	pMsgChat->dwObjectID	= pMsg->dwObjectID;
	SafeStrCpy( pMsgChat->Msg, pMsg->Msg, MAX_CHAT_LENGTH + 1 );
	pMsgChat->Name[0] = 0;

	DWORD dwKey = ICAllocIndex( m_ICHandle );

	if( m_htMsg.Add( pMsgChat, dwKey ) )
	{
		*pdwKeyOut = dwKey;
		return TRUE;
	}
	else
	{
		ICFreeIndex( m_ICHandle, dwKey );
		m_mpMsg.Free( pMsgChat );
		return FALSE;
	}
}


BOOL CMsgTable::AddMsg( MSG_CHAT* pMsg, DWORD* pdwKeyOut )
{
	if( !pMsg )			return FALSE;

	MSG_CHAT* pMsgChat	= m_mpMsg.Alloc();
	if( !pMsgChat )		return FALSE;

//	*pMsgChat = *pMsg;
	pMsgChat->Category		= pMsg->Category;
	pMsgChat->Protocol		= pMsg->Protocol;
	pMsgChat->dwObjectID	= pMsg->dwObjectID;
	SafeStrCpy( pMsgChat->Msg, pMsg->Msg, MAX_CHAT_LENGTH + 1 );
	SafeStrCpy( pMsgChat->Name, pMsg->Name, MAX_NAME_LENGTH + 1 );

	DWORD dwKey = ICAllocIndex( m_ICHandle );

	if( m_htMsg.Add( pMsgChat, dwKey ) )
	{
		*pdwKeyOut = dwKey;
		return TRUE;
	}
	else
	{
		ICFreeIndex( m_ICHandle, dwKey );
		m_mpMsg.Free( pMsgChat );
		return FALSE;
	}
}

void CMsgTable::RemoveMsg( DWORD dwKey )
{
	m_mpMsg.Free( m_htMsg.GetData( dwKey ) );
	m_htMsg.Remove( dwKey );
	ICFreeIndex( m_ICHandle, dwKey );
}





/*
void CMsgTable::AddWisperMsg(MSG_CHAT* pmc)
{
	m_WisperMsgTable.AddHead(pmc);
}
void CMsgTable::AddPartyMsg(MSG_CHAT* pmc)
{
	m_PartyMsgTable.AddHead(pmc);
}
void CMsgTable::AddMunpaMsg(MSG_CHAT* pmc)
{
	m_MunpaMsgTable.AddHead(pmc);
}

void CMsgTable::RemoveWisperMsg()
{
	m_WisperMsgTable.RemoveTail();
}
void CMsgTable::RemovePartyMsg()
{
	m_PartyMsgTable.RemoveTail();
}
void CMsgTable::RemoveMunpaMsg()
{
	m_MunpaMsgTable.RemoveTail();
}

MSG_CHAT* CMsgTable::GetWisperMsg()
{
	MSG_CHAT* pos = (MSG_CHAT*)m_WisperMsgTable.GetTail();
	return pos;
}
MSG_CHAT* CMsgTable::GetPartyMsg()
{
	MSG_CHAT* pos = (MSG_CHAT*)m_PartyMsgTable.GetTail();
	return pos;
}
MSG_CHAT* CMsgTable::GetMunpaMsg()
{
	MSG_CHAT* pos = (MSG_CHAT*)m_MunpaMsgTable.GetTail();
	return pos;
}
*/