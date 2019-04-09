#include "FontCache.h"
#include "CoD3DDevice.h"
#include "global_variable.h"
#include "VBHeap.h"
#include "IBHeap.h"



//----------------------------------------------------------------------------
// Name : CFont::DDBToDIB
// Description :
//---------------------------------------------------------------------------
BOOL DDBToDIB(HDC hDC,HBITMAP hBitmap, BYTE* pOut)
{

	

	// Spectify DIB properties.

	BITMAP bitmap;
	GetObject(hBitmap, sizeof(BITMAP), &bitmap);

	BITMAPINFOHEADER bih;
	
	bih.biClrImportant	= 0;
	bih.biClrUsed		= 0;			
	bih.biXPelsPerMeter	= 0;
	bih.biYPelsPerMeter	= 0;
	bih.biSize			= sizeof(BITMAPINFOHEADER);
	bih.biWidth			= bitmap.bmWidth;
	bih.biHeight		= bitmap.bmHeight;
	bih.biPlanes		= 1;
	bih.biBitCount		= 16;
	bih.biCompression	= BI_RGB;	

	// Get DIB Size.

//	bih.biSizeImage = ((((bih.biWidth * bih.biBitCount) + 31) & ~31) >> 3) * bih.biHeight;
	
	bih.biSizeImage = (bih.biWidth<<1) * bih.biHeight;

//	int iWidth = ((bih.biWidth +0x00000001) & 0xfffffffe);
//	bih.biSizeImage = iWidth * 2 * bih.biHeight;
	
	
	// Get Pixel data.

	BITMAPINFO bif;
	bif.bmiHeader = bih;

	GetDIBits(hDC, hBitmap, 0, bitmap.bmHeight, pOut, &bif, DIB_RGB_COLORS);


	return TRUE;
}
CFontCache::CFontCache()
{
	memset(this,0,sizeof(CFontCache));
}
BOOL CFontCache::Initialize(CoD3DDevice* pRenderer,DWORD dwMaxCacheItemNum,DWORD dwTexWidth,DWORD dwTexHeight,DWORD dwTexNum)
{
	BOOL		bResult = FALSE;
	m_pRenderer = pRenderer;
	m_pD3DDevice = pRenderer->GetDevice();
		
	WORD	index[6];
	index[0] = 0;
	index[1] = 1;
	index[2] = 2;

	index[3] = 1;
	index[4] = 3;
	index[5] = 2;

	
	DWORD	dwErrorCode;
	if (!m_pRenderer->GetVBSpriteHeap()->AllocVB(&dwErrorCode,&m_VB,4,NULL,&m_VB.m_bEnable,NULL))
		__asm int 3

	if (!m_pRenderer->GetIBHeapSprite()->AllocIB(&dwErrorCode,&m_IB,6,NULL,&m_IB.m_bEnable,NULL))
		__asm int 3

	m_IB.CheckValid();
	m_IB.SetIndexList(index,6);

	m_hDC = GetDC(NULL);
	m_hMemDC = CreateCompatibleDC(m_hDC);
	
	
	m_hBrush = CreateSolidBrush(0xff000000);	
	

	m_pImageBuffer = new BYTE[dwTexWidth*dwTexHeight*2];
	memset(m_pImageBuffer,0,dwTexWidth*dwTexHeight*2);



	m_dwTexWidth = dwTexWidth;
	m_dwTexHeight = dwTexHeight;

	m_dwTextureBufferNum = dwTexNum;
	m_dwMaxCacheItemNum = dwMaxCacheItemNum;

	m_pTextureBufferDesc = new TEXTURE_BUFFER_DESC;

	TEXTURE_BUFFER_DESC*	pBufferDesc = m_pTextureBufferDesc;
	memset(pBufferDesc,0,sizeof(TEXTURE_BUFFER_DESC));
	for(int i=0; i<(int)m_dwTextureBufferNum-1; i++)
	{

		pBufferDesc->pNext = new TEXTURE_BUFFER_DESC;
		pBufferDesc = pBufferDesc->pNext;
		memset(pBufferDesc,0,sizeof(TEXTURE_BUFFER_DESC));
	}
	pBufferDesc->pNext = m_pTextureBufferDesc;
	for (i=0; i<(int)m_dwTextureBufferNum; i++)
	{
		HRESULT		hr = m_pRenderer->GetD3DResourceManager()->CreateTexture(
			dwTexWidth,
			dwTexHeight,
			1,
			0,
			D3DFMT_A1R5G5B5,
			D3DPOOL_MANAGED,
			&pBufferDesc->pTexture);
	
		if (D3D_OK != hr)
		{
			OutputErrorMsg(m_pRenderer,hr,"CreateFontCache");
			goto lb_return;
		}
			
		pBufferDesc = pBufferDesc->pNext;
	}


	m_pPrimaryBufferDesc = m_pTextureBufferDesc;
	m_pPrimaryBufferDesc->iAvailableHeight= (int)m_dwTexHeight;
	m_pPrimaryBufferDesc->iPosY = 0;
	m_pPrimaryBufferDesc->pBlockDesc = NULL;
	m_pPrimaryBufferDesc->pLineDesc = NULL;
	
	

	
	
	m_dwMaxLineDescNum = m_dwTexHeight / MIN_FONT_HEIGHT * m_dwTextureBufferNum;
	m_dwMaxTextureBlockDescNum = m_dwMaxCacheItemNum;

	m_pBlockDescPool = CreateStaticMemoryPool();
	InitializeStaticMemoryPool(m_pBlockDescPool,sizeof(TEXTURE_BLOCK_DESC),m_dwMaxTextureBlockDescNum/8,m_dwMaxTextureBlockDescNum);

	m_pLineDescPool = CreateStaticMemoryPool();
	InitializeStaticMemoryPool(m_pLineDescPool,sizeof(TEXTURE_LINE_DESC),m_dwMaxLineDescNum/8,m_dwMaxLineDescNum);

	m_pLineDescSearch = BSAECreate();
	BSAEInitialize(m_pLineDescSearch,m_dwMaxLineDescNum);
	m_fontHash.Initialize(m_dwMaxCacheItemNum,MAX_STRING_BYTES_NUM);

	for (i=0; i<4; i++)
	{
		m_v4Rect[i].z = 0.1f;
		m_v4Rect[i].w = 1.0f;
	
	}

	bResult = TRUE;
lb_return:
	return bResult;
}
BOOL CFontCache::GetResource(LPDIRECT3DTEXTURE8* ppTex,CVertexBuffer** ppVB,CIndexBuffer** ppIB,RECT* pRect,FONT_PROPERTY_DESC* pProperty)
{

	int		iWidth;
	int		iHeight;
	*ppVB = &m_VB;
	*ppIB = &m_IB;
	

	DWORD	dwBytesPerChar;
	if (pProperty->type == CHAR_CODE_TYPE_UNICODE)
		dwBytesPerChar = 2;
	else
		dwBytesPerChar = 1;

#ifdef _DEBUG
	if (!pProperty->dwStrLen)
	{
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_PARAMETER_INVALID,0,(void*)dwAddr,"string length is zero");
	}
