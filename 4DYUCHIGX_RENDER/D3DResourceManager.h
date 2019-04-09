// D3DResourceManager.h: interface for the CD3DResourceManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_D3DRESOURCEMANAGER_H__321C49A9_8451_4890_B663_4116BFEEF0C7__INCLUDED_)
#define AFX_D3DRESOURCEMANAGER_H__321C49A9_8451_4890_B663_4116BFEEF0C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <d3d8.h>
#include <d3dx8.h>
#include "../4DyuchiGXGFunc/global.h"

class CoD3DDevice;

enum D3DRESOURCE_TYPE
{
	D3DRESOURCE_TYPE_TEXTURE		= 0x00000001,
	D3DRESOURCE_TYPE_VERTEXBUFFER	= 0x00000002,
	D3DRESOURCE_TYPE_INDEXBUFFER	= 0x00000004,
	D3DRESOURCE_TYPE_D3DXFONT		= 0x00000008
};

struct D3DRESOURCE_DESC
{
	IDirect3DResource8*		pResource;
	D3DRESOURCE_TYPE		type;
	D3DPOOL					pool;
	void*					pHashHandle;
};

class CD3DResourceManager  
{
	CoD3DDevice*				m_pRenderer;
	IDirect3DDevice8*			m_pD3DDevice;
	DWORD						m_dwMaxItemNum;
	DWORD						m_dwItemNum;

	QBHASH_HANDLE				m_pHashResource;
	STMPOOL_HANDLE				m_pRCDescPool;

	
	
	

public:
	CoD3DDevice*				GetRenderer() {return m_pRenderer;}

	BOOL			Initialize(CoD3DDevice* pRenderer,IDirect3DDevice8* pDevice,DWORD dwMaxItemNum);
	HRESULT			CreateVertexBuffer(
								UINT Length,
								DWORD Usage,
								DWORD FVF,
								D3DPOOL Pool,
								IDirect3DVertexBuffer8** ppVertexBuffer
								);

	HRESULT			CreateIndexBuffer(
								UINT Length,
								DWORD Usage,
								D3DFORMAT Format,
								D3DPOOL Pool,
								IDirect3DIndexBuffer8** ppIndexBuffer
								);


	HRESULT			CreateTexture(
								UINT Width,
								UINT Height,
								UINT  Levels,
								DWORD Usage,
								D3DFORMAT Format,
								D3DPOOL Pool,
								IDirect3DTexture8** ppTexture
								);

	HRESULT			CreateFont(
								CONST LOGFONT* pLogFont,
								LPD3DXFONT* ppFont
								);


	BOOL	DuplicateTexture(LPDIRECT3DTEXTURE8* ppTexDest,D3DPOOL pool,LPDIRECT3DTEXTURE8 pTexSrc,DWORD Flag);
	BOOL	CreateTextureWithBitmap(LPDIRECT3DTEXTURE8* ppTex,DWORD dwUsage,D3DFORMAT format,D3DPOOL pool,char* pBits,DWORD dwWidth,DWORD dwHeight,BOOL bUseMipmap,BOOL bCompress);
	BOOL	CreateBumpMap(LPDIRECT3DTEXTURE8* ppBumpMap,LPDIRECT3DTEXTURE8 pSrc);
	BOOL	SetSurfaceWithBitmap(DWORD dwSurfIndex,LPDIRECT3DTEXTURE8 pTex,char* pBits,DWORD dwWidth,DWORD dwHeight,D3DFORMAT format);

	
	BOOL	CreateBitmapWithSurface(char** ppBits,DWORD* pdwWidth,DWORD* pdwHeight,DWORD* pdwBytesPerPixel,IDirect3DSurface8* pSurface);
	void	ReleaseBitmapWithSurface(char* pBits);

	HRESULT	Release(IUnknown* pResource);
#ifdef _DEBUG
	void	Check();
#endif
	BOOL	IsValid(IUnknown* pResource);
	void	OnLostFont();
	void	OnResetFont();
	BOOL	ConvertCompressedTexture(char* szFileName,DWORD dwFlag);
	BOOL	CreateTextureWithDDS(IDirect3DTexture8** ppTexture,char* szFileName);
	

	BOOL	CopyTexture(LPDIRECT3DTEXTURE8 pTexDest,LPDIRECT3DTEXTURE8 pTexSrc);
	BOOL	SetTextureBorder(LPDIRECT3DTEXTURE8 pTex,DWORD dwBorderColor);
	
	CD3DResourceManager();
	~CD3DResourceManager();

};

#endif // !defined(AFX_D3DRESOURCEMANAGER_H__321C49A9_8451_4890_B663_4116BFEEF0C7__INCLUDED_)
