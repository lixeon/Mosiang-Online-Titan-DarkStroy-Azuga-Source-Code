// UserTable.h: interface for the CUserTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERTABLE_H__5BBB3509_10AF_4E68_9B3A_C07E9F23D2A9__INCLUDED_)
#define AFX_USERTABLE_H__5BBB3509_10AF_4E68_9B3A_C07E9F23D2A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//AAA--- Disconnect할때 해야할 일을 flag로 넘긴다.
enum eDISCONNECT_OPTION
{
	eDISCONOPT_NONE				= 0,
	eDISCONOPT_DELETELOGINCHECK	= 1,
};

BOOL DisconnectUser( DWORD dwConnectionIndex );
//------

typedef struct tagUSERINFO
{
	DWORD	dwConnectionIndex;
	DWORD	dwUserID;
	BYTE	UserLevel;
	WORD	wUserMapNum;
	DWORD	State;
	BOOL	bRecvDistOut;
	DWORD	dwUniqueConnectIdx;	//고유 연결 번호, //dwConnectionIdx와는 다름. 계속 증가함.
								
//--- 커넥션 체크용
	DWORD	dwLastConnectionCheckTime;
	BOOL	bConnectionCheckFailed;

//AAA
	DWORD	dwDisconOption;		//접속이 종료될 때 처리해야할 일 들
//---

} USERINFO;

class CUserTable : public CYHHashTable<USERINFO>
{
	DWORD m_MaxUserCount;	//서버툴에 설정된 MaxUser
	DWORD m_dwUserCount;	//현재 Distribute에 접속된 유저수

	DWORD m_addCount;		//총 접속된 유저수

public:

	CUserTable();
	virtual ~CUserTable();

	void SetPositionUserHead() {  CYHHashTable<USERINFO>::SetPositionHead(); }
	USERINFO * GetUserData() { return CYHHashTable<USERINFO>::GetData(); }

	void Init(DWORD dwBucket);
	USERINFO * FindUser(DWORD dwKey);
	BOOL AddUser(USERINFO* pObject,DWORD dwKey);
	USERINFO * RemoveUser(DWORD dwKey);
	void RemoveAllUser();

	DWORD GetUserCount() { return m_dwUserCount; }
	DWORD GetUserMaxCount() {return m_MaxUserCount;}

	BOOL SendToUser(DWORD dwKey,DWORD dwUniqueConnectIdx,MSGBASE* pmsg,DWORD size);
	BOOL OnDisconnectUser(DWORD dwKey);

	DWORD GetAddCount() { return m_addCount; }
};  


extern CUserTable * g_pUserTable;							// key : connectionidx

struct sLOGINUSER
{
	DWORD dwTime;
	DWORD dwCount;
};

class CLoginUser
{
	CYHHashTable<sLOGINUSER>	m_User;

public:
	CLoginUser();
	virtual ~CLoginUser();
/*
	void	SetCount( DWORD dwUserIdx, WORD wNum );
	void	Receive( DWORD dwUserIdx );
	int		Check( DWORD dwUserIdx );
*/
	void	Remove( DWORD dwUserIdx );
	void	AddWaitForLoginCheckDelete( DWORD dwUserIdx );

};

extern CLoginUser g_LoginUser;


#endif // !defined(AFX_USERTABLE_H__5BBB3509_10AF_4E68_9B3A_C07E9F23D2A9__INCLUDED_)

