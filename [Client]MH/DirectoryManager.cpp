// DirectoryManager.cpp: implementation of the CDirectoryManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DirectoryManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CDirectoryManager)
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

#ifdef _HACK_SHIELD_
	int nLen = GetModuleFileName( NULL, root, 256 );
	char* t = root + nLen;

	while( *(--t) != '\\' );
	*t = 0;
#else
	GetCurrentDirectory(256,root);
#endif
	m_RootDirectory = root;

#define DM_INIT(mode,Directory)		strcpy(temp,root);		\
									strcat(temp,Directory);	\
									m_Directory[mode] = temp;
	
	if(g_bUsingEnginePack)
	{
		DM_INIT(eLM_Root				,""				);
		DM_INIT(eLM_EffectScript		,LDP_EFFECTSCRIPT);
		DM_INIT(eLM_Ini					,LDP_INI		);
		DM_INIT(eLM_Sound				,LDP_SOUND		);
		DM_INIT(eLM_Resource			,LDP_RESOURCE	);
		DM_INIT(eLM_BGM					,LDP_BGM		);
		DM_INIT(eLM_Character			,LDP_CHARACTER	);
		DM_INIT(eLM_Pet					,LDP_PET		);
		DM_INIT(eLM_Monster				,LDP_MONSTER	);
		DM_INIT(eLM_Npc					,LDP_NPC		);
		DM_INIT(eLM_Effect				,LDP_EFFECT		);
		DM_INIT(eLM_Map					,LDP_MAP		);
		DM_INIT(eLM_Cloud				,LDP_CLOUD		);
		DM_INIT(eLM_Minimap				,LDP_MINIMAP	);
		DM_INIT(eLM_MapObject			,LDP_MAPOBJECT	);
		DM_INIT(eLM_Titan				,LDP_TITAN		);
		DM_INIT(eLM_Map2				,LDP_MAP2		);
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
		DM_INIT(eLM_Pet					,LD_PET			);
		DM_INIT(eLM_Monster				,LD_MONSTER		);
		DM_INIT(eLM_Npc					,LD_NPC			);
		DM_INIT(eLM_Effect				,LD_EFFECT		);
		DM_INIT(eLM_Map					,LD_MAP			);
		DM_INIT(eLM_Cloud				,LD_CLOUD		);
		DM_INIT(eLM_Minimap				,LD_MINIMAP		);
		DM_INIT(eLM_MapObject			,LD_MAPOBJECT	);
		DM_INIT(eLM_Titan				,LD_TITAN		);
		DM_INIT(eLM_Map2				,LD_MAP2		);
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

#define BEGINLD(a)		char tmp[255];	switch(a)	{	case -1:	{
#define ENDLD				}	break;		}
#define CLD(a, b)				}	break;		case (a):	{  strcpy(tmp, b);

char * CDirectoryManager::GetFullDirectoryName(DWORD mode)
{
	ASSERT(mode < eLM_Max);

	static char path[255] = {0,};

	BEGINLD(mode)
		CLD(eLM_Root, "")	
		CLD(eLM_EffectScript, LD_EFFECTSCRIPT)
		CLD(eLM_Ini, LD_INI)
		CLD(eLM_Sound, LD_SOUND)
		CLD(eLM_Resource, LD_RESOURCE)
		CLD(eLM_BGM, LD_BGM)	
		CLD(eLM_Character, LD_CHARACTER)
		CLD(eLM_Pet, LD_PET)
		CLD(eLM_Monster, LD_MONSTER)	
		CLD(eLM_Npc, LD_NPC)	
		CLD(eLM_Effect, LD_EFFECT)
		CLD(eLM_Map, LD_MAP)		
		CLD(eLM_Cloud, LD_CLOUD)
		CLD(eLM_Minimap, LD_MINIMAP)
		CLD(eLM_Titan, LD_TITAN)
		CLD(eLM_Map2, LD_MAP2)
	ENDLD
	sprintf( path, "%s\\%s", LPCTSTR(m_RootDirectory), tmp );

	return path;
}	
