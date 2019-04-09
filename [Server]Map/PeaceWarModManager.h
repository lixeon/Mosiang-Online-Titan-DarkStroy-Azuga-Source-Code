// PeaceWarModManager.h: interface for the CPeaceWarModManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PEACEWARMODMANAGER_H__51960C2A_C232_4145_A89A_FDE083298787__INCLUDED_)
#define AFX_PEACEWARMODMANAGER_H__51960C2A_C232_4145_A89A_FDE083298787__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define PWMODMGR_OBJ CPeaceWarModManager::GetInstance()

class CPlayer;

class CPeaceWarModManager  
{
	static CHARACTER_TOTALINFO m_ModeAckMsg;
	CPeaceWarModManager();
public:
	MAKESINGLETON(CPeaceWarModManager);
	virtual ~CPeaceWarModManager();
	
	void Init_PeaceWarMode(CPlayer* pPlayer);
	void Peace_WarMode(CPlayer* pPlayer, BOOL bPeace); // ÀÚ½ÅÀÇ »óÅÂ¸¦ ¹Ù²Þ. 
};

#endif // !defined(AFX_PEACEWARMODMANAGER_H__51960C2A_C232_4145_A89A_FDE083298787__INCLUDED_)
