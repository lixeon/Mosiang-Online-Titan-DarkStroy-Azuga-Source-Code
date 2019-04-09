#pragma once

#include "../max_common/stdafx.h"
#include "../4DyuchiGXGFunc/global.h"

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
	TCHAR			uszTexmapFileName[TEXTURE_MAP_TYPE_NUM][MAX_NAME_LEN];
	TCHAR			uszMtlName[MAX_NAME_LEN];
	DWORD			dwIndex;
	DWORD			dwFlag;
};
#pragma pack(pop)

class CMaterial
{
	OBJECT_TYPE		m_dwType;
	DWORD			m_dwTextureNum;
	DWORD			m_dwDiffuse;
	DWORD			m_dwAmbient;
	DWORD			m_dwSpecular;
	float			m_fTransparency;
	float			m_fShine;
	float			m_fShineStrength;
	TCHAR			m_uszTexmapFileName[TEXTURE_MAP_TYPE_NUM][MAX_NAME_LEN];
	TCHAR			m_uszMtlName[MAX_NAME_LEN];
	DWORD			m_dwIndex;
	DWORD			m_dwFlag;

public:
	BOOL			IsEqual(CMaterial* pMtl);
	void			SetName(TCHAR* uszName) {lstrcpy(m_uszMtlName,uszName);}
	void			Set2Side()	{ m_dwFlag |= MATERIAL_TYPE_2SIDE;}
	void			SetDiffuse(float r,float g,float b,float a) {m_dwDiffuse = COLORtoDWORD(r,g,b,a);}
	void			SetAmbient(float r,float g,float b,float a) {m_dwAmbient = COLORtoDWORD(r,g,b,a);}
	void			SetSpecular(float r,float g,float b,float a) {m_dwSpecular = COLORtoDWORD(r,g,b,a);}
	void			SetTransparency(float fVal) {m_fTransparency = fVal;}
	void			SetShine(float fVal) {m_fShine = fVal;}
	void			SetShineStrength(float fVal) {m_fShineStrength = fVal;}
	
	DWORD			GetDiffuse() {return m_dwDiffuse;}
	DWORD			GetAmbient() {return m_dwAmbient;}
	DWORD			GetSpecular() {return m_dwSpecular;}

	float			GetTransparency() {return m_fTransparency;}
	float			GetShine() {return m_fShine;}
	float			GetShineStrength() {return m_fShineStrength;}
	
	DWORD			GetIndex()				{return m_dwIndex;}
	void			SetIndex(DWORD dwIndex) {m_dwIndex = dwIndex;}
	BOOL			SetTexture(TEXTURE_MAP_TYPE type,TCHAR* uszFileName);
	void			SetTranspFilter() {m_dwFlag = (m_dwFlag & MATERIAL_TRANSP_TYPE_MASK_INVERSE) | TRANSP_TYPE_FILTER;}
	void			SetTranspSubtractive() {m_dwFlag = (m_dwFlag & MATERIAL_TRANSP_TYPE_MASK_INVERSE) | TRANSP_TYPE_SUBTRACTIVE;}
	void			SetTranspAdditive() {m_dwFlag = (m_dwFlag & MATERIAL_TRANSP_TYPE_MASK_INVERSE) | TRANSP_TYPE_ADDITIVE;}
	void			SetSelfIllunum()	{m_dwFlag = (m_dwFlag & MATERIAL_ILLUNUM_TYPE_MASK_INVERSE) | SELF_ILLUNUM;}
	DWORD			WriteFile(FILE* fp);

	CMaterial*		m_pNextMaterial;

	

	CMaterial();
};
