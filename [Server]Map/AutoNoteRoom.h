#pragma once


enum eAutoNoteRoomState
{
	eANRS_NONE,
	eANRS_ASKING,
	eANRS_WAITANSWER,
	eANRS_FASTANSWER,
	eANRS_ENDANSWER,
	eANRS_MAX,
};

class CAutoNoteRoom
{
protected:
	DWORD	m_dwAskCharacter;
	DWORD	m_dwAutoCharacter;

	DWORD	m_dwAskUserIdx;
	DWORD	m_dwAutoUserIdx;

	char	m_AskCharacterName[MAX_NAME_LENGTH+1];
	char	m_AutoCharacterName[MAX_NAME_LENGTH+1];

	int		m_nANRState;			//---Room 상태
	DWORD	m_dwCreateTime;			//---방 생성 시간
	int		m_nChance;

	//---------------------------------------
	DWORD	m_dwAskColor[4];

	DWORD	m_dwLastAnswerTime;		// 마지막 답변보낸시간...

public:
	CAutoNoteRoom(void);
	~CAutoNoteRoom(void);

	DWORD GetAskCharacterIdx()	{ return m_dwAskCharacter; }
	DWORD GetAutoCharacterIdx()	{ return m_dwAutoCharacter; }
	DWORD GetAskUserIdx()	{ return m_dwAskUserIdx; }
	DWORD GetAutoUserIdx()	{ return m_dwAutoUserIdx; }
	char* GetAskCharacterName()	{ return m_AskCharacterName; }
	char* GetAutoCharacterName()	{ return m_AutoCharacterName; }

	void CreateRoom( CPlayer* pAskPlayer, CPlayer* pAutoPlayer, DWORD dwQuestion );
	void AskToAuto();
	BOOL CheckAnswerFromAuto( DWORD dwData1, DWORD dwData2, DWORD dwData3, DWORD dwData4 );

	BOOL IsTimeOver();
	int	GetChance()	{ return m_nChance; }

	void SetAutoNoteRoomState( int nANRState ) { m_nANRState = nANRState; }
	int GetAutoNoteRoomState() { return m_nANRState; }

	DWORD GetAnswerTime() { return gCurTime - m_dwCreateTime; }
	DWORD GetLastAnswerTime()	{ return gCurTime - m_dwLastAnswerTime; }
	void FastAnswer();
};
