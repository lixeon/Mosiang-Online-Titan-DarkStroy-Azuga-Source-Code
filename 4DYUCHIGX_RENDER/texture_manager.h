#pragma once

#include "../4DyuchiGRX_common/typedef.h"
#include "d3dtexture.h"
#include "../4DyuchiGXGFunc/global.h"

#include "CTexture.h"


class CD3DResourceManager;
class CoD3DDevice;




class CTextureManager
{
	CoD3DDevice*					m_pRenderer;
	CD3DResourceManager*			m_pResourceManager;
	
	VBHASH_HANDLE					m_pFileNameHash;
	DWORD							m_dwMaxTextureNum;
	DWORD							m_dwTextureNum;
	DWORD							m_dwFileTextureNum;
	

	DWORD							m_dwBytesPerPixelTGA;
	DWORD							m_dwBytesPerPixelTIF;
	D3DFORMAT						m_d3dfmtTGA;
	D3DFORMAT						m_d3dfmtTIF;
	CTexture						m_DefaultDiffuseTexture;
	CBaseTextureContainer*			m_pDefaultDiffuseTexContainer;
	CTexture						m_DefaultBumpTexture;
	CBaseTextureContainer*			m_pDefaultBumpTexContainer;
	
	

	CTextureContainer*				AddTexture(char* szFileName,DWORD dwUsage,D3DPOOL pool,DWORD dwWidthHeight,BOOL bUseMipmap,BOOL bCompress,TEXTURE_MAP_TYPE texType);
	
	void							ReleaseAllTextures();
	void							ResourceCheck();

	//trustpak 2005/04/04

	TEXTURE_TABLE*					m_pLoadFailedTextureTable;
	DWORD							m_dwLoadFailedTextureTableSize;

	DWORD							m_dwFailedTextureCount;
	///


public:

	CD3DResourceManager*			GetD3DResourceManager()	{return m_pResourceManager;}

	void							SetDefaultDiffuseTexture(LPDIRECT3DDEVICE8 pDevice,DWORD dwTexStageIndex) 
	{
		m_DefaultDiffuseTexture.SetTexture(pDevice,dwTexStageIndex);
	}
	void							SetDefaultBumpTexture(LPDIRECT3DDEVICE8 pDevice,DWORD dwTexStageIndex) 
	{
		m_DefaultBumpTexture.SetTexture(pDevice,dwTexStageIndex);
	}
	void							SetBytesPerPixel(DWORD dwBPS);
	DWORD							GetTextureNum()					{return m_dwTextureNum;}
	DWORD							GetFileTextureNum()				{return m_dwFileTextureNum;}
	
	BOOL							Initialize(CoD3DDevice* pRenderer,CD3DResourceManager* pResourceManager,DWORD dwMaxTexNum,DWORD dwMaxBucketNum);
	BOOL							AllocTexture(CTexture* pTextureHandle,char* szFileName,DWORD dwWidthHeight,BOOL bUseMipmap,BOOL bCompress,TEXTURE_MAP_TYPE texType);
	void							AllocDefaultDiffuseTexture(CTexture* pTexHandle);
	void							AllocDefaultBumpTexture(CTexture* pTexHandle);
	BOOL							AllocRawTexture(CTexture* pTextureHandle,UINT Width,UINT Height,UINT  Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool);
	void							FreeTexture(CBaseTextureContainer* pTexContainer);


	//trustpak 2005/04/04

	BOOL							SetLoadFailedTextureTable(TEXTURE_TABLE* pLoadFailedTextureTable, DWORD dwLoadFailedTextureTableSize);
	VOID							GetLoadFailedTextureTable(TEXTURE_TABLE** ppoutLoadFailedTextureTable, DWORD* poutdwLoadFailedTextureTableSize, DWORD* poutdwFailedTextureCount);

	///
	
	CTextureManager();
	~CTextureManager();
	
	

};
