#pragma once


class CPlayer;

struct GAMEEVEN_LEVELUP
{
	BOOL	bOn;
	DWORD	dwItemIdx;
	int		nTotalLevelCount;
	WORD*	pLevel;

	GAMEEVEN_LEVELUP()	{ bOn = FALSE; dwItemIdx = 0; nTotalLevelCount = 0; }
	~GAMEEVEN_LEVELUP()	{ if( pLevel ) { delete [] pLevel; } }
};

//EventKind	On(1)/Off(0)	ItemIdx(º¸»ó)	TotalLevelCount	Level	Level	Level	Level

#define GAMEEVENTMGR	USINGTON(CGameEventManager)

class CGameEventManager
{
protected:
	GAMEEVEN_LEVELUP	m_LevelUpEventInfo;

public:
	CGameEventManager(void);
	~CGameEventManager(void);

	void Init();
	void Release();

	BOOL LoadGameEventInfo();
	void ProcessLevelUpEvent( CPlayer* pPlayer, WORD wMaxLevel );

	BOOL EventOnOff( DWORD dwItemIdx, BOOL bOnOff );
};

EXTERNGLOBALTON(CGameEventManager)