#endif

	if (pProperty->dwStrLen > MAX_STRING_BYTES_NUM / dwBytesPerChar)
		pProperty->dwStrLen = MAX_STRING_BYTES_NUM / dwBytesPerChar;
	

	// 해쉬를 뒤져서 찾는다..
	// 찾으면 리턴..



	

	DWORD dwBytesNum = dwBytesPerChar*pProperty->dwStrLen;
	
	TEXTURE_BLOCK_DESC* pBlockDesc;

	pBlockDesc = (TEXTURE_BLOCK_DESC*)m_fontHash.SearchItem(pProperty->pszString,pProperty->dwStrLen,pProperty->hFont,pProperty->dwColor);
	
	if (pBlockDesc)
	{
		goto lb_set_vb;
	}
	
	HBITMAP	hOldBitmap;
	// 못찾으면...
	// 비트맵을 만들고...
	HBITMAP	hBitmap;

//	RECT	rectTemp;
//	rectTemp = *pRect;


	hBitmap = CreateBitmapFromText(
		&iWidth,&iHeight,
		pProperty->hFont,
		pProperty->pszString,pProperty->dwStrLen,
		pRect,
		pProperty->dwColor,
		pProperty->type,
		pProperty->iHeight,
		&hOldBitmap);


//	if (iWidth <= 0)
//		__asm int 3
/*
	BOOL BitBlt(
  HDC hdcDest, // handle to destination DC
  int nXDest,  // x-coord of destination upper-left corner
  int nYDest,  // y-coord of destination upper-left corner
  int nWidth,  // width of destination rectangle
  int nHeight, // height of destination rectangle
  HDC hdcSrc,  // handle to source DC
  int nXSrc,   // x-coordinate of source upper-left corner
  int nYSrc,   // y-coordinate of source upper-left corner
  DWORD dwRop  // raster operation code
);
*/

	DDBToDIB(m_hMemDC,hBitmap,m_pImageBuffer);

	SelectObject(m_hMemDC,hOldBitmap);
	DeleteObject(hBitmap);

	if (iWidth > (int)m_dwTexWidth)
		iWidth = (int)m_dwTexWidth;

	if (iHeight > (int)m_dwTexHeight)
		iHeight = (int)m_dwTexHeight;
	


	// 텍스쳐블럭을 할당받는다..
	pBlockDesc = AllocTextureBlock(iWidth,iHeight);
	pBlockDesc->pHashHandle = m_fontHash.InsertItem(pProperty->pszString,pProperty->dwStrLen,pProperty->hFont,pProperty->dwColor,pBlockDesc);
	

	pBlockDesc->rect.left = pBlockDesc->wPosX;
	pBlockDesc->rect.right = pBlockDesc->wPosX + pBlockDesc->wWidth;
	pBlockDesc->rect.top = pBlockDesc->wPosY;
	pBlockDesc->rect.bottom = pBlockDesc->wPosY + pBlockDesc->wHeight;
	

	pBlockDesc->wVBWidth = pRect->right - pRect->left;
	pBlockDesc->wVBHeight = pRect->bottom - pRect->top;

	
	
	D3DLOCKED_RECT	lockRect;

	LPDIRECT3DTEXTURE8		pTexture;
	pTexture = pBlockDesc->pTexture;
	pTexture->LockRect(0,&lockRect,&pBlockDesc->rect,0);


