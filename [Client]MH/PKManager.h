// PKManager.h: interface for the CPKManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PKMANAGER_H__0681C2A4_4B93_460A_A1F3_2655FAFD3396__INCLUDED_)
#define AFX_PKMANAGER_H__0681C2A4_4B93_460A_A1F3_2655FAFD3396__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PKMGR USINGTON(CPKManager)

#include "IndexGenerator.h"

class CPlayer;
class CPKLootingDialog;

class CPKManager  
{
protected:
	BOOL	m_bSyncing;
	DWORD	m_dwPKModeStartTime;
	DWORD	m_dwPKContinueTime;
	
	BOOL	m_bPKAllow;
	
//---·çÆÃ´çÇÔ
	BOOL	m_bLooted;
//---·çÆÃÁß
	BOOL	m_bLooting;

//---------------
//ÀÎÅÍÆäÀÌ½º °ü·Ã
	CYHHashTable<CPKLootingDialog>	m_htLootingDlg;
	CIndexGenerator					m_IndexGenerator;	

public:
	CPKManager();
	virtual ~CPKManager();

	void	Init();
	void	Release();
	BOOL	HEROPKModeOn();
	BOOL	HEROPKModeOff();
	BOOL	ToggleHeroPKMode();
	void	SendMsgPKOn();
	
	void	SetPlayerPKMode( CPlayer* pPlayer, BOOL bPKMode );
	void	SetPKStartTimeReset()				{ m_dwPKModeStartTime = gCurTime; }
	void	AddPKContinueTime( DWORD dwTime )	{ m_dwPKContinueTime += dwTime; }

	void	SetPKAllow( BOOL bAllow )	{ m_bPKAllow = bAllow; }
	BOOL	IsPKAllow()					{ return m_bPKAllow; }
//parsing
	void NetworkMsgParse( BYTE Protocol, void* pMsg );

//---·çÆÃ´çÇÔ
	void SetPKLooted( BOOL bLooted )	{ m_bLooted = bLooted; }
	BOOL IsPKLooted()					{ return m_bLooted; }
//---·çÆÃÇÔ
	void SetPKLooting( BOOL bLooting )	{ m_bLooting = bLooting; }
	BOOL IsPKLooting()					{ return m_bLooting; }
	
//---------------
//ÀÎÅÍÆäÀÌ½º °ü·Ã
	DWORD AllocWindowId()	{ return m_IndexGenerator.GenerateIndex(); }
	void FreeWindowId( DWORD dwID ) { m_IndexGenerator.ReleaseIndex( dwID ); }

	CPKLootingDialog* CreateLootingDlg( DWORD dwDiePlayerIdx );
	void RemoveAllLootingDlg();
	void PositioningDlg( LONG& rX, LONG& rY );
	void CloseLootingDialog( CPKLootingDialog* pLootingDlg, BOOL bMsgToServer );
	
	int GetLootingChance( DWORD dwBadFame );
	int GetLootingItemNum( DWORD dwBadFame );
};

EXTERNGLOBALTON(CPKManager)

#endif // !defined(AFX_PKMANAGER_H__0681C2A4_4B93_460A_A1F3_2655FAFD3396__INCLUDED_)
