// BossState.h: interface for the CBossState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOSSSTATE_H__A7B88BEB_370E_4A60_9359_96754778C82C__INCLUDED_)
#define AFX_BOSSSTATE_H__A7B88BEB_370E_4A60_9359_96754778C82C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBossState  
{
	int m_CurState;
	int m_NextState;
	DWORD m_StateStartTime;
	DWORD m_StateEndTime;
	BOOL m_bStateUpdate;
	
	BOOL m_bStateFirst;


public:
	CBossState();
	virtual ~CBossState();

	void Init();
	
	void SetState(int State);
	void SetNextState(int state);

	int GetCurState();
	int GetNextState();
	
	DWORD GetCurStateEndTime();
	BOOL IsStateUpdated();
	void UpdateState();

	void SetStateStartTime(DWORD time);
	DWORD GetStateStartTime();

	void SetStateEndTime(DWORD time);
	DWORD GetStateEndTime();

	void SetStateFirst(BOOL vals);
	BOOL IsStateFirst();	
};

#endif // !defined(AFX_BOSSSTATE_H__A7B88BEB_370E_4A60_9359_96754778C82C__INCLUDED_)