//	WriteTGA("hoho.tga",(char*)m_pImageBuffer,m_dwTexWidth,m_dwTexHeight,iWidth*2,16);
	int		iImagePitch;
	iImagePitch = iWidth*2;

	char*	pSrc;
	pSrc = (char*)m_pImageBuffer;
	/*
	char*	pDest;
	pDest = (char*)lockRect.Pitch;


	DWORD	x,y;
	for (y=0; y<iHeight; y++)
	{
		for (x=0; x<iWidth; x++)
		{
			if (*(WORD*)pSrc == 0x0000)
				*(WORD*)pDest = (WORD)*pSrc & 0x7fff;
			else 
				*(WORD*)pDest = (WORD)*pSrc | 0x8000;

			pSrc += 2;
			pDest += 2;
			

		}
		pSrc += iImagePitch;
		
		pDest += lockRect.Pitch;
		pDest -= iImagePitch;
	}
*/
	
	__asm
	{
		mov		esi,dword ptr[pSrc]
		mov		edi,dword ptr[lockRect.pBits]
		
		mov		eax,dword ptr[iHeight]
		mov		ecx,dword ptr[iWidth]		; iWidth
		dec		eax							; iHeight-1
		mul		ecx
		shl		eax,1						; iWidth*(iHeight-1)*2
		add		esi,eax

		mov		eax,dword ptr[iHeight]		; iHeight

		xor		edx,edx

lb_height:
		mov		ecx,dword ptr[iWidth]
lb_width:
		xor		ebx,ebx
		mov		dx,word ptr[esi]

		cmp		dx,1
		sbb		ebx,ebx
		inc		ebx
		and		dx,0x7fff			; xRGB
		shl		ebx,15
		or		dx,bx

		mov		word ptr[edi],dx

		add		esi,2
		add		edi,2
		loop	lb_width


		dec		eax
		jz		lb_exit

		add		edi,dword ptr[lockRect.Pitch]
		sub		edi,dword ptr[iImagePitch]

		sub		esi,dword ptr[iImagePitch]
		sub		esi,dword ptr[iImagePitch]


		jmp		lb_height;
lb_exit:
	}
	pTexture->UnlockRect(0);

	pBlockDesc->rect.right = pBlockDesc->wPosX + iWidth;
	pBlockDesc->rect.bottom = pBlockDesc->wPosY + iHeight;


lb_set_vb:
	
	m_v4Rect[0].x = pRect->left;
	m_v4Rect[0].y = pRect->top;
	
	m_v4Rect[1].x = pRect->left + pBlockDesc->wVBWidth - 1;
	m_v4Rect[1].y = pRect->top;
	
	m_v4Rect[2].x = pRect->left;
	m_v4Rect[2].y = pRect->top + pBlockDesc->wVBHeight - 1;
	
	m_v4Rect[3].x = pRect->left + pBlockDesc->wVBWidth - 1;
	m_v4Rect[3].y = pRect->top + pBlockDesc->wVBHeight - 1;


	TVERTEX		tvRect[4];

