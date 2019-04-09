// DirectoryManager.h: interface for the CDirectoryManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIRECTORYMANAGER_H__366E8598_49B1_4150_BFDE_9772A132F74B__INCLUDED_)
#define AFX_DIRECTORYMANAGER_H__366E8598_49B1_4150_BFDE_9772A132F74B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DefineStruct.h"

enum eLOADMODE
{
	eLM_Root,
	eLM_EffectScript,
	eLM_Ini,
	eLM_Sound,
	eLM_Resource,
	eLM_BGM,
	eLM_Character,
	eLM_Monster,
	eLM_Npc,
	eLM_Effect,
	eLM_Minimap,

	eLM_Map,
	eLM_Cloud,

	eLM_NotChange,

	eLM_Max,
};

#define LDP_EFFECTSCRIPT	"\\EffectScript"
#define LDP_INI				"\\Ini"
#define LDP_SOUND			"\\Sound"
#define LDP_RESOURCE			"\\Resource"
#define LDP_BGM				"\\BGM"
#define LDP_CHARACTER		""
#define LDP_MONSTER			""
#define LDP_NPC				""
#define LDP_EFFECT			""
#define LDP_MINIMAP			"\\Image\\Minimap"
#define LDP_MAP				""
#define LDP_CLOUD			""

#define LD_EFFECTSCRIPT		"\\EffectScript"
#define LD_INI				"\\Ini"
#define LD_SOUND			"\\Sound"
#define LD_RESOURCE			"\\Resource"
#define LD_BGM				"\\BGM"
#define LD_CHARACTER		"\\3DData\\Character"
#define LD_MONSTER			"\\3DData\\Monster"
#define LD_NPC				"\\3DData\\Npc"
#define LD_EFFECT			"\\3DData\\Effect"
#define LD_MINIMAP			"\\Image\\Minimap"
#define LD_MAP				"\\3DData\\Map"
#define LD_CLOUD			"\\3DData\\Cloud"

#define DIRECTORYMGR	CDirectoryManager::GetInstance()

class CDirectoryManager  
{
	StaticString m_RootDirectory;
	StaticString m_Directory[eLM_Max];
	
	DWORD m_CurMode;

public:
	MAKESINGLETON(CDirectoryManager);

	CDirectoryManager();
	virtual ~CDirectoryManager();

	void Init();
	void Release();

	void SetLoadMode(DWORD LoadMode);
};

#endif // !defined(AFX_DIRECTORYMANAGER_H__366E8598_49B1_4150_BFDE_9772A132F74B__INCLUDED_)
