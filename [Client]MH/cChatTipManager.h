// cChatTipManager.h: interface for the cChatTipManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCHATTIPMANAGER_H__CE1E7806_E594_4A01_8604_AF67682DE85C__INCLUDED_)
#define AFX_CCHATTIPMANAGER_H__CE1E7806_E594_4A01_8604_AF67682DE85C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CHATTIPMGR	USINGTON(cChatTipManager)

//#define	CHATTIPMSG_TIMELEN	180000


struct sChatTip
{
	DWORD	dwIndex;		//메시지 인덱스
	DWORD	FromLv;			//레벨 비교 인자
	DWORD	ToLv;			//레벨 비교 인자
	char*	pTipStr;		//MSG

	sChatTip():dwIndex(0),FromLv(0),ToLv(0),pTipStr(NULL)
	{}
};

class cChatTipManager  
{
protected:

	BOOL	m_bActive;			//더이상 유효 메시지가 없을때 동작그만!
	DWORD	m_dwSize;
	DWORD	m_dwMsgTimeLen;
	DWORD	m_dwLastMsgTime;	//먼저 메시지 출력 시각
	DWORD	m_dwCurHeroLevel;	//HEROINFO 구조체 대체
	int		m_nCurIndex;		//다음 메시지 전환을 위한 현재 인덱스 기억

	sChatTip*	m_spChatTipMsg;	//MSG pool

public:
	void SetActive( BOOL bActive ) { m_bActive = bActive; };
	void SetCurHeroInfo(DWORD Lv) { m_dwCurHeroLevel = Lv; };//임시 테스트용
//	char* GetStringInQuotation(char* buf);	//임시
	void Init();
	void Release();
	void Process();			
	void LoadChatTipMsg();	//메시지 로드
	char* GetChatTipMsg();	//레벨과 시간 조건 맞을시 메시지 반환
	bool IsTimePassed();	//유효시간 되었는가

	void ReleaseMsgList();	//m_spChatTipMsg pTipStr 해제

	cChatTipManager();
	virtual ~cChatTipManager();

};

EXTERNGLOBALTON(cChatTipManager)
#endif // !defined(AFX_CCHATTIPMANAGER_H__CE1E7806_E594_4A01_8604_AF67682DE85C__INCLUDED_)
