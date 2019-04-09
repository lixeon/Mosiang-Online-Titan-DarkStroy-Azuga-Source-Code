// GameState.h: interface for the CGameState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMESTATE_H__A45C7DD6_6E8B_486F_81B8_9A18546FC927__INCLUDED_)
#define AFX_GAMESTATE_H__A45C7DD6_6E8B_486F_81B8_9A18546FC927__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGameState  
{
protected:
	BOOL	m_bBlockScreen;
	BOOL	m_bDisconnected;
public:
	CGameState();
	virtual ~CGameState();

	virtual BOOL Init(void* pInitParam) = 0;
	virtual void Release(CGameState* pNextGameState) = 0;		// NULL 이면 끝나는거임

	virtual void Process() = 0;
	virtual void BeforeRender() = 0;
	virtual void AfterRender() = 0;
	
	virtual void NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg) = 0;
	static	BOOL CommonNetworkParser(BYTE Category,BYTE Protocol,void* pMsg);

	void	SetBlock(BOOL val);
	BOOL	IsBlock(){ return m_bBlockScreen;	}
	
	virtual void OnDisconnect();
};

#endif // !defined(AFX_GAMESTATE_H__A45C7DD6_6E8B_486F_81B8_9A18546FC927__INCLUDED_)
