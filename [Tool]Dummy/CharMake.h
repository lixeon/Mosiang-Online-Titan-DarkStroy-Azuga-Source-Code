// CharMake.h: interface for the CCharMake class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARMAKE_H__BD463053_B1A0_4670_ADE1_50CE7C3B72DF__INCLUDED_)
#define AFX_CHARMAKE_H__BD463053_B1A0_4670_ADE1_50CE7C3B72DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameState.h"

class CCharMake : public CGameState 
{
public:
	CCharMake();
	virtual ~CCharMake();

	virtual BOOL	Init( int id, CStateMng* pMgr, CDummyNetwork* pNet, CDummyObjMng* pDOMgr );
	virtual void	Open( void* pInitParam );
	virtual void	Close();
	virtual void	Exit();
	virtual void	Process();
	virtual void	NetworkMsgParse( BYTE Category, BYTE Protocol, void* pMsg );

	virtual void	OnDisconnect();
	virtual void	OnConnectSuccessed()	{}
	virtual void	OnConnectFailed()		{}
};

#endif // !defined(AFX_CHARMAKE_H__BD463053_B1A0_4670_ADE1_50CE7C3B72DF__INCLUDED_)