/*
	u1 = (float)dwXPos / (float)pHeader->dwWidth;
	u2 = (float)(dwXPos+dwWidth) / (float)pHeader->dwWidth;

	v1 = (float)(pHeader->dwHeight - (dwYPos+dwHeight)) / (float)pHeader->dwHeight;
	v2 = (float)(pHeader->dwHeight - dwYPos) / (float)pHeader->dwHeight;
*/
	/*
	float	a = 1.0f / (float)m_dwTexHeight;


	tvRect[0].u = (float)pBlockDesc->rect.left	/ (float)m_dwTexWidth;
	tvRect[0].v = (float)pBlockDesc->rect.top	/ (float)m_dwTexHeight - a;

	tvRect[1].u = (float)pBlockDesc->rect.right / (float)m_dwTexWidth;
	tvRect[1].v = (float)pBlockDesc->rect.top	/ (float)m_dwTexHeight - a;

	tvRect[2].u = (float)pBlockDesc->rect.left	/ (float)m_dwTexWidth;
	tvRect[2].v = (float)pBlockDesc->rect.bottom / (float)m_dwTexHeight - a;

	tvRect[3].u = (float)pBlockDesc->rect.right / (float)m_dwTexWidth;
	tvRect[3].v = (float)pBlockDesc->rect.bottom / (float)m_dwTexHeight - a;
	
	*/

	

/*
	if (pBlockDesc->wVBHeight != 12)
		__asm nop

	if (texBlockRect.bottom - texBlockRect.top != 12)
		__asm nop

	if (pRect->bottom - pRect->top != 12)
		__asm nop*/


	tvRect[0].u = (float)(pBlockDesc->rect.left + 0.5f)	/ (float)m_dwTexWidth;
	tvRect[0].v = (float)(pBlockDesc->rect.top	+ 0.5f) / (float)m_dwTexHeight;

	tvRect[1].u = (float)(pBlockDesc->rect.right) / (float)(m_dwTexWidth);
	tvRect[1].v = (float)(pBlockDesc->rect.top + 0.5f) / (float)m_dwTexHeight;

	tvRect[2].u = (float)(pBlockDesc->rect.left + 0.5f)	/ (float)m_dwTexWidth;
	tvRect[2].v = (float)(pBlockDesc->rect.bottom) / (float)(m_dwTexHeight);

	tvRect[3].u = (float)(pBlockDesc->rect.right) / (float)(m_dwTexWidth);
	tvRect[3].v = (float)(pBlockDesc->rect.bottom) / (float)(m_dwTexHeight);
	

	m_VB.SetTexCoord(tvRect,TEX1_COORD_OFFSET_RHW_DIFFUSE,4);
	m_VB.SetTLVERTEXList(m_v4Rect,pProperty->dwColor,4);

	*ppTex = pBlockDesc->pTexture;

	return TRUE;
}


