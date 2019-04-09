#pragma once

#include "../4DyuchiGRX_common/IRenderer.h"
#include "../4DyuchiGXGFunc/global.h"
#include "d3dtexture.h"
#include "FontHash.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

struct TEXTURE_BLOCK_DESC;

#define MAX_SEARCH_TABLE_NUM	10
#define MIN_FONT_HEIGHT			8
#define MAX_STRING_BYTES_NUM	256

struct STRING_BUFFER_DESC;
struct TEXTURE_BUFFER_DESC;

struct TEXTURE_LINE_DESC
{
	WORD					wHeight;				// 이것이 key

	WORD					wAvailableWidth;
	WORD					wCurrentPosX;
	WORD					wPosY;
	LPDIRECT3DTEXTURE8		pTexture;
	ITEM_CONTAINER_AE*		pSearchHandle;

	TEXTURE_BUFFER_DESC*	pBufferDesc;
	TEXTURE_LINE_DESC*		pNext;



};
struct TEXTURE_BLOCK_DESC
{
	WORD						wPosX;
	WORD						wPosY;
	WORD						wWidth;
	WORD						wHeight;
	LPDIRECT3DTEXTURE8			pTexture;
	RECT						rect;
	WORD						wVBWidth;
	WORD						wVBHeight;
	
	FONTCACHE_ITEM_CONTAINER*	pHashHandle;
	TEXTURE_BUFFER_DESC*		pBufferDesc;
	TEXTURE_BLOCK_DESC*			pNext;


};

struct TEXTURE_BUFFER_DESC
{
	LPDIRECT3DTEXTURE8			pTexture;
	int							iPosY;
	int							iAvailableHeight;
	TEXTURE_LINE_DESC*			pLineDesc;
	TEXTURE_BUFFER_DESC*		pNext;
	TEXTURE_BLOCK_DESC*			pBlockDesc;
};

class CoD3DDevice;
class CFontCache
{
	BSAE_HANDLE					m_pLineDescSearch;
	CFontHash					m_fontHash;
	HBRUSH						m_hBrush;
	STMPOOL_HANDLE				m_pBlockDescPool;
	STMPOOL_HANDLE				m_pLineDescPool;
	
	TEXTURE_BUFFER_DESC*		m_pTextureBufferDesc;	// 주 포인터
	TEXTURE_BUFFER_DESC*		m_pPrimaryBufferDesc;	// 현재 할당받을 버퍼 포인터
	




	DWORD					m_dwTextureBufferNum;
	DWORD					m_dwAllocateLineDescNum;
	DWORD					m_dwMaxLineDescNum;
	DWORD					m_dwMaxTextureBlockDescNum;
	DWORD					m_dwMaxCacheItemNum;


	CoD3DDevice*			m_pRenderer;
	LPDIRECT3DDEVICE8		m_pD3DDevice;

	DWORD					m_dwTexWidth;
	DWORD					m_dwTexHeight;
	BYTE*					m_pImageBuffer;
		
	VECTOR4					m_v4Rect[4];
	
	

	CVertexBuffer		m_VB;
	CIndexBuffer		m_IB;
		
	HDC					m_hMemDC;
	HBITMAP				m_hBitmap;
	HDC					m_hDC;

	TEXTURE_BLOCK_DESC*		AllocTextureBlock(DWORD dwWidth,DWORD dwHeight);
	void					ChangeTextureBuffer();
	TEXTURE_LINE_DESC*		AllocLineDesc(DWORD dwHeight);
	HBITMAP		CreateBitmapFromText(int* piWidth,int* piHeight,HFONT hFont, char* szString,DWORD dwLen,RECT* pRect, DWORD dwTextColor,CHAR_CODE_TYPE type,DWORD dwFontHeight,HBITMAP* pOldBitmap);
public:
	DWORD		GetMaxWidth() {return m_dwTexWidth;}
	DWORD		GetMaxHeight() {return m_dwTexHeight;}
	BOOL		Initialize(CoD3DDevice* pRenderer,DWORD dwMaxCacheItemNum,DWORD dwTexWidth,DWORD dwTexHeight,DWORD dwTexNum);
	BOOL		GetResource(LPDIRECT3DTEXTURE8* ppTex,CVertexBuffer** ppVB,CIndexBuffer** ppIB,RECT* pRect,FONT_PROPERTY_DESC* pProperty);
	
	CFontCache();
	~CFontCache();

};
