// PKManager.h: interface for the CPKManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PKMANAGER_H__7471A7CD_E158_427A_B1A1_6E6943F84662__INCLUDED_)
#define AFX_PKMANAGER_H__7471A7CD_E158_427A_B1A1_6E6943F84662__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CPlayer;

#define PKMGR CPKManager::GetInstance()


class CPKManager  
{
protected:
	
	BOOL	m_bPKAllow[MAX_CHANNEL_NUM];

public:
	CPKManager();
	virtual ~CPKManager();

	MAKESINGLETON( CPKManager );

	void Init( BOOL bPKAllow );
	void InitForChannel();

//	BOOL IsPKAllow() { return m_bPKAllow; }
	BOOL IsPKAllow( DWORD dwChannel )
	{
		if( dwChannel < 1 )
			dwChannel = 1;
		return m_bPKAllow[dwChannel-1];
	}
	void SetPKAllowWithMsg( BOOL bAllow );
	void NetworkMsgParse( BYTE Protocol, void* pMsg );

	void PKModeExitPanelty( CPlayer* pPlayer, BOOL bPKModePanelty = FALSE );		//°­Á¦Á¾·á

	void DiePanelty( CPlayer* pDiePlayer, CObject* pAttacker );
	void PKPlayerDiePanelty( CPlayer* pPlayer );
	void PKPlayerKillPanelty( CPlayer* pDiePlayer, CPlayer* pAttacker );
	void PKPlayerKiiledByWantedPanelty( CPlayer* pDiePlayer, CPlayer* pAttacker );
};

#endif // !defined(AFX_PKMANAGER_H__7471A7CD_E158_427A_B1A1_6E6943F84662__INCLUDED_)
