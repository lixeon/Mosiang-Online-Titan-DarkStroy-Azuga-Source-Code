// MapChange.h: interface for the CMapChange class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPCHANGE_H__B637E92F_3207_4239_B7C5_5D51A437D342__INCLUDED_)
#define AFX_MAPCHANGE_H__B637E92F_3207_4239_B7C5_5D51A437D342__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameState.h"

class CMapChange : public CGameState 
{
public:
	CMapChange();
	virtual ~CMapChange();

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

#endif // !defined(AFX_MAPCHANGE_H__B637E92F_3207_4239_B7C5_5D51A437D342__INCLUDED_)
