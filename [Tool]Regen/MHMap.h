// MHMap.h: interface for the CMHMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MHMAP_H__444BAED6_A7A5_4CB6_B1BF_027999B116FE__INCLUDED_)
#define AFX_MHMAP_H__444BAED6_A7A5_4CB6_B1BF_027999B116FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../4DyuchiGRX_common/typedef.h"
#include "./Engine/EngineMap.h"
#include "./Engine/EngineSky.h"
#include "./Engine/EngineLight.h"
#include "./Engine/EngineObject.h"
#include "TileManager.h"

class CTileManager;
class CMHFile;

#define MAP		CMHMap::GetInstance()
#define MAX_WIDTH_HIEGTH 1048576

struct MAPDESC
{
	MAPDESC()
	{
		DefaultSight = 8000;
		Fov = 45;

		fogdesc.bEnable = TRUE;
		fogdesc.dwColor = 0xaaaaaaaa;
		fogdesc.fDensity = 0.5f;
		fogdesc.fStart = 20000;
		fogdesc.fEnd = 80000;
		
		strcpy(MapFileName,"new_gae_002.map");
		strcpy(TileFileName,"");
		strcpy(SkyMod,"SkyBox.MOD");
		strcpy(SkyAnm,"SkyBox.ANM");
		strcpy(BGMSound,"bg_login.mp3");
		strcpy(SunObject,"bg_login.mp3");

		bSun = 0;
		bSky = 1;

		Ambient = 0xaaaaaaaa;
		
		SunPos.x = 800;
		SunPos.y = 1300;
		SunPos.z = 200;		
		
		backColor = 0x00000000;

		SunDistance = 7000;

		bFixHeight = FALSE;
	}
	BOOL bFixHeight;
	float FixHeight;
	VECTOR3 SunPos;

	DWORD Ambient;

	float DefaultSight;
	float Fov;
	
	FOG_DESC fogdesc;
	
	char MapFileName[64];
	char TileFileName[64];
	
	BOOL bSky;
	char SkyMod[64];
	char SkyAnm[64];

	char BGMSound[64];
	
	BOOL bSun;
	char SunObject[64];

	DWORD backColor;

	float SunDistance;
};
class CMHMap  
{
	DWORD m_MapNum;
	VECTOR3 m_ShadowPivotPos;

	MAPDESC* mapDesc;
	CEngineMap map;
	CEngineSky* sky;
	CTileManager* m_pTileManager;
	CEngineObject m_SunObject; 
	
	CEngineLight m_Shadowlight;
	IHeightField* m_pHField;
	
	float m_AttY[MAX_WIDTH_HIEGTH];
	
	void LoadStaticNpc(DWORD MapNum);
	BOOL LoadMapDesc(CMHFile* file);
public:
	GETINSTANCE(CMHMap);

	CMHMap();
	virtual ~CMHMap();

	void InitMap(BYTE MapNum);
	void Release();

	void InitTile(char* TileName, CTile* pViewTile);
	void Process(DWORD tick);
	
	BOOL CollisonCheck(VECTOR3* pStart,VECTOR3* pEnd,VECTOR3* pRtCollisonPos);		// 충돌하면 TRUE반환하고 pRtCollisonPos에 새로운 타겟 설정
	void SetShadowPivotPos(VECTOR3* pPivotPos);
	CTileManager* GetTileManager()		{	return m_pTileManager;	}
	DWORD GetMapNum()	{	return m_MapNum;	}
	void TestLoadMap(char* szMapFile);
};

#endif // !defined(AFX_MHMAP_H__444BAED6_A7A5_4CB6_B1BF_027999B116FE__INCLUDED_)
