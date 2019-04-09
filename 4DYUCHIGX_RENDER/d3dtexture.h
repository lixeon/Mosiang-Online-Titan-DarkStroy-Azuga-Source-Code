#pragma once

#include <d3d8.h>
#include "renderer_typedef.h"
#include "../4DyuchiGRX_common/typedef.h"

class CTextureManager;
/*
class TEXTURE_STATUS
{
protected:
	DWORD						m_dwTexFlag;

public:
	

	DWORD						GetTexFlag()	{return m_dwTexFlag;}
};
*/



class CBaseTextureContainer
{
protected:
	friend class CTextureManager;
	DWORD						m_dwIndexInSortTable;
	
	LPDIRECT3DTEXTURE8			m_pD3DTexture;
	IMAGE_HEADER				m_ImageHeader;
	CTextureManager*			m_pTextureManager;
	
	LPDIRECT3DTEXTURE8			m_pBackupTexture;
	/*
	D3DSURFACE_DESC				m_ddsc;
	BYTE*						m_pRawData;
	DWORD						m_dwRawSize;
	DWORD						m_dwMipLevel;
	*/
	
	BOOL						m_bFromFile;
	
	
	DWORD						m_dwRefCount;
	DWORD						m_dwTexFlag;
	void*						m_pSearchHandle;

	DWORD						m_dwPageFlag;
	

public:
	BOOL						SwapToSystemMemory();
	BOOL						SwapToDriverMemory();
	CTextureManager*			GetTextureManager() {return m_pTextureManager;}

	void						SetTexFlag(DWORD dwFlag) {m_dwTexFlag = dwFlag;}
	DWORD						GetTexFlag() {return m_dwTexFlag;}
	IMAGE_HEADER*				GetImageHeader() {return &m_ImageHeader;}
	

	DWORD						AddRef();
	DWORD						Release();
	
	LPDIRECT3DTEXTURE8			GetTexture();
	void						SetTexture(LPDIRECT3DDEVICE8 pDevice,DWORD dwTexStageIndex);

	void						SetTextureImage(LPDIRECT3DTEXTURE8 pTex,IMAGE_HEADER* pHeader)
	{
		m_pD3DTexture = pTex;
		m_ImageHeader = *pHeader;
	}
	BOOL						CreateImage(IMAGE_HEADER* pHeader,char** ppBits);
	void						ReleaseImage(char* pBits);
	BOOL						IsFromFile() {return m_bFromFile;}

	CBaseTextureContainer();
	~CBaseTextureContainer();
};	
class CTextureContainer : public CBaseTextureContainer
{
	friend class CTextureManager;

/*
	LPDIRECT3DTEXTURE8			m_pD3DTexture;
	IMAGE_HEADER				m_ImageHeader;
	CTextureManager*			m_pTextureManager;
	
	DWORD						m_dwIndex;
	DWORD						m_dwRefCount;
	DWORD						m_dwTexFlag;
	void*						m_pSearchHandle;
*/	
	char						m_szFileName[MAX_NAME_LEN];
public:
	char*						GetFileName() {return m_szFileName;}
	void						SetFileName(char* szFileName) 
	{
		m_bFromFile = TRUE;
		lstrcpy(m_szFileName,szFileName);
	}


	//	TEXTURE_STATUS*				GetTextureStatus() {return (TEXTURE_STATUS*)&m_dwTexFlag;}
	CTextureContainer();
	
};