TEXTURE_BLOCK_DESC* CFontCache::AllocTextureBlock(DWORD dwWidth,DWORD dwHeight)
{
//	GLOBAL_FUNC_DLL void*			__stdcall	LALAlloc(STMPOOL_HANDLE pool);
//	GLOBAL_FUNC_DLL void			__stdcall	LALFree(STMPOOL_HANDLE pool,void* pMemory);

	// height가 맞는 라인 디스크립터부터 찾는다.

	// 검색
	TEXTURE_LINE_DESC*	pLineDesc;
	TEXTURE_BLOCK_DESC*	pBlockDesc = NULL;
	ITEM_CONTAINER_AE*	pCur;
//	dwHeight += 1;
	DWORD	dwKey = dwHeight;

lb_alloc_texblock:
	pBlockDesc = (TEXTURE_BLOCK_DESC*)LALAlloc(m_pBlockDescPool);
	if (!pBlockDesc)
	{
		ChangeTextureBuffer();
		goto lb_alloc_texblock;
	}

lb_begin_search:
	pCur = BSAESearchItemContainer(m_pLineDescSearch,dwKey);
	if (!pCur)
	{
		// 없으면 새로 할당...
		goto lb_alloc_linedesc;
	}
	

	if (pCur)
		dwKey = ((TEXTURE_LINE_DESC*)pCur->pItem)->wHeight + 1;

	while (pCur)
	{
		pLineDesc = (TEXTURE_LINE_DESC*)pCur->pItem;
		if (pLineDesc->wAvailableWidth >= dwWidth)
			goto lb_find_ok;

		pCur = pCur->pNext;
	}
lb_research:
//	dwKey = (DWORD)pLineDesc->wHeight + 1;
	goto lb_begin_search;

	// 그래도 없으면 새로 할당..	
lb_alloc_linedesc:
	pLineDesc = AllocLineDesc(dwHeight);

	pLineDesc->pSearchHandle = BSAEInsertItem(m_pLineDescSearch,pLineDesc->wHeight,pLineDesc);
	 
lb_find_ok:
	TEXTURE_BUFFER_DESC*	pBufferDesc;
	pBufferDesc = pLineDesc->pBufferDesc;

	

	pBlockDesc->pTexture = pLineDesc->pTexture;
	pBlockDesc->wHeight = pLineDesc->wHeight;
	pBlockDesc->wWidth = dwWidth;
	
	pBlockDesc->wPosX = pLineDesc->wCurrentPosX;
	pBlockDesc->wPosY = pLineDesc->wPosY;
	

	// 기존의 라인 디스크립터가 가지고 있던 버퍼 디스크립터 체인에 연결.한번에 지우기 위해서다.
	pBlockDesc->pBufferDesc = pBufferDesc;
	pBlockDesc->pNext = pBufferDesc->pBlockDesc;
	pBufferDesc->pBlockDesc = pBlockDesc;


	pLineDesc->wCurrentPosX += (WORD)dwWidth;		// 다음번 할당받을 X좌표를 옮긴다.
	pLineDesc->wAvailableWidth -= (WORD)dwWidth;	// 남은 폭을 계산한다.
lb_return:
	return pBlockDesc;
}
void CFontCache::ChangeTextureBuffer()
{

	/*
	char* pBits;
	IDirect3DSurface8*	pSurf;
	DWORD	dwWidth,dwHeight,dwBPP;

	m_pPrimaryBufferDesc->pTexture->GetSurfaceLevel(0,&pSurf);

	m_pRenderer->GetD3DResourceManager()->CreateBitmapWithSurface(&pBits,&dwWidth,&dwHeight,&dwBPP,pSurf);
	WriteTGA("hoho.tga",(char*)pBits,dwWidth,dwHeight,dwWidth*2,16);
	m_pRenderer->GetD3DResourceManager()->ReleaseBitmapWithSurface(pBits);
	pSurf->Release();
*/
	TEXTURE_BUFFER_DESC*	pBackBufferDesc;

	pBackBufferDesc = m_pPrimaryBufferDesc->pNext;
	TEXTURE_LINE_DESC*	pNextLineDesc;
	TEXTURE_LINE_DESC*	pCurLineDesc = pBackBufferDesc->pLineDesc;
	while (pCurLineDesc)
	{
		pNextLineDesc = pCurLineDesc->pNext;
		BSAEDeleteItem(m_pLineDescSearch,pCurLineDesc->pSearchHandle);

		LALFree(m_pLineDescPool,pCurLineDesc);
		pCurLineDesc = pNextLineDesc;
	}
	TEXTURE_BLOCK_DESC*	pNextBlockDesc;
	TEXTURE_BLOCK_DESC*	pCurBlockDesc = pBackBufferDesc->pBlockDesc;
	while (pCurBlockDesc)
	{
		pNextBlockDesc = pCurBlockDesc->pNext;
		
		// 블럭디스크럽터를해쉬에서 제거.
		m_fontHash.DeleteItem(pCurBlockDesc->pHashHandle);
		//
		//////////////////////////////
	
		LALFree(m_pBlockDescPool,pCurBlockDesc);
		pCurBlockDesc = pNextBlockDesc;

	}
	m_pPrimaryBufferDesc = pBackBufferDesc;
	m_pPrimaryBufferDesc->iAvailableHeight= (int)m_dwTexHeight;
	m_pPrimaryBufferDesc->iPosY = 0;
	m_pPrimaryBufferDesc->pBlockDesc = NULL;
	m_pPrimaryBufferDesc->pLineDesc = NULL;
}
TEXTURE_LINE_DESC* CFontCache::AllocLineDesc(DWORD dwHeight)
{
	TEXTURE_LINE_DESC*		pLineDesc;
//	TEXTURE_BUFFER_DESC*	pBackBufferDesc;
	
	int	iReservedHeight = m_pPrimaryBufferDesc->iAvailableHeight - (int)dwHeight;
	if (iReservedHeight < 0)
	{
		ChangeTextureBuffer();

	}
	
	pLineDesc = (TEXTURE_LINE_DESC*)LALAlloc(m_pLineDescPool);

	pLineDesc->pBufferDesc = m_pPrimaryBufferDesc;
	pLineDesc->pNext = m_pPrimaryBufferDesc->pLineDesc;
	m_pPrimaryBufferDesc->pLineDesc = pLineDesc;
	
	

	pLineDesc->pTexture = m_pPrimaryBufferDesc->pTexture;

	pLineDesc->wAvailableWidth = m_dwTexWidth;
	pLineDesc->wCurrentPosX = 0;
	pLineDesc->wHeight = dwHeight;
	pLineDesc->wPosY = m_pPrimaryBufferDesc->iPosY;
	
	m_pPrimaryBufferDesc->iPosY += (int)dwHeight;
	m_pPrimaryBufferDesc->iAvailableHeight -= (int)dwHeight;

lb_return:
	return pLineDesc;
}
//----------------------------------------------------------------------------
// Name : CFont::CreateBitmapFromText
// Description :
//---------------------------------------------------------------------------

