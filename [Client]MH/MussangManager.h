// MussangManager.h: interface for the CMussangManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MUSSANGMANAGER_H__4F0BBCDB_B978_4804_A05F_F72B21DF285F__INCLUDED_)
#define AFX_MUSSANGMANAGER_H__4F0BBCDB_B978_4804_A05F_F72B21DF285F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MUSSANG_VALID_TIME		1000*60*10


#define MUSSANGMGR	USINGTON(CMussangManager)

enum eMussangStates{ eMSS_None=1, eMSS_Ready=2, eMSS_InMode=4, eMSS_LevelUp=8};


class CMussangManager  
{
	DWORD				m_dwMussangPoint;
	DWORD				m_dwMaxMussangPoint;
//	DWORD				m_dwMussangStartTime;
	BOOL				m_bIsMussangMode;
	BOOL				m_bIsMussangReady;
	BOOL				m_bValidBTNforSendMsg;	//메시지 관련 버튼은 disable 을 사용해야한다. 무쌍은 단축키 때문에 따로 변수로 제어한다.

	// 06. 03. 국내무쌍 - 이영준
#ifndef _JAPAN_LOCAL_
	BOOL				m_bMussang;
	WORD				m_wMussangStat;
#endif

public:
	CMussangManager();
	virtual ~CMussangManager();

	void Init();
	void SetMussangPoint(DWORD dwMussangPoint);
//	void SetMussangMaxPointFromLevel(DWORD dwLevel);	//레벨로부터 해당 공식에 의해 최대값 세팅.
	void SetMussangMaxPoint(DWORD dwPoint) { m_dwMaxMussangPoint = dwPoint;}
	void SetMussangReady(BOOL bVal);
	void SetMussangMode(BOOL bVal, DWORD time = 0);
	BOOL IsMussangReady() {return m_bIsMussangReady;}
	void SendMsgMussangOn();

	void SetValidMsg(BOOL bValid) {m_bValidBTNforSendMsg = bValid;}

	// 06. 03. 국내무쌍 - 이영준
#ifndef _JAPAN_LOCAL_
	BOOL IsMussangMode() { return m_bMussang; }
	WORD GetMussangStat() { return m_wMussangStat; }
#endif
};

//서버로 부터 게이지 값을 받는다.
//게이지 풀이 되면 게이지바 깜박이고 무쌍 버튼 활성화 한다.
//단축 버튼이나 버튼이 클릭되면 서버로 무쌍 발동 정보 보낸다.
//발동 승인 정보 받아서 시작이펙트(지역), 지속이펙트(지역), 상태 아이콘(자신) 을 보여준다.
//무쌍모드 종료 정보 받아서 이펙트 처리해준다.

EXTERNGLOBALTON(CMussangManager)

#endif // !defined(AFX_MUSSANGMANAGER_H__4F0BBCDB_B978_4804_A05F_F72B21DF285F__INCLUDED_)
