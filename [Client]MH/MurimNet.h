// MurimNet.h: interface for the CMurimNet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MURIMNET_H__E5818821_77D6_4918_937C_21B7B82D48D3__INCLUDED_)
#define AFX_MURIMNET_H__E5818821_77D6_4918_937C_21B7B82D48D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameState.h"

#define MURIMNET USINGTON(CMurimNet)


class CMNPlayerManager;
class CMNStage;

enum eMNSTAGE {

	MNSTG_FRONT,
	MNSTG_CHANNEL,
	MNSTG_PLAYROOM,

	MNSTG_COUNT,
};

extern char gStrTemp128[128];

class CMurimNet : public CGameState
{
protected:
	
	int			m_nCurMNStage;
	CMNStage*	m_apMNStage[MNSTG_COUNT];
	
	CMNPlayerManager*	m_pMNPlayerManager;

protected:

	void InitStages();
	void ReleaseStages();

public:
	CMurimNet();
	virtual ~CMurimNet();

	BOOL Init( void* pInitParam );		//---resource loading
	void Release(CGameState* pNextGameState);						//---resource release

	void Process();
	void BeforeRender();
	void AfterRender();
	
	void NetworkMsgParse( BYTE Category, BYTE Protocol, void* pMsg );

//
	void ChangeStage( int nStage );
	

///////
	void SendConnectMsg( DWORD dwBackMapNum );
	void SendReConnectMsg();
//Inline
	CMNStage*	GetMNStage( int nStage ) { return m_apMNStage[nStage]; }
	CMNStage*	GetCurMNStage() { return m_apMNStage[m_nCurMNStage]; }
	void		SetCurMNStage( int nMNStage )	{ m_nCurMNStage = nMNStage; }

	CMNPlayerManager* GetMNPlayerManager()	{ return m_pMNPlayerManager;	}

//임시임당.
//	void	SetHeroID( DWORD dwHeroID )		{ m_dwHeroID = dwHeroID; }
//	DWORD	GetHeroID()						{ return m_dwHeroID; }

};


EXTERNGLOBALTON(CMurimNet)

#endif // !defined(AFX_MURIMNET_H__E5818821_77D6_4918_937C_21B7B82D48D3__INCLUDED_)
