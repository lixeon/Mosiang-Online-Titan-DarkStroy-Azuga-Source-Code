// CharSelect.h: interface for the CCharSelect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARSELECT_H__A26276F4_E7FA_4362_B9A3_81AEFCA7DFC7__INCLUDED_)
#define AFX_CHARSELECT_H__A26276F4_E7FA_4362_B9A3_81AEFCA7DFC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameState.h"

class CCharSelect : public CGameState 
{
public:
	CCharSelect();
	virtual ~CCharSelect();

	virtual BOOL	Init( int id, CStateMng* pMgr, CDummyNetwork* pNet, CDummyObjMng* pDOMgr );
	virtual void	Open(  void* pInitParam );
	virtual void	Close();
	virtual void	Exit();
	virtual void	Process();
	virtual void	NetworkMsgParse( BYTE Category, BYTE Protocol, void* pMsg );

	virtual void	OnDisconnect();
	virtual void	OnConnectSuccessed()	{}
	virtual void	OnConnectFailed()		{}
};

#endif // !defined(AFX_CHARSELECT_H__A26276F4_E7FA_4362_B9A3_81AEFCA7DFC7__INCLUDED_)
