// MASManager.h: interface for the CMASManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MASMANAGER_H__7D213F53_3D21_4CE7_AA50_F5B5A366D352__INCLUDED_)
#define AFX_MASMANAGER_H__7D213F53_3D21_4CE7_AA50_F5B5A366D352__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define MASMGR USINGTON(CMASManager)

#include "Hashtable.h"

class CMASManager  
{
	CYHHashTable<SERVERSTATE>	m_ServerStateTable;

public:
	CMASManager();
	virtual ~CMASManager();

	void LoadMapServerInfo();
	void AddServerState(SERVERSTATE * NewState, DWORD key);
	SERVERSTATE * GetServerState(DWORD key);

	void SetStateInitForConn(DWORD dwConnectionIdx);

	void Release();
	void SendServerStateToMCs(DWORD key);
	void SendServerStateToMC(DWORD dwConnectionIdx, DWORD key);
	void SendServerStateAllToMC(DWORD dwConnectionIdx);
	void SendServerStateBootListToMS(DWORD dwConnectionIdx, WORD ServerTypeOrig, WORD ServerPortOrig);
	void SendServerStateAllToMCAll();
	void SendMSServerStateAllToMCAll();

	void	SendToAllMC( char* pMsg, DWORD dwLength );

};
EXTERNGLOBALTON(CMASManager)
#endif // !defined(AFX_MASMANAGER_H__7D213F53_3D21_4CE7_AA50_F5B5A366D352__INCLUDED_)
