// DirectoryManager.cpp: implementation of the CDirectoryManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DirectoryManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDirectoryManager::CDirectoryManager()
{
	m_CurMode = 0;
}

CDirectoryManager::~CDirectoryManager()
{
	
}

void CDirectoryManager::Init()
{
	char root[256] = {0,};
	char temp[256] = {0,};
	GetCurrentDirectory(256,root);
	m_RootDirectory = root;

#define DM_INIT(mode,Directory)		strcpy(temp,root);		\
									strcat(temp,Directory);	\
									m_Directory[mode] = temp;
	if(g_bUsingEnginePack)
	{
		DM_INIT(eLM_Root				,""				);
		DM_INIT(eLM_EffectScript		,LDP_EFFECTSCRIPT);
		DM_INIT(eLM_Ini					,LDP_INI			);
		DM_INIT(eLM_Sound				,LDP_SOUND		);
		DM_INIT(eLM_Resource			,LDP_RESOURCE	);
		DM_INIT(eLM_BGM					,LDP_BGM			);
		DM_INIT(eLM_Character			,LDP_CHARACTER	);
		DM_INIT(eLM_Monster				,LDP_MONSTER		);
		DM_INIT(eLM_Npc					,LDP_NPC			);
		DM_INIT(eLM_Effect				,LDP_EFFECT		);
		DM_INIT(eLM_Map					,LDP_MAP			);
		DM_INIT(eLM_Cloud				,LDP_CLOUD		);
		DM_INIT(eLM_Minimap				,LDP_MINIMAP		);
	}
	else
	{
		DM_INIT(eLM_Root				,""				);
		DM_INIT(eLM_EffectScript		,LD_EFFECTSCRIPT);
		DM_INIT(eLM_Ini					,LD_INI			);
		DM_INIT(eLM_Sound				,LD_SOUND		);
		DM_INIT(eLM_Resource			,LD_RESOURCE	);
		DM_INIT(eLM_BGM					,LD_BGM			);
		DM_INIT(eLM_Character			,LD_CHARACTER	);
		DM_INIT(eLM_Monster				,LD_MONSTER		);
		DM_INIT(eLM_Npc					,LD_NPC			);
		DM_INIT(eLM_Effect				,LD_EFFECT		);
		DM_INIT(eLM_Map					,LD_MAP			);
		DM_INIT(eLM_Cloud				,LD_CLOUD		);
		DM_INIT(eLM_Minimap				,LD_MINIMAP		);
	}


	m_CurMode = eLM_Root;

}

void CDirectoryManager::Release()
{
	SetCurrentDirectory(m_RootDirectory);
}

void CDirectoryManager::SetLoadMode(DWORD LoadMode)
{
	ASSERT(LoadMode < eLM_Max);

	if(LoadMode == m_CurMode)
	{
		return;
	}
	
	if(LoadMode == eLM_NotChange)
	{
		return;
	}

	SetCurrentDirectory(m_Directory[LoadMode]);

	m_CurMode = LoadMode;
}
