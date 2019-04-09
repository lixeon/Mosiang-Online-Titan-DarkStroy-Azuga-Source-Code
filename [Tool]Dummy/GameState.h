// GameState.h: interface for the CGameState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMESTATE_H__CAF4FFC5_E913_4BEE_943F_70390829CBE9__INCLUDED_)
#define AFX_GAMESTATE_H__CAF4FFC5_E913_4BEE_943F_70390829CBE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DummyNetwork.h"
#include "DummyObjMng.h"

class CStateMng;

class CGameState 
{
public:
	int				m_nID;
	CStateMng*		m_pStateMng;
	CDummyNetwork*	m_pNetwork;	
	CDummyObjMng*	m_pDummyObjMgr;
	
public:
	CGameState();
	virtual ~CGameState();

	virtual BOOL	Init( int id, CStateMng* pMgr, CDummyNetwork* pNet, CDummyObjMng* pDOMgr )
	{
		m_nID = id;
		m_pStateMng = pMgr;
		m_pNetwork = pNet;
		m_pDummyObjMgr = pDOMgr;
		return TRUE;
	}
	virtual void	Open( void* pInitParam )	{}
	virtual void	Close()	{}
	virtual void	Exit()	{}
	virtual void	Process()	{}
	virtual void	NetworkMsgParse( BYTE Category, BYTE Protocol, void* pMsg )	{}

	virtual void	OnDisconnect()			{}
	virtual void	OnConnectSuccessed()	{}
	virtual void	OnConnectFailed()		{}
	
	void			CommonNetworkMsgParse( BYTE Category, BYTE Protocol, void* pMsg );
};

#endif // !defined(AFX_GAMESTATE_H__CAF4FFC5_E913_4BEE_943F_70390829CBE9__INCLUDED_)
