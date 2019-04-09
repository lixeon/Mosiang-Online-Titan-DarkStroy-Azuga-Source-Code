#pragma once

#include "../4DyuchiGRX_common/stdafx.h"
#include "../4DyuchiGRX_common/typedef.h"
#include "../4DyuchiGXGFunc/global.h"

#pragma pack(push,1)
struct STM_MATERIAL
{
	OBJECT_TYPE		dwType;
	DWORD			dwIndex;

	DWORD			dwTextureNum;
	DWORD			dwDiffuse;
	DWORD			dwAmbient;
	DWORD			dwSpecular;
	float			fTransparency;
	float			fShine;
	float			fShineStrength;
	char			szTexmapFileName[MAX_NAME_LEN];
	DWORD			dwFlag;
};
#pragma pack(pop)

#pragma pack(push,4)

struct FILE_MATERIAL_HEADER
{
	DWORD			dwTextureNum;
	DWORD			dwDiffuse;
	DWORD			dwAmbient;
	DWORD			dwSpecular;
	float			fTransparency;
	float			fShine;
	float			fShineStrength;
	char			szTexmapFileName[TEXTURE_MAP_TYPE_NUM][MAX_NAME_LEN];
	char			szMtlName[MAX_NAME_LEN];
	DWORD			dwIndex;
	DWORD			dwFlag;
};
#pragma pack(pop)



class CMaterial
{
	OBJECT_TYPE		m_dwType;
	DWORD			m_dwIndex;

	DWORD			m_dwTextureNum;
	DWORD			m_dwDiffuse;
	DWORD			m_dwAmbient;
	DWORD			m_dwSpecular;
	float			m_fTransparency;
	float			m_fShine;
	float			m_fShineStrength;
	char			m_szDiffuseTexmapFileName[MAX_NAME_LEN];	
	char			m_szReflectTexmapFileName[MAX_NAME_LEN];	
	char			m_szBumpTexmapFileName[MAX_NAME_LEN];

	DWORD			m_dwFlag;
	
	// 이 앞까진 MATERIAL과 동일.추가된 영역 
	char			m_szMtlName[MAX_NAME_LEN];

public:
	OBJECT_TYPE		GetType()		{return m_dwType;}

	MATERIAL*		GetMaterialInfo() {return (MATERIAL*)&m_dwTextureNum;}
	void			SetMateiralInfo(MATERIAL* pMtl) {*(MATERIAL*)&m_dwTextureNum = *pMtl;}
	char*			GetDiffuseTexmapName() {return m_szDiffuseTexmapFileName;}
	char*			GetReflectTexmapName() {return m_szReflectTexmapFileName;}
	char*			GetBumpTexmapName() {return m_szBumpTexmapFileName;}

	void			SetDiffuse(float r,float g,float b,float a) {m_dwDiffuse = COLORtoDWORD(r,g,b,a);}
	void			SetAmbient(float r,float g,float b,float a) {m_dwAmbient = COLORtoDWORD(r,g,b,a);}
	void			SetSpecular(float r,float g,float b,float a) {m_dwSpecular = COLORtoDWORD(r,g,b,a);}
	void			SetTransparency(float fVal) {m_fTransparency = fVal;}
	void			SetShine(float fVal) {m_fShine = fVal;}
	void			SetShineStrength(float fVal) {m_fShineStrength = fVal;}
	void			SetIndex(DWORD dwIndex) {m_dwIndex = dwIndex;}

	DWORD			GetDiffuse() {return m_dwDiffuse;}
	DWORD			GetAmbient() {return m_dwAmbient;}
	DWORD			GetSpecular() {return m_dwSpecular;}

	float			GetTransparency() {return m_fTransparency;}
	float			GetShine() {return m_fShine;}
	float			GetShineStrength() {return m_fShineStrength;}
	DWORD			GetIndex() {return m_dwIndex;}
	char*			GetName() {return m_szMtlName;}

	DWORD			ReadFile(void* pFP);
	DWORD			ReadSTMMaterial(void* pFP);
	DWORD			WriteSTMMaterial(FILE* fp);


	

	CMaterial();
};
