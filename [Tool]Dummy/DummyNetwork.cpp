// DummyNetwork.cpp: implementation of the CDummyNetwork class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DummyNetwork.h"
#include "GameState.h"
#include "Network.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern CYHHashTable<CDummyNetwork> g_DNTable;

CDummyNetwork::CDummyNetwork()
{
	m_ConnectionIdx = 0;
	m_CheckSum = 0;
	m_pState = NULL;
}

CDummyNetwork::~CDummyNetwork()
{
	g_DNTable.Remove(m_ConnectionIdx);
	Disconnect();
}

BOOL CDummyNetwork::ConnectToServer(char* ip,WORD port)
{
	return g_Network.ConnectToServer(ip,port,this);
}
void CDummyNetwork::Disconnect()
{
	if(m_ConnectionIdx)
	{
		g_Network.Disconnect(m_ConnectionIdx);
	}
}

void CDummyNetwork::SetCurState(CGameState* pState)
{
	m_pState = pState;
}
void CDummyNetwork::SetCheckSum(BYTE CheckSum)
{
	m_CheckSum = CheckSum;
}

void CDummyNetwork::Send(MSGBASE* pMsg,WORD len)
{
	if(pMsg->Category >= MP_MAX)
	{
		_asm int 3;
	}
	pMsg->CheckSum = m_CheckSum++;
	BOOL rt = g_Network.Send(m_ConnectionIdx,pMsg,len);
}

void CDummyNetwork::NetworkMsgParse( BYTE Category, BYTE Protocol, void* pMsg )
{
	m_pState->NetworkMsgParse(Category,Protocol,pMsg);
}

void CDummyNetwork::OnDisconnect()
{
	m_pState->OnDisconnect();
}

void CDummyNetwork::OnConnected(DWORD ConnectionIdx)
{
	m_ConnectionIdx = ConnectionIdx;
	m_pState->OnConnectSuccessed();
}
void CDummyNetwork::OnConnectFailed()
{
	m_pState->OnConnectFailed();
}