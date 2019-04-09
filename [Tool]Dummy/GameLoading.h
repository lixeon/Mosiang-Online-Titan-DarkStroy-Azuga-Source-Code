// GameLoading.h: interface for the CGameLoading class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMELOADING_H__4C08CF5D_27F2_4826_A4E8_B3EF7E5600B4__INCLUDED_)
#define AFX_GAMELOADING_H__4C08CF5D_27F2_4826_A4E8_B3EF7E5600B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameState.h"

class CGameLoading : public CGameState 
{
public:
	MSGBASE			m_Msg;
	
public:
	CGameLoading();
	virtual ~CGameLoading();

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

#endif // !defined(AFX_GAMELOADING_H__4C08CF5D_27F2_4826_A4E8_B3EF7E5600B4__INCLUDED_)
