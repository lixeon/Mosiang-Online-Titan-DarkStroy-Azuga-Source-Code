// ServerSystem.h: interface for the CServerSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERSYSTEM_H__FD3EBFC3_EE3D_4505_A5A1_24DA471D20AB__INCLUDED_)
#define AFX_SERVERSYSTEM_H__FD3EBFC3_EE3D_4505_A5A1_24DA471D20AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SERVERLISTNAME "Agentserver_info.txt"
#define __AGENTSERVER__
#include "ServerTable.h"
#include "UserTable.h"
#include "..\4dyuchigxgfunc/global.h"

//--- max user&server define ------
#define MAX_USER_IN_A_AGENT		4000
#define MAX_SERVER_CONNECTION	100
//---------------------------------

//-----------------------------------------------------------------------
// 맵전환 좌표기억 구조체
//-----------------------------------------------------------------------
typedef struct _MAPCHANGE_INFO
{
	DWORD Kind;
	char CurMapName[MAX_NAME_LENGTH];
	char ObjectName[MAX_NAME_LENGTH];
	WORD CurMapNum;
	WORD MoveMapNum;
	VECTOR3 CurPoint;
	VECTOR3 MovePoint;
	WORD chx_num;
	
	// 방향설정
}MAPCHANGE_INFO;



enum eNATION
{
	eNATION_KOREA,
	eNATION_CHINA,
};


class CServerSystem  
{
	MAPCHANGE_INFO	m_MapChange[MAX_POINT_NUM];

//	INDEXCR_HANDLE	m_IdxCreater;
	WORD			m_wServerNum;
	
	BOOL			m_bUseCrypt;

	BOOL			m_bTestServer;
//국가
	int		m_Nation;

//이벤트공지용
	char	m_strEventNotifyTitle[32];
	char	m_strEventNotifyContext[128];
	BOOL	m_bUseEventNotify;
	BOOL	m_bEvenList[eEvent_Max];
//


public:
	CServerSystem();
	virtual ~CServerSystem();

	WORD GetServerNum() { return m_wServerNum; }
	void ConnectionCheck();
	void SendConnectionCheckMsg(USERINFO* pInfo);
	void SendSpeedHackCheckMsg(USERINFO* pInfo);

	void Process();
	void Start(WORD ServerNum);
	void End();		

	// PJH가 추가함
	void SetMapChangeInfo();
	MAPCHANGE_INFO* GetMapChangeInfo(DWORD Index);
	
	// KES
	void SendMsgMapServerDown( WORD wServerNum );
	void SetEnableCrypt( BOOL bEnable )		{ m_bUseCrypt = bEnable; }
	BOOL IsEnableCrypt()					{ return m_bUseCrypt; }

	void SetNation();
	int GetNation() { return m_Nation; }

	BOOL IsTestServer() { return m_bTestServer; }

#ifdef _CHINA_LOCAL_
	void SendBillingRemainTimeMsg( USERINFO* pInfo );
#endif
	
//이벤트공지용
	void SetEventNotifyStr( char* pStrTitle, char* pStrContext );
	void SetUseEventNotify( BOOL bUse );
	BOOL IsUseEventNotify()			{ return m_bUseEventNotify; }
	char* GetEventNotifyTitle()		{ return m_strEventNotifyTitle; }
	char* GetEventNotifyContext()	{ return m_strEventNotifyContext; }
	void ResetApplyEvent()			{	memset( m_bEvenList, 0, sizeof(BOOL)*eEvent_Max );	}
	void SetApplyEvent( DWORD EventIdx )	{	m_bEvenList[EventIdx] = TRUE;	}
	// magi82(43)
	BOOL* GetApplyEvent()	{	return m_bEvenList;	}
//
	

//---	
	DWORD MakeAuthKey();
	void ReleaseAuthKey(DWORD key);
};


extern CServerSystem * g_pServerSystem;
void OnConnectServerSuccess(DWORD dwIndex, void* pVoid);
void OnConnectServerFail(void* pVoid);

void ProcessDBMessage();
void ReceivedMsgFromServer(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);
void ReceivedMsgFromUser(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);
void OnAcceptServer(DWORD dwConnectionIndex);
void OnDisconnectServer(DWORD dwConnectionIndex);
void OnAcceptUser(DWORD dwConnectionIndex);
void OnDisconnectUser(DWORD dwConnectionIndex);

void GameProcess();

void ButtonProc1();
void ButtonProc2();
void ButtonProc3();
void OnCommand(char* szCommand);


#endif // !defined(AFX_SERVERSYSTEM_H__FD3EBFC3_EE3D_4505_A5A1_24DA471D20AB__INCLUDED_)
