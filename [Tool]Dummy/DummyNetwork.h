// DummyNetwork.h: interface for the CDummyNetwork class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DUMMYNETWORK_H__A5821145_39CF_44A6_B49F_7977D6EB4132__INCLUDED_)
#define AFX_DUMMYNETWORK_H__A5821145_39CF_44A6_B49F_7977D6EB4132__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGameState;

class CDummyNetwork  
{
public:
	CGameState* m_pState;
	BYTE m_CheckSum;
	DWORD m_ConnectionIdx;

public:
	CDummyNetwork();
	virtual ~CDummyNetwork();

	BOOL ConnectToServer(char* ip,WORD port);
	void Disconnect();

	void SetCurState(CGameState* pState);
	void SetCheckSum(BYTE CheckSum);

	void Send(MSGBASE* pMsg,WORD len);

	void NetworkMsgParse( BYTE Category, BYTE Protocol, void* pMsg );
	void OnDisconnect();

	void OnConnected(DWORD ConnectionIdx);
	void OnConnectFailed();
};

#endif // !defined(AFX_DUMMYNETWORK_H__A5821145_39CF_44A6_B49F_7977D6EB4132__INCLUDED_)
