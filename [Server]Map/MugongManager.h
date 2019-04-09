// MugongManager.h: interface for the CMugongManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MUGONGMANAGER_H__12870C9F_2E96_4A52_9AF3_B4C8FA0A27DE__INCLUDED_)
#define AFX_MUGONGMANAGER_H__12870C9F_2E96_4A52_9AF3_B4C8FA0A27DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPlayer;

#define MUGONGMNGR CMugongManager::GetInstance()

class CMugongManager  
{
	CMugongManager();
public:
	GETINSTANCE(CMugongManager);
	virtual ~CMugongManager();

	BOOL RemMugong(CPlayer * pPlayer, WORD wMugongIdx, POSTYPE TargetPos, BYTE bType);
	BOOL MoveMugong(CPlayer * pPlayer, POSTYPE FromPos, POSTYPE ToPos);
	BOOL AddMugong( CPlayer * pPlayer, WORD wMuongIdx , WORD ItemKind, POSTYPE QuickPos = 0, BYTE bSung = 1, WORD Option = 0 );
	// magi82 - Titan(070611) 타이탄 무공변환 주석처리
	// BOOL AddTitanMugong( CPlayer * pPlayer, WORD wMuongIdx, POSTYPE QuickPos = 0, BYTE bSung = 0, WORD Option = 0 );
	// MUGONGBASE* GetTitanMugongBase( CPlayer * pPlayer, WORD idx );	// magi82 - Titan(070319)

	void AddMugongDBResult(CPlayer * pPlayer, MUGONGBASE * pMugongBase);

	void ChangeMugong(CPlayer* pPlayer, WORD wRemMugongIdx, POSTYPE TargetPos, WORD wAddMugongIdx, WORD LogType);
	
	void NetworkMsgParse( BYTE Protocol, void* pMsg );
	
#ifdef _JAPAN_LOCAL_
	BOOL	DeleteSkill( CPlayer* pPlayer, WORD wSkillIdx );
#endif

	BOOL SetOption(CPlayer * pPlayer, WORD wMugongIdx, POSTYPE TargetPos, WORD ItemIdx, WORD ItemPos);
	BOOL ClearOption(CPlayer * pPlayer, WORD wMugongIdx, POSTYPE TargetPos, WORD ItemIdx, WORD ItemPos);
};

#endif // !defined(AFX_MUGONGMANAGER_H__12870C9F_2E96_4A52_9AF3_B4C8FA0A27DE__INCLUDED_)