HBITMAP	CFontCache::CreateBitmapFromText(int* piWidth,int* piHeight,HFONT hFont, char* szString,DWORD dwLen,RECT* pRect, DWORD dwTextColor,CHAR_CODE_TYPE type,DWORD dwFontHeight,HBITMAP* pOldBitmap )
{
	BYTE dwRed		= (BYTE)((dwTextColor	& 0xFF0000) >> 16);
	BYTE dwGreen	= (BYTE)((dwTextColor	& 0xFF00) >> 8);
	BYTE dwBlue		= (BYTE)((dwTextColor	& 0xFF));

	COLORREF TextColor = RGB(dwRed, dwGreen, dwBlue);	

	RECT	rect;

	*piWidth = pRect->right - pRect->left;
	*piHeight = pRect->bottom - pRect->top;

	if (*piHeight < dwFontHeight)
	{
		*piHeight = dwFontHeight;
	}

	*piWidth = ((*piWidth + 0x00000001) & 0xfffffffe);

	pRect->right = pRect->left + *piWidth;
	rect.right = *piWidth+10;
	rect.bottom = *piHeight+10;

	
	rect.left = 0;
	rect.top = 0;
	

	HBITMAP hBitmap = CreateCompatibleBitmap(m_hDC,*piWidth,*piHeight);

	HFONT hOldFont;
	hOldFont = (HFONT)SelectObject(m_hMemDC, hFont);

	SetTextColor(m_hMemDC, TextColor);
	SetBkColor(m_hMemDC,0x00000000);
	
	SetBkMode(m_hMemDC, OPAQUE);
	HBITMAP hOldBitmap;
	hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, hBitmap);


	

	FillRect(m_hMemDC,&rect,m_hBrush);

	



	if (type == CHAR_CODE_TYPE_UNICODE)
		DrawTextW(m_hMemDC,(WORD*)szString,dwLen,&rect,DT_NOPREFIX);
	else
		DrawTextA(m_hMemDC,szString,dwLen,&rect,DT_NOPREFIX);

	SelectObject(m_hMemDC,hOldFont);
	*pOldBitmap = hOldBitmap;
	

	return hBitmap;
}
CFontCache::~CFontCache()
{
	TEXTURE_BUFFER_DESC*	pBufferDesc = m_pTextureBufferDesc;
	TEXTURE_BUFFER_DESC*	pNext;
	for (int i=0; i<m_dwTextureBufferNum; i++)
	{
		pNext = pBufferDesc->pNext;
		m_pRenderer->GetD3DResourceManager()->Release(pBufferDesc->pTexture);
		delete pBufferDesc;
		pBufferDesc = pNext;
	}
	if (m_pImageBuffer)
	{
		delete [] m_pImageBuffer;
		m_pImageBuffer = NULL;
	}
	if (m_hBrush)
	{
		DeleteObject(m_hBrush);
		m_hBrush = NULL;
	}

	if (m_hMemDC)
	{
		DeleteDC(m_hMemDC);
		m_hMemDC = NULL;
	}
	if (m_hDC)
	{
		ReleaseDC(NULL,m_hDC);
		m_hDC = NULL;

	}
	ReleaseStaticMemoryPool(m_pBlockDescPool);
	ReleaseStaticMemoryPool(m_pLineDescPool);

	BSAERelease(m_pLineDescSearch);
	m_pLineDescSearch = NULL;
}

