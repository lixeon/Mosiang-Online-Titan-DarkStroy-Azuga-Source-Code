// AppearanceManager.h: interface for the CAppearanceManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APPEARANCEMANAGER_H__6121D532_2201_469B_A298_35F0C2E66876__INCLUDED_)
#define AFX_APPEARANCEMANAGER_H__6121D532_2201_469B_A298_35F0C2E66876__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define APPEARANCEMGR CAppearanceManager::GetInstance()

class CPlayer;
class CObject;
class CMonster;
class CNpc;



//#include "PtrList.h"

class CAppearanceManager  
{
//	struct APPEAR_CHANGE
//	{
//		CPlayer* pObject;
//	};
//	cPtrList m_PtrList;

	void SetCharacterAppearance(CPlayer* pPlayer);
	void SetMonsterAppearance(WORD MonsterKind);
	void SetNpcAppearance(CNpc* pNpc);

	//void ChangeCharacterPart(APPEAR_CHANGE* pAC);

	BOOL GetPartName(DWORD AppearPart,DWORD PartNum,char* RtFileName);
public:
	MAKESINGLETON(CAppearanceManager);

	CAppearanceManager();
	virtual ~CAppearanceManager();

	void Init();
	void Release();
	
	void InitAppearance(CObject* pObject);
	void AddCharacterPartChange(DWORD PlayerID);
	void ProcessAppearance();
	//
};

#endif // !defined(AFX_APPEARANCEMANAGER_H__6121D532_2201_469B_A298_35F0C2E66876__INCLUDED_)
