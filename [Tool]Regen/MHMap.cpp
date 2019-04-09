// MHMap.cpp: implementation of the CMHMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MHMap.h"

//#include "GameResourceManager.h"
#include "TileManager.h"
#include "MHFile.h"
#include "DefineStruct.h"
#include "resource.h"
#include "MainFrm.h"
#include "RegenToolDoc.h"
#include "RegenToolView.h"

//#include "Mp3/BGMLoader.h"
//#include "ObjectManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMHMap::CMHMap()
{
	sky = NULL;
	m_pTileManager = NULL;
	mapDesc = NULL;
}

CMHMap::~CMHMap()
{
//	Release();
	SAFE_DELETE(mapDesc);
}

void CMHMap::InitMap(BYTE MapNum)
{
	m_MapNum = MapNum;
	char mapdescfile[256];
	sprintf(mapdescfile,"ini\\MAP%d.mhm",MapNum);
	CMHFile file;
	file.Init(mapdescfile,"r");
	LoadMapDesc(&file);
	file.Release();
	
//	MAINGAME->GetEngine()->GetGraphicsEngine()->SetBackGroundColor(mapDesc->backColor);
//	MAINGAME->GetEngine()->GetGraphicsEngine()->SetFixHeight(mapDesc->bFixHeight,mapDesc->FixHeight);
	
	//YH2DO Remove Global Variable
	bRenderSky = mapDesc->bSky;
	g_pExecutive->GetGeometry()->SetAmbientColor(0,mapDesc->Ambient);
	
	DIRECTORYMGR->SetLoadMode(eLM_Map);
	{
		// 임시
		BOOL rt;
		rt = map.Init(mapDesc->MapFileName);
		
		SAFE_DELETE(sky);
		if(mapDesc->bSky)
		{
			sky = new CEngineSky;
			sky->CreateSky(mapDesc->SkyMod,mapDesc->SkyAnm);
		}
		
		m_pTileManager = new CTileManager;
		if(m_pTileManager->LoadTileInfo(mapDesc->TileFileName) == FALSE)
			SAFE_DELETE(m_pTileManager);
	}
	DIRECTORYMGR->SetLoadMode(eLM_Root);

	
	if(mapDesc->fogdesc.bEnable)
	{
		// 임시
		g_pExecutive->GetGeometry()->EnableFog(0);
		g_pExecutive->GetGeometry()->SetFog(&mapDesc->fogdesc,0);
	}

	VECTOR3 pos;
	pos.x = 0;
	pos.y = 0;
	pos.z = 0;
	g_pExecutive->GetGeometry()->ResetCamera(&pos,100,mapDesc->DefaultSight,DEGTORAD(mapDesc->Fov),0);	

	//////////////////////////////////////////////////////////////////////////
	// BGM 
	/*
	//taiyo : 시끄럽다.
	*/
	if(mapDesc->BGMSound[0])
	{
		//MP3->PlayFile(mapDesc->BGMSound,TRUE);
		//mapDesc->BGMSound[0] = 0;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 방향성 라이트
	g_pExecutive->GetGeometry()->EnableDirectionalLight(0);
	
	DIRECTIONAL_LIGHT_DESC LightDesc;
	LightDesc.dwAmbient = 0;
	LightDesc.dwDiffuse = 0x66666666;
	LightDesc.dwSpecular = 0;
	LightDesc.v3Dir = mapDesc->SunPos * -1.f;
	LightDesc.bEnable = FALSE;
	g_pExecutive->GetGeometry()->SetDirectionalLight(&LightDesc,0);
	//////////////////////////////////////////////////////////////////////////
	

	//////////////////////////////////////////////////////////////////////////
	// 그림자
//	if(GAMERESRCMNGR->m_GameDesc.bShadow)
//	{
//		VECTOR3 to;
//		to.x = 0;
//		to.y = 0;
//		to.z = 0;
//		m_Shadowlight.InitShadowLight(0,&pos,&to,gPHI/8,5000);
//	}
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// 해 와 달
	if(mapDesc->bSun)
	{
		DIRECTORYMGR->SetLoadMode(eLM_Map);
		m_SunObject.Init(mapDesc->SunObject,NULL,eEngineObjectType_Effect);
		DIRECTORYMGR->SetLoadMode(eLM_Root);
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 스태틱 Npc 
	if(MapNum > 0)			// taiyo code	
	LoadStaticNpc(MapNum);

	//////////////////////////////////////////////////////////////////////////
	
}

void CMHMap::InitTile(char* TileName, CTile* pViewTile)
{
//	DIRECTORYMGR->SetLoadMode(eLM_Resource);

	m_pTileManager = new CTileManager;
	if(m_pTileManager->LoadTileInfo(TileName) == FALSE)
		SAFE_DELETE(m_pTileManager);

	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CRegenToolView* pView = (CRegenToolView*)pMain->GetActiveView();

	DIRECTORYMGR->SetLoadMode(eLM_Root);
}
void CMHMap::Release()
{
	ReleasePool();
	ProcessGarbageObject();
	m_Shadowlight.Release();
//	if(MP3)
//		MP3->Stop();
	SAFE_DELETE(mapDesc);
	map.Release();
	SAFE_DELETE(sky);
	SAFE_DELETE(m_pTileManager);
	m_SunObject.Release();
}

void CMHMap::Process(DWORD CurTime)
{	
	static DWORD SumTick = 0;
	static DWORD LastTime = 0;
	static DWORD Tick = 0;

	if(LastTime)
	{
		Tick = CurTime - LastTime;
		LastTime = CurTime;
	}
	else
	{
		LastTime = CurTime;
		return;
	}


	if(mapDesc->bSky)
	{
		SumTick += Tick;
		if(SumTick > 50)
		{
			sky->IncreaseSkyAniFrame(1);
			SumTick -= 50;
		}
	
		sky->RenderSky();
	}

/*	if(GAMERESRCMNGR->m_GameDesc.bShadow)
	{
		VECTOR3 pos;
		pos = m_ShadowPivotPos + mapDesc->SunPos;
		m_Shadowlight.SetPosition(&pos);
		LIGHT_DESC desc;
		m_Shadowlight.GetLightDesc(&desc);
		desc.v3Point = pos;
		desc.v3To = m_ShadowPivotPos;
		m_Shadowlight.SetLightDesc(&desc);
	}
*/
	if(mapDesc->bSun)
	{
		VECTOR3 pos;
		Normalize(&pos,&mapDesc->SunPos);
		pos = pos * mapDesc->SunDistance;
		pos = m_ShadowPivotPos;// + pos;
		pos.x += 10000;
		pos.y -= 2000;
		m_SunObject.SetEngObjPosition(&pos);
	}
}
BOOL CMHMap::CollisonCheck(VECTOR3* pStart,VECTOR3* pEnd,VECTOR3* pRtCollisonPos)
{
	if(m_pTileManager == NULL)
		return FALSE;
	if(m_pTileManager->CollisonCheck(pStart,pEnd,pRtCollisonPos) == TRUE)
		return TRUE;


	return FALSE;
}

void CMHMap::SetShadowPivotPos(VECTOR3* pPivotPos)
{
	m_ShadowPivotPos = *pPivotPos;
}


BOOL CMHMap::LoadMapDesc(CMHFile* file)
{
	char value[64] = {0,};
	SAFE_DELETE(mapDesc);
	mapDesc = new MAPDESC;

	while(1)
	{
		if(file->IsEOF())
			break;

		strcpy(value,strupr(file->GetString()));

		if(strcmp(value,"*SIGHT") == 0)
		{
			mapDesc->DefaultSight = file->GetFloat();
		}
		else if(strcmp(value,"*FOV") == 0)
		{
			mapDesc->Fov = file->GetFloat();
		}
		else if(strcmp(value,"*FOG") == 0)
		{
			mapDesc->fogdesc.bEnable = file->GetBool();
		}
		else if(strcmp(value,"*FOGCOLOR") == 0)
		{
			DWORD r,g,b,a;
			r = file->GetDword();
			g = file->GetDword();
			b = file->GetDword();
			a = file->GetDword();
			mapDesc->fogdesc.dwColor = RGBA_MAKE(r,g,b,a);
		}
		else if(strcmp(value,"*FOGDENSITY") == 0)
		{
			mapDesc->fogdesc.fDensity = file->GetFloat();
		}
		else if(strcmp(value,"*FOGSTART") == 0)
		{
			mapDesc->fogdesc.fStart = file->GetFloat();
		}
		else if(strcmp(value,"*FOGEND") == 0)
		{
			mapDesc->fogdesc.fEnd = file->GetFloat();
		}
		else if(strcmp(value,"*MAP") == 0)
		{
			file->GetString(mapDesc->MapFileName);
		}
		else if(strcmp(value,"*TILE") == 0)
		{
			file->GetString(mapDesc->TileFileName);
		}
		else if(strcmp(value,"*SKYMOD") == 0)
		{
			file->GetString(mapDesc->SkyMod);
		}
		else if(strcmp(value,"*SKYANM") == 0)
		{
			file->GetString(mapDesc->SkyAnm);
		}
		else if(strcmp(value,"*SKYBOX") == 0)
		{
			mapDesc->bSky = file->GetBool();
		}
		else if(strcmp(value,"*BGM") == 0)
		{
			file->GetString(mapDesc->BGMSound);
		}
		else if(strcmp(value,"*BRIGHT") == 0)
		{
			BYTE dd = file->GetByte();
			mapDesc->Ambient = RGBA_MAKE(dd,dd,dd,dd);
		}
		else if(strcmp(value,"*SUNPOS") == 0)
		{
			mapDesc->SunPos.x = file->GetFloat();
			mapDesc->SunPos.y = file->GetFloat();
			mapDesc->SunPos.z = file->GetFloat();
		}
		else if(strcmp(value,"*SUNOBJECT") == 0)
		{
			file->GetString(mapDesc->SunObject);
		}
		else if(strcmp(value,"*SUN") == 0)
		{
			mapDesc->bSun = file->GetBool();
		}
		else if(strcmp(value,"*SUNDISTANCE") == 0)
		{
			mapDesc->SunDistance = file->GetFloat();
		}
		else if(strcmp(value,"*BACKCOLOR") == 0)
		{
			DWORD r,g,b,a;
			r = file->GetDword();
			g = file->GetDword();
			b = file->GetDword();
			a = file->GetDword();
			mapDesc->backColor = RGBA_MAKE(r,g,b,a);
		}
		else if(strcmp(value,"*FIXHEIGHT") == 0)
		{
			mapDesc->bFixHeight = TRUE;
			mapDesc->FixHeight = file->GetFloat();			
		}
	}
	
	file->Release();



	return TRUE;
}

void CMHMap::LoadStaticNpc(DWORD MapNum)
{
/*	DWORD npcMapnum;
	char value[256] = {0,};

	CMHFile file;
	file.Init("Resource/StaticNpc.txt","r");

	if(file.IsInited() == FALSE)
		return;

	NPC_TOTALINFO tinfo;
	BASEOBJECT_INFO binfo;
	BASEMOVE_INFO minfo;

	DWORD n=0;

	while(1)
	{
		if(file.IsEOF())
			break;

		npcMapnum = file.GetDword();
		if(npcMapnum != MapNum)
		{
			file.GetLine(value,256);
			continue;
		}

		binfo.dwObjectID = STATICNPC_ID_START + n++;
		binfo.ObjectState = 0;
		tinfo.MapNum = (BYTE)MapNum;
		tinfo.NpcJob = 0;
		minfo.bMoving = FALSE;
		minfo.KyungGongIdx = 0;
		minfo.MoveMode = 0;

		tinfo.NpcKind = file.GetWord();
		file.GetString(binfo.ObjectName);
		tinfo.NpcUniqueIdx = file.GetWord();
		minfo.CurPosition.x = file.GetFloat();
		minfo.CurPosition.y = 0;
		minfo.CurPosition.z = file.GetFloat();
		
		OBJECTMGR->AddNpc(&binfo,&minfo,&tinfo);
	}
	
	file.Release();
*/
}

void CMHMap::TestLoadMap(char* szMapFile)
{
	char temp[256];
	GetCurrentDirectory(256,temp);
	BOOL rt = map.Init(szMapFile);

}