// MHNetwork.h: interface for the CMHNetwork class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MHNETWORK_H__E93C75BD_70FA_4330_B63C_2B5A6A57132A__INCLUDED_)
#define AFX_MHNETWORK_H__E93C75BD_70FA_4330_B63C_2B5A6A57132A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "basenetworkinterface.h"

#define WM_SOCKEVENTMSG WM_USER+155

#define NETWORK USINGTON(CMHNetwork)

class CMHNetwork  
{
protected:
	ISC_BaseNetwork* m_pBaseNetwork;
	DWORD	m_dwConnectionIndex[MAX_SERVERSET];
	BYTE	m_CheckSum[MAX_SERVERSET];

public:
	HWND	m_hMainWnd;

public:
	CMHNetwork();
	virtual ~CMHNetwork();
	
	BOOL	Init(HWND hWnd);
	void	Release();

	BOOL	ConnectToServer( int serverset, char* ip, WORD port );
	void	Disconnect( int serverset );
	void	AllDisconnect();
	void	Send( int serverset, MSGROOT* pMsg, int MsgLen );
	void	SendAll( MSGROOT* pMsg, int MsgLen );

	ISC_BaseNetwork* GetBaseNetwork()		{ return m_pBaseNetwork; }
	friend void OnCon( DWORD dwConIndex );
	friend void OnDisCon( DWORD dwConIndex );
	friend void OnRecv( DWORD dwConIndex, char* pMsg, DWORD msglen);
};

EXTERNGLOBALTON(CMHNetwork)


#endif // !defined(AFX_MHNETWORK_H__E93C75BD_70FA_4330_B63C_2B5A6A57132A__INCLUDED_)
