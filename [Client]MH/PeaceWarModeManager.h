// PeaceWarModeManager.h: interface for the CPeaceWarModeManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PEACEWARMODEMANAGER_H__02CB9F63_8503_4314_8262_A13EA55B84A7__INCLUDED_)
#define AFX_PEACEWARMODEMANAGER_H__02CB9F63_8503_4314_8262_A13EA55B84A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PEACEWARMGR USINGTON(CPeaceWarModeManager)

class CPlayer;

class CPeaceWarModeManager  
{
	
	CHARACTER_TOTALINFO m_CharacterInfo;
	
	//SW050810 평화모드 자동변환
#define MODEVALIDTIME	10000
	DWORD	m_dwBeginCheckTime;

public:
	CPeaceWarModeManager();
//MAKESINGLETON(CPeaceWarModeManager);

	virtual ~CPeaceWarModeManager();

	void InitPeace_WarMode(CPlayer* pObject);
	void ToggleHeroPeace_WarMode();
	void Peace_WarMode(CPlayer* pPlayer, BOOL bPeace);
	
	BOOL IsPeaceMode(CPlayer* pPlayer);

	void NetworkMsgParse(BYTE Protocol,void* pMsg);

	//SW050810 평화모드 자동변환
	void DoCheckForPeaceMode(CPlayer* pPlayer);
	void SetCheckTime(DWORD Curtime) {m_dwBeginCheckTime = Curtime;}
	
};
EXTERNGLOBALTON(CPeaceWarModeManager);
#endif // !defined(AFX_PEACEWARMODEMANAGER_H__02CB9F63_8503_4314_8262_A13EA55B84A7__INCLUDED_)
