// ServerConnect.h: interface for the CServerConnect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERCONNECT_H__32D22D75_F2D1_4C03_A002_232AFB60C22A__INCLUDED_)
#define AFX_SERVERCONNECT_H__32D22D75_F2D1_4C03_A002_232AFB60C22A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameState.h"

#define SERVERCONNECT USINGTON(CServerConnect)


class CServerConnect : public CGameState  
{
protected:
	
	DWORD	m_dwStartTime;
	BOOL	m_bShowingLogoAVI;
	HWND	m_hWndAVI;
public:
	//MAKESINGLETON(CServerConnect)

	CServerConnect();
	virtual ~CServerConnect();

	BOOL Init(void* pInitParam);
	void Release(CGameState* pNextGameState);

	void Process();
	void BeforeRender();
	void AfterRender();
	
	void NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg);

	void EndShowLogoAVI();
	void StartShowLogoAVI();

};
EXTERNGLOBALTON(CServerConnect);
#endif // !defined(AFX_SERVERCONNECT_H__32D22D75_F2D1_4C03_A002_232AFB60C22A__INCLUDED_)
