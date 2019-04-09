// ServerSystem.h: interface for the CServerSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERSYSTEM_H__FD3EBFC3_EE3D_4505_A5A1_24DA471D20AB__INCLUDED_)
#define AFX_SERVERSYSTEM_H__FD3EBFC3_EE3D_4505_A5A1_24DA471D20AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SERVERLISTNAME "DistributeServer_info.txt"
#define __DISTRIBUTESERVER__

#include "../4dyuchigxgfunc/global.h"
#include "UserTable.h"

enum eNATION
{
	eNATION_KOREA,
	eNATION_CHINA,
};



class CServerSystem  
{
	DWORD m_DBThreadID;
//	INDEXCR_HANDLE m_IdxCreater;

	BOOL m_bUserFireWall;
	char	m_strFireWallIP[16];

//국가
	int		m_Nation;

#ifdef _HK_LOCAL_
	int		m_aniTimeMale[30];
	int		m_ac[30];
#endif

public:
	DWORD m_DBThreadTerminate;
	
	CServerSystem();
	virtual ~CServerSystem();


	void Process();
	void Start(WORD ServerNum);
	void End();		

	DWORD MakeAuthKey();
	void ReleaseAuthKey(DWORD key);
	
#ifdef _HK_LOCAL_
	void LoadSAT();
	int GetSAT( int n ) { return m_aniTimeMale[n]; }
	void LoadAC();
	int GetAC( int n ) { return m_ac[n]; }
#endif
	void LoadFireWallIP();
	BOOL IsUsedFireWall() { return m_bUserFireWall;	}
	char* GetFireWallIP() { return m_strFireWallIP;	}

//국가
	void SetNation();
	int GetNation() { return m_Nation; }
	
	void ConnectionCheck();
	void SendConnectionCheckMsg(USERINFO* pInfo);
	BOOL IsInvalidCharInclude( char* pStr );

};

extern CServerSystem * g_pServerSystem;
void OnConnectServerSuccess(DWORD dwIndex, void* pVoid);
void OnConnectServerFail(void* pVoid);

#endif // !defined(AFX_SERVERSYSTEM_H__FD3EBFC3_EE3D_4505_A5A1_24DA471D20AB__INCLUDED_)
