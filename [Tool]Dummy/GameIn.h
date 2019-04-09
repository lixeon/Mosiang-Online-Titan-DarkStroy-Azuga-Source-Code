// GameIn.h: interface for the CGameIn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEIN_H__0B877BB7_B8E0_4BE8_928A_A376F608BFAB__INCLUDED_)
#define AFX_GAMEIN_H__0B877BB7_B8E0_4BE8_928A_A376F608BFAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameState.h"

class CGameIn : public CGameState 
{
protected:
	BOOL			m_bGameIn;

public:
	CGameIn();
	virtual ~CGameIn();

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

#endif // !defined(AFX_GAMEIN_H__0B877BB7_B8E0_4BE8_928A_A376F608BFAB__INCLUDED_)
