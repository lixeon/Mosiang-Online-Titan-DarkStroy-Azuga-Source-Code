#pragma once


class CPunishUnit
{
protected:
	DWORD	m_dwUserIdx;
	int		m_nPunishKind;
	DWORD	m_dwEndTime;

public:
	CPunishUnit(void);
	~CPunishUnit(void);

	void Init( DWORD dwUserIdx, int nPunishKind, DWORD dwEndTime );
	BOOL IsTimeEnd();

	DWORD GetRemainTime();
	DWORD GetUserIdx() { return m_dwUserIdx; }
};


#define PUNISHMGR	USINGTON(CPunishManager)

class CPunishManager
{
protected:
	CYHHashTable< CPunishUnit >			m_htPunishUnit[ePunish_Max];
	CMemoryPoolTempl< CPunishUnit >*	m_pmpPunishUnit;

	DWORD	m_dwAutoNoteUseTime;	// Ка
	DWORD	m_dwAutoBlockTime;		// Ка

public:
	CPunishManager(void);
	~CPunishManager(void);

	void Init();
	void Release();
	void Process();

	void AddPunishUnit( DWORD dwUserIdx, int nPunishKind, DWORD dwPunishTime );
	void RemovePunishUnit( DWORD dwUserIdx, int nPunishKind );
	void RemovePunishUnitAll( DWORD dwUserIdx );
	CPunishUnit* GetPunishUnit( DWORD dwUserIdx, int nPunishKind );

	DWORD GetAutoNoteUseTime()				{ return m_dwAutoNoteUseTime; }
	void SetAutoNoteUseTime( DWORD dwTime )	{ m_dwAutoNoteUseTime = dwTime; }
	DWORD GetAutoBlockTime()				{ return m_dwAutoBlockTime; }
	void SetAutoBlockTime( DWORD dwTime )	{ m_dwAutoBlockTime = dwTime; }
};

EXTERNGLOBALTON(CPunishManager)