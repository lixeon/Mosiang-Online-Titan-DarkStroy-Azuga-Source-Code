// GameEnd.h: interface for the CGameEnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEEND_H__B47661FA_B739_4718_B8F2_B483CE212972__INCLUDED_)
#define AFX_GAMEEND_H__B47661FA_B739_4718_B8F2_B483CE212972__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameState.h"

class CGameEnd : public CGameState 
{
public:
	CGameEnd();
	virtual ~CGameEnd();

	virtual BOOL	Init( int id, CStateMng* pMgr, CDummyNetwork* pNet, CDummyObjMng* pDOMgr );
	virtual void	Open( void* pInitParam );
	virtual void	Close();
	virtual void	Exit();
	virtual void	Process();
	virtual void	NetworkMsgParse( BYTE Category, BYTE Protocol, void* pMsg );

	virtual void	OnDisconnect();
	virtual void	OnConnectSuccessed()	{}
	virtual void	OnConnectFailed()		{}


	DWORD			m_dwConnectTime;

};

#endif // !defined(AFX_GAMEEND_H__B47661FA_B739_4718_B8F2_B483CE212972__INCLUDED_)
