#pragma once

#include "../4DyuchiGRX_common/typedef.h"
#include "d3dtexture.h"

class CTextureManager;

class CTexture 
{
	CBaseTextureContainer*		m_pTexContainer;
	DWORD						dwSuccessMask;
	DWORD						m_dwTexFlag;
	
public:
	void						SetTextureInfo(CBaseTextureContainer* pContainer);
	void						SetTexture(LPDIRECT3DDEVICE8 pDevice,DWORD dwTexStageIndex);
	DWORD						GetTexFlag() {return m_dwTexFlag;}
	BOOL						HasTexture()	{return dwSuccessMask;}
	LPDIRECT3DTEXTURE8			GetTexture();
	IMAGE_HEADER*				GetImageHeader() {return (IMAGE_HEADER*)( dwSuccessMask & (DWORD)(m_pTexContainer->GetImageHeader()) );}
	CBaseTextureContainer*		GetTexContainer() {return m_pTexContainer;}
	

	void						Release();
	CTexture();
	~CTexture();
};


struct VLMESH_EFFECT_DESC
{
	BOOL			bDisableSrcTex;
	TEXGEN_METHOD	method;
	CTexture		m_texture;
	BOOL			bSuccess;
	DWORD			dwFlag;
};