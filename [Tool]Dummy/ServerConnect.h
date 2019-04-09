// ServerConnect.h: interface for the CServerConnect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERCONNECT_H__1F1AD663_A67A_4A3F_B106_13E48D439F3E__INCLUDED_)
#define AFX_SERVERCONNECT_H__1F1AD663_A67A_4A3F_B106_13E48D439F3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameState.h"

// purpose : distribute server¿¡ Á¢¼Ó ÈÄ agent server ip ¾ò±â.
class CServerConnect : public CGameState 
{
public:
	CServerConnect();
	virtual ~CServerConnect();

	virtual BOOL	Init(  int id, CStateMng* pMgr, CDummyNetwork* pNet, CDummyObjMng* pDOMgr );
	virtual void	Open( void* pInitParam );
	virtual void	Close();
	virtual void	Exit();
	virtual void	Process();
	virtual void	NetworkMsgParse( BYTE Category, BYTE Protocol, void* pMsg );

	virtual void	OnDisconnect()			{}
	virtual void	OnConnectSuccessed()	{}
	virtual void	OnConnectFailed()		{}
	
};

#endif // !defined(AFX_SERVERCONNECT_H__1F1AD663_A67A_4A3F_B106_13E48D439F3E__INCLUDED_)
