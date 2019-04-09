// SuryunRegenManager.h: interface for the CSuryunRegenManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SURYUNREGENMANAGER_H__2F9780A4_E280_4B82_A373_84EBBC5E4F76__INCLUDED_)
#define AFX_SURYUNREGENMANAGER_H__2F9780A4_E280_4B82_A373_84EBBC5E4F76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define SURYUNREGENMGR  CSuryunRegenManager::GetInstance()
class CSuryunRegen;
class CSuryunRegenInfo;
class CSuryunRegenManager  
{
	CSuryunRegenManager();
	CYHHashTable<CSuryunRegen> m_SuryunRegen;
	CYHHashTable<CSuryunRegenInfo> m_SuryunRegenInfo;
	
public:
	GETINSTANCE(CSuryunRegenManager);
	~CSuryunRegenManager();
	
	void LoadSuryunRegenList();
	CSuryunRegen* MakeNewRegen(int RegenNum, DWORD SuryunID, WORD* MonsterKindInfo);
	void ReleaseRegen(DWORD SuryunID);
		
	void Release();
};

#endif // !defined(AFX_SURYUNREGENMANAGER_H__2F9780A4_E280_4B82_A373_84EBBC5E4F76__INCLUDED_)
