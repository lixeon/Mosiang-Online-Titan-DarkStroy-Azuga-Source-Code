#pragma once

#define BOBUSANGMGR	USINGTON(BobusangManager_Agent)

#define BOBUSANG_POSNUM_MAX	4	// 맵별 출현 최대 위치갯수
#define DEALITEM_BIN_TABNUM	7	// deallist.bin 상의 탭 갯수 // 보부상은 이중 한개만 랜덤하게 판매한다.

#define BOBUSANG_CHECKTIME	60000	// 1분 간격 확인

struct BobusangPosPerMap
{
	BobusangPosPerMap() {memset(this, 0, sizeof(this));};
	VECTOR3	ApprPos;
	float	ApprDir;
};

struct BOBUSANG_MAPINFO
{
	BOBUSANG_MAPINFO() : dataIdx(0), mapNum(0) {};
	DWORD dataIdx;
	DWORD mapNum;
	BobusangPosPerMap Pos[BOBUSANG_POSNUM_MAX];
};

enum bobusangInfotime{ eBBSIT_CUR, eBBSIT_NEXT, eBBSIT_KINDNUM, };
enum bobusangAppearedState{
	eBBSAS_NONE = -1,
	eBBSAS_DISAPPEAR			= 0,	// 잠적 상태
	eBBSAS_APPEAR				= 2,	// 출현 상태
	eBBSAS_APPEAR_DELAYED		= 4,	// 출현 메시지에 맵 응답 없음
	eBBSAS_DISAPPEAR_DELAYED	= 8,	// 잠적 메시지에 맵 응답 없음
	eBBSAS_TIME_DELAYED			= 16,	// 시간 체크 초과
};

class BobusangManager_Agent
{
	BOOL				m_bManager;
	BOOL				m_bOnProcessing;		// OnOff

	DWORD				m_dwBobusangCheckTime;	// Process 체크 interval
	int*				m_piAppearedState;		// 현재 소환 중인가	//bobusangAppearedState
	DWORD				m_nChannelTotalNum;		// 전체 채널 갯수
	DWORD				m_dwAppearTimeMin;		// 다음 출현까지 최소 시간
	DWORD				m_dwAppearTimeMax;		// 다음 출현까지 최대 시간
	DWORD				m_dwDurationTimeMin;	// 출현 유지 최소 시간
	DWORD				m_dwDurationTimeMax;	// 출현 유지 최대 시간

	//BobusangPosPerMap	m_PosPerMap[MAX_MAP_ENUMCOUNT][BOBUSANG_POSNUM_MAX];	// 맵별 생성 위치
	CYHHashTable<BOBUSANG_MAPINFO>	m_mapInfo;

	BOBUSANGINFO*		m_pBobusangInfo;

public:
	BobusangManager_Agent(void);
	~BobusangManager_Agent(void);

	void	CheckManager();
	void	SetManager(BOOL bVal)
	{
		m_bManager = bVal;
	}

	void	BobusangMgr_Start();
	void	BobusangMgr_Init();
	void	BobusangMgr_Process();		// check time to Appear & inform to map
	void	BobusangMgr_Release();
	BOOL	LoadChannelInfo();		// set serverset & channel info
	BOOL	LoadBobusangInfo();			// regentime interval & etc.
	void	InitBobusangInfo();
	void	SetBobusangInfo(DWORD iChannel);			// set struct BOBUSANGINFO(...)	//eBBSIT_NEXT
	void	SendBobusangInfoMsgToMap(BOBUSANGINFO* pInfo);	// send bobusangInfo for map
	void	SendBobusangLeaveMsgToMap(BOBUSANGINFO* pInfo);	// bobusang remove from map
	void	SendBobusangLeaveNotifyToMap(BOBUSANGINFO* pInfo);

	void	SetChannelState(DWORD iChannel, int bobusangAppearedState);

	//for cheat
	void	DeveloperRequest(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

	BOOL	SetProcessing(BOOL bVal);

	stTIME*	SetstTIMEfromMinValue(DWORD min);
};

EXTERNGLOBALTON(BobusangManager_Agent)