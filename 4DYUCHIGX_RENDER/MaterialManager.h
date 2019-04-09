// MaterialManager.h: interface for the CMaterialManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATERIALMANAGER_H__EBBC1A17_319C_4649_8DD5_6195DF1C8EDA__INCLUDED_)
#define AFX_MATERIALMANAGER_H__EBBC1A17_319C_4649_8DD5_6195DF1C8EDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../4DyuchiGXGFunc/global.h"
#include "renderer_typedef.h"
#include "CTexture.h"

class CoD3DDevice;


struct DIMATERIAL
{
	CTexture				TextureDiffuse;
	CTexture				TextureReflect;
	CTexture				TextureBump;
	D3DMATERIAL8			d3dMtl;
	DWORD					dwProperty;
	DWORD					dwAlpha;
//	TEXTURE_MAP_TYPE		texMapType;
	
	
///	BOOL					m_bSelfIllunum;			
//	BOOL					m_b2Side;
};

struct MTL_HANDLE
{
	DWORD					dwRefCount;
	void*					pSearchHandle;
	DWORD					dwIndex;
	DIMATERIAL				mtlData;
	
};
#define DIMATERIAL_OFFSET 12

struct MATERIAL_SET
{
	DWORD					dwMtlNum;	// √÷¥Î¿Œµ¶Ω∫+1
	DIMATERIAL**			ppMtl;
};
class CMaterialManager  
{
	CoD3DDevice*			m_pRenderer;
	STMPOOL_HANDLE			m_pMtlMemoryPool;
	VBHASH_HANDLE			m_pMtlHash;
	INDEXCR_HANDLE			m_pICMtl;
	INDEXCR_HANDLE			m_pICMtlSet;

	MTL_HANDLE**			m_ppMtlHandleList;
	MATERIAL_SET*			m_pMtlSetList;
	MTL_HANDLE				m_defaultMtlHandle;
	DIMATERIAL*				m_pDefaultMtlHandle;


	DWORD					m_dwMaxMtlSetNum;
	DWORD					m_dwMaxMtlNum;
	DWORD					m_dwMtlNum;
	
	
	

public:
	DIMATERIAL*				GetDefaultMaterial()	{ return m_pDefaultMtlHandle;}
	DIMATERIAL*				GetMaterial(DWORD dwMtlSetIndex,DWORD dwMtlIndex);

	void					SetMaterial(DIMATERIAL* pMtlResult,MATERIAL* pMtlSrc);
	BOOL					Initialize(CoD3DDevice* pRenderer,DWORD dwMaxMtlNum,DWORD dwMaxMtlSetNum,DWORD dwFlag);

	DWORD					CreateMaterialSet(MATERIAL_TABLE* pMtlEntry,DWORD dwNum);
	void					DeleteMaterialSet(DWORD dwMtlSetIndex);

	MTL_HANDLE*				Add(DWORD* pdwIndexResult,MATERIAL* pMtl,DWORD dwFlag);
	DIMATERIAL*				Alloc(MATERIAL* pMtl,DWORD* pdwWidth,DWORD* pdwHeight,DWORD dwFlag);
	void					Free(DIMATERIAL* pMtl);
	void					SetMaterialTextureBorder(DIMATERIAL* pMtl,DWORD dwColor);
	
	
	CMaterialManager();
	~CMaterialManager();

};

#endif // !defined(AFX_MATERIALMANAGER_H__EBBC1A17_319C_4649_8DD5_6195DF1C8EDA__INCLUDED_)
