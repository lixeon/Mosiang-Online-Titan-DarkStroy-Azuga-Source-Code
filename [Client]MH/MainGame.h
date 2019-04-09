// MainGame.h: interface for the CMainGame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINGAME_H__7AEA4DB0_7847_40E1_8CDC_15C321ED0CB3__INCLUDED_)
#define AFX_MAINGAME_H__7AEA4DB0_7847_40E1_8CDC_15C321ED0CB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#define MAINGAME USINGTON(CMainGame)

class CGameState;
class CEngine;

enum eGAMESTATE
{
	eGAMESTATE_END = 0,
	eGAMESTATE_INTRO,	// magi82 - Intro(070712)
	eGAMESTATE_CONNECT,
	eGAMESTATE_TITLE,
	eGAMESTATE_CHARSELECT,
	eGAMESTATE_CHARMAKE,
	eGAMESTATE_GAMELOADING,
	eGAMESTATE_GAMEIN,
	eGAMESTATE_MAPCHANGE,
	eGAMESTATE_MURIMNET,
};

class CMainGame
{
	BOOL m_bEndGame;
	CEngine* m_pEngine;

	CGameState** m_ppGameState;
	BOOL m_bChangeState;
	int m_ChangeState;
	CGameState* m_pCurrentGameState;

	CGameState* GetGameState(int StateNum);

	char* m_pStateInitParam;
	
//KES 030827 Ãß°¡
	int m_nCurStateNum;
	
	int m_nUserLevel;

public:
	CGameState* GetCurGameState() { return m_pCurrentGameState;	}
	void EndGame()	{	m_bEndGame = TRUE;	}
	CMainGame();
	virtual ~CMainGame();

	void Init(HWND hMainWnd);
	void Release();

	void SetGameState(int StateNum,void* pStateInitParam = 0,int ParamLen = 0);
	BOOL IsChangeState() { return m_bChangeState; }

	int Process();
	void BeforeRender();
	void AfterRender();
	
	CEngine* GetEngine()	{	return m_pEngine;	}

//KES 030827 Ãß°¡
	int GetCurStateNum()	{ return m_nCurStateNum; }
	int GetUserLevel()		{ return m_nUserLevel; }
	void SetUserLevel( int nUserLevel ) { m_nUserLevel = nUserLevel; }
	
	void PauseRender( BOOL bPause );
};

EXTERNGLOBALTON(CMainGame);

DWORD __stdcall MapLoadCallback(DWORD dwCurCount,DWORD dwTotalCount,void* pArg);
DWORD _stdcall BeforeRenderFunc();
DWORD _stdcall AfterRenderFunc();


#ifndef _RESOURCE_WORK_NOT_APPLY_

void ResourceWork( void *pVoid );

#endif

#endif // !defined(AFX_MAINGAME_H__7AEA4DB0_7847_40E1_8CDC_15C321ED0CB3__INCLUDED_)
