// MainTitle.h: interface for the CMainTitle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINTITLE_H__4F0019BE_E65A_4AEE_8408_B174F4A9BF56__INCLUDED_)
#define AFX_MAINTITLE_H__4F0019BE_E65A_4AEE_8408_B174F4A9BF56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameState.h"

enum
{
	PL_None = 0,
	PL_ConDist,
	PL_ReceiveDCS,
	PL_SendLoginSyn,
	PL_ReceiveLoginAck,
};
class CMainTitle : public CGameState 
{
protected:
	BOOL	m_bDisconnectToDist;
	BOOL	m_bConnectingToAgent;
	BOOL	m_bConnectingToDist;

	DWORD	m_dwDistAuthKey;
	
	BOOL	m_bLoginAck;

	DWORD	m_ProcLevel;
	
	BOOL	m_bLoginTime;

public:
	CMainTitle();
	virtual ~CMainTitle();

	virtual BOOL	Init( int id, CStateMng* pMgr, CDummyNetwork* pNet, CDummyObjMng* pDOMgr );
	virtual void	Open( void* pInitParam );
	virtual void	Close();
	virtual void	Exit();
	virtual void	Process();
	virtual void	NetworkMsgParse( BYTE Category, BYTE Protocol, void* pMsg );
	
	virtual void	OnDisconnect();
	virtual void	OnConnectFailed();
	virtual void	OnConnectSuccessed();

	void			LoginError( int code );
	void			SendIDPass();

	DWORD			m_dwLogin;

	BOOL			m_bOverLogin;
	BOOL			m_bEnterAgent;
};

#endif // !defined(AFX_MAINTITLE_H__4F0019BE_E65A_4AEE_8408_B174F4A9BF56__INCLUDED_)
