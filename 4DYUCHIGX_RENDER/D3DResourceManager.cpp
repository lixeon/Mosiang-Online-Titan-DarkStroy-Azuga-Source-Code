// D3DResourceManager.cpp: implementation of the CD3DResourceManager class.
//
//////////////////////////////////////////////////////////////////////

#include "D3DResourceManager.h"
#include "CoD3DDevice.h"
#include "d3d_helper.h"
#include <d3dx8.h>
#include <ddraw.h>
#include "global_variable.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


struct DXTColBlock
{
	WORD col0;
	WORD col1;

	// no bit fields - use bytes
	BYTE row[4];
};

struct DXTAlphaBlockExplicit
{
	WORD row[4];
};

struct DXTAlphaBlock3BitLinear
{
	BYTE alpha0;
	BYTE alpha1;

	BYTE stuff[6];
};



// use cast to struct instead of RGBA_MAKE as struct is
//  much
struct Color8888
{
	BYTE r;		// change the order of names to change the 
	BYTE g;		//  order of the output ARGB or BGRA, etc...
	BYTE b;		//  Last one is MSB, 1st is LSB.
	BYTE a;
};


struct Color565
{
	unsigned nBlue  : 5;		// order of names changes
	unsigned nGreen : 6;		//  byte order of output to 32 bit
	unsigned nRed	: 5;
};

void GetColorBlockColors( DXTColBlock * pBlock, Color8888 * col_0, Color8888 * col_1, 
													 Color8888 * col_2, Color8888 * col_3,
													 WORD & wrd  )
{
	// There are 4 methods to use - see the Time_ functions.
	// 1st = shift = does normal approach per byte for color comps
	// 2nd = use freak variable bit field color565 for component extraction
	// 3rd = use super-freak DWORD adds BEFORE shifting the color components
	//  This lets you do only 1 add per color instead of 3 BYTE adds and
	//  might be faster
	// Call RunTimingSession() to run each of them & output result to txt file

 
	// freak variable bit structure method
	// normal math
	// This method is fastest

	Color565 * pCol;

	pCol = (Color565*) & (pBlock->col0 );

	col_0->a = 0xff;
	col_0->r = pCol->nRed;
	col_0->r <<= 3;				// shift to full precision
	col_0->g = pCol->nGreen;
	col_0->g <<= 2;
	col_0->b = pCol->nBlue;
	col_0->b <<= 3;

	pCol = (Color565*) & (pBlock->col1 );
	col_1->a = 0xff;
	col_1->r = pCol->nRed;
	col_1->r <<= 3;				// shift to full precision
	col_1->g = pCol->nGreen;
	col_1->g <<= 2;
	col_1->b = pCol->nBlue;
	col_1->b <<= 3;


	if( pBlock->col0 > pBlock->col1 )
	{
		// Four-color block: derive the other two colors.    
		// 00 = color_0, 01 = color_1, 10 = color_2, 11 = color_3
		// These two bit codes correspond to the 2-bit fields 
		// stored in the 64-bit block.

		wrd = ((WORD)col_0->r * 2 + (WORD)col_1->r )/3;
											// no +1 for rounding
											// as bits have been shifted to 888
		col_2->r = (BYTE)wrd;

		wrd = ((WORD)col_0->g * 2 + (WORD)col_1->g )/3;
		col_2->g = (BYTE)wrd;

		wrd = ((WORD)col_0->b * 2 + (WORD)col_1->b )/3;
		col_2->b = (BYTE)wrd;
		col_2->a = 0xff;

		wrd = ((WORD)col_0->r + (WORD)col_1->r *2 )/3;
		col_3->r = (BYTE)wrd;

		wrd = ((WORD)col_0->g + (WORD)col_1->g *2 )/3;
		col_3->g = (BYTE)wrd;

		wrd = ((WORD)col_0->b + (WORD)col_1->b *2 )/3;
		col_3->b = (BYTE)wrd;
		col_3->a = 0xff;

	}
	else
	{
		// Three-color block: derive the other color.
		// 00 = color_0,  01 = color_1,  10 = color_2,  
		// 11 = transparent.
		// These two bit codes correspond to the 2-bit fields 
		// stored in the 64-bit block. 

		// explicit for each component, unlike some refrasts...
		
		// TRACE("block has alpha\n");

		wrd = ((WORD)col_0->r + (WORD)col_1->r )/2;
		col_2->r = (BYTE)wrd;
		wrd = ((WORD)col_0->g + (WORD)col_1->g )/2;
		col_2->g = (BYTE)wrd;
		wrd = ((WORD)col_0->b + (WORD)col_1->b )/2;
		col_2->b = (BYTE)wrd;
		col_2->a = 0xff;

		col_3->r = 0x00;		// random color to indicate alpha
		col_3->g = 0xff;
		col_3->b = 0xff;
		col_3->a = 0x00;

	}
}			//  Get color block colors (...)
/*
void DecodeColorBlock( DWORD * pImPos, DXTColBlock * pColorBlock, int width,
								DWORD * col_0,
								DWORD * col_1, DWORD * col_2, DWORD * col_3 )
{
	// width is width of image in pixels


	DWORD bits;
	int r,n;

	// bit masks = 00000011, 00001100, 00110000, 11000000
	const DWORD masks[] = { 3, 12, 3 << 4, 3 << 6 };
	const int   shift[] = { 0, 2, 4, 6 };

	// r steps through lines in y
	for( r=0; r < 4; r++, pImPos += width-4 )	// no width*4 as DWORD ptr inc will *4
	{

		// width * 4 bytes per pixel per line
		// each j dxtc row is 4 lines of pixels

		// pImPos = (DWORD*)((DWORD)pBase + i*16 + (r+j*4) * m_nWidth * 4 );

		// n steps through pixels
		for( n=0; n < 4; n++ )
		{
			bits =		pColorBlock->row[r] & masks[n];
			bits >>=	shift[n];

			switch( bits )
			{
			case 0 :
				*pImPos = *col_0;
				pImPos++;		// increment to next DWORD
				break;
			case 1 :
				*pImPos = *col_1;
				pImPos++;
				break;
			case 2 :
				*pImPos = *col_2;
				pImPos++;
				break;
			case 3 :
				*pImPos = *col_3;
				pImPos++;
				break;
			default:
				__asm int 3
				pImPos++;
				break;
			}
		}
	}
}
*/
/*
void DecompressDXT1(char* pBits,IDirect3DSurface8* pSurface)
{
	D3DSURFACE_DESC	ddsc;
	pSurface->GetDesc(&ddsc);

	int xblocks, yblocks;

#ifdef DEBUG
	if( (ddsc.Width % 4 ) != 0 )
	{
		__asm int 3
	}
	if( (ddsc.Height %4 ) != 0 )
	{
		__asm int 3
	}
#endif 

	xblocks = ddsc.Width / 4;
	yblocks = ddsc.Height / 4;


	D3DLOCKED_RECT	rect;
	pSurface->LockRect(&rect,NULL,D3DLOCK_READONLY);

	
	int i,j;

	DWORD * pBase  = (DWORD*)  pBits;
	DWORD * pImPos = (DWORD*)  pBase;			// pos in decompressed data
	WORD  * pPos   = (WORD*)   rect.pBits;		// pos in compressed data

	DXTColBlock * pBlock;

	Color8888 col_0, col_1, col_2, col_3;


	WORD wrd;

	for( j=0; j < yblocks; j++ )
	{
		// 8 bytes per block
		pBlock = (DXTColBlock*) ( (DWORD)rect.pBits + j * xblocks * 8 );


		for( i=0; i < xblocks; i++, pBlock++ )
		{

			// inline func:
			GetColorBlockColors( pBlock, &col_0, &col_1, &col_2, &col_3, wrd );


			// now decode the color block into the bitmap bits
			// inline func:

			pImPos = (DWORD*)((DWORD)pBase + i*16 + (j*4) * ddsc.Width * 4 );


			DecodeColorBlock( pImPos, pBlock, ddsc.Width, (DWORD*)&col_0, (DWORD*)&col_1,
								(DWORD*)&col_2, (DWORD*)&col_3 );
		}
	}
	pSurface->UnlockRect();
}*/

void DecodeColorBlock( WORD * pImPos, DXTColBlock * pColorBlock, int width, DWORD* pdwColor )
{
	// width is width of image in pixels

/*
			*(WORD*)((char*)p + pPatch->sx*2 + pPatch->sy*m_dwSurfaceWidth*2) = (WORD)
				(
				((R>>3)<<11) |
				((G>>2)<<5) |
				(B>>3)
				);*/
	DWORD	A,R,G,B;
	WORD	wColor;
	DWORD bits;
	int r,n;

	// bit masks = 00000011, 00001100, 00110000, 11000000
	const DWORD masks[] = { 3, 12, 3 << 4, 3 << 6 };
	const int   shift[] = { 0, 2, 4, 6 };

	// r steps through lines in y
	for( r=0; r < 4; r++, pImPos += width-4 )	// no width*4 as DWORD ptr inc will *4
	{

		// width * 4 bytes per pixel per line
		// each j dxtc row is 4 lines of pixels

		// pImPos = (DWORD*)((DWORD)pBase + i*16 + (r+j*4) * m_nWidth * 4 );

		// n steps through pixels
		for( n=0; n < 4; n++ )
		{
			bits =		pColorBlock->row[r] & masks[n];
			bits >>=	shift[n];

			A = ((pdwColor[bits]>> 24) & 0x000000ff) >> 3;
			R = ((pdwColor[bits]>> 16) & 0x000000ff) >> 3;
			G = ((pdwColor[bits]>> 8 ) & 0x000000ff) >> 2;
			B = ( pdwColor[bits]	   & 0x000000ff) >> 3;
			wColor = (WORD)( (R<<11) | (G<<5) | B );
			*pImPos = wColor;
			pImPos++;
/*
			switch( bits )
			{
			case 0 :
				*pImPos = pdwColor[0];
				pImPos++;		// increment to next DWORD
				break;
			case 1 :
				*pImPos = pdwColor[1];
				pImPos++;
				break;
			case 2 :
				*pImPos = pdwColor[2];
				pImPos++;
				break;
			case 3 :
				*pImPos = pdwColor[3];
				pImPos++;
				break;
			default:
				__asm int 3
				pImPos++;
				break;
			}*/

		}
	}
}

BOOL DecodePixelFormat(D3DFORMAT* pD3DFormat, DDPIXELFORMAT* pddpf )
{
	BOOL	bResult = TRUE;
    switch( pddpf->dwFourCC )
    {
        case MAKEFOURCC('D','X','T','1'):
			*pD3DFormat = D3DFMT_DXT1;
			break;

        case MAKEFOURCC('D','X','T','2'):
			*pD3DFormat = D3DFMT_DXT2;
			break;

        case MAKEFOURCC('D','X','T','3'):
			*pD3DFormat = D3DFMT_DXT3;
            break;

        case MAKEFOURCC('D','X','T','4'):
			*pD3DFormat = D3DFMT_DXT4;
            break;

        case MAKEFOURCC('D','X','T','5'):
			*pD3DFormat = D3DFMT_DXT5;
            break;
		default:
			bResult = FALSE;
    }
	return bResult;
}

BOOL CD3DResourceManager::CreateTextureWithDDS(IDirect3DTexture8** ppTexture,char* szFileName)
{
	// only understands .dds files for now
	// return true if success
	IDirect3DTexture8*	pTexture = NULL;
	BOOL	bResult = FALSE;


	char	dds[] = ".dds";
	char	szDDSFileName[_MAX_PATH];
	memset(szDDSFileName,0,sizeof(szDDSFileName));
	RemoveExt(szDDSFileName,szFileName);
	lstrcat(szDDSFileName,dds);
	
	void*	pFP = g_pFileStorage->FSOpenFile(szDDSFileName,FSFILE_ACCESSMODE_BINARY);
	if (!pFP)
		goto lb_return;

	DDSURFACEDESC2      ddsd;
    DWORD				dwMagic;


	

    // Read magic number
//    fread( &dwMagic, sizeof(DWORD), 1, file );
	g_pFileStorage->FSRead(pFP,&dwMagic,sizeof(DWORD));

    if( dwMagic != MAKEFOURCC('D','D','S',' ') )
    {
		goto lb_close_return;
    }
	
    // Read the surface description
   // fread( &ddsd, sizeof(DDSURFACEDESC2), 1, file );
	g_pFileStorage->FSRead(pFP,&ddsd,sizeof(DDSURFACEDESC2));


	BOOL	bMipTexture;
	bMipTexture = ( ddsd.dwMipMapCount > 0 ) ? TRUE : FALSE;
	
    // Does texture have mipmaps?
  //  m_bMipTexture = ( ddsd.dwMipMapCount > 0 ) ? TRUE : FALSE;

    // Clear unwanted flags
	
//	ddsd.dwFlags &= (~DDSD_PITCH);
//	ddsd.dwFlags &= (~DDSD_LINEARSIZE);


	HRESULT hr;

	BOOL	bUseCompress;
	bUseCompress = m_pRenderer->IsCanUseCompressedTexture();


	D3DPOOL	pool;
	D3DFORMAT	format;
	if (!DecodePixelFormat(&format,&ddsd.ddpfPixelFormat ))
		goto lb_close_return;


	if (bUseCompress)
	{

		pool = D3DPOOL_MANAGED;
	}
	else
	{
		pool = D3DPOOL_SCRATCH;
	}
	


	
	hr = CreateTexture(ddsd.dwWidth,ddsd.dwHeight,ddsd.dwMipMapCount,0,format,pool,&pTexture);
	if (D3D_OK != hr)
		goto lb_close_return;

	D3DSURFACE_DESC		ddsc;
	DWORD i;
	D3DLOCKED_RECT	lockRect;

	for (i=0; i<ddsd.dwMipMapCount; i++)
    {
		
		pTexture->LockRect(i,&lockRect,NULL,0);
		pTexture->GetLevelDesc(i,&ddsc);

        if( ddsd.dwFlags & DDSD_LINEARSIZE )
        {
			__asm nop
		//	g_pFileStorage->FSRead(pFP,lockRect.pBits,ddsc.dwLinearSize);
            //fread( ddsc.lpSurface, ddsc.dwLinearSize, 1, file );
        }
        else
        {
            BYTE* pDest;
			DWORD dwBytesPerRow;
			pDest = (BYTE*)lockRect.pBits;
            dwBytesPerRow = ddsc.Width * 2;//(ddsc.ddpfPixelFormat.dwSize / 8);
            
			DWORD	yp;
            for(yp = 0; yp < ddsc.Height; yp++ )
            {
               // fread( pDest, dwBytesPerRow, 1, file );
				g_pFileStorage->FSRead(pFP,pDest,dwBytesPerRow);
                pDest += lockRect.Pitch;
            }
        }
		pTexture->UnlockRect(i);
    }


	if (!bUseCompress)
	{
		IDirect3DSurface8*	pSurfaceSrc;

		IDirect3DTexture8*	pTexResult;
		IDirect3DSurface8*	pSurfaceResult;
		hr = CreateTexture(ddsd.dwWidth,ddsd.dwHeight,ddsd.dwMipMapCount,0,D3DFMT_R5G6B5,D3DPOOL_MANAGED,&pTexResult);
		
		for (i=0; i<ddsd.dwMipMapCount; i++)
		{
			pTexture->GetSurfaceLevel(i,&pSurfaceSrc);

			pTexResult->GetSurfaceLevel(i,&pSurfaceResult);

			//pSurfaceResult->LockRect(&lockRect,NULL,0);
			D3DXLoadSurfaceFromSurface(pSurfaceResult,NULL,NULL,pSurfaceSrc,NULL,NULL,D3DX_FILTER_NONE ,0x00000000);
			//DecompressDXT1((char*)lockRect.pBits,lockRect.Pitch,pSurfaceSrc);
			//pSurfaceResult->UnlockRect();

			pTexResult->Release();
			pSurfaceSrc->Release();
		}
		Release(pTexture);
		pTexture = pTexResult;
		

	}



lb_success:
	bResult = TRUE;

lb_close_return:
	g_pFileStorage->FSCloseFile(pFP);

lb_return:
	*ppTexture = pTexture;
	return bResult;
}







void SetOutLineBorder2Bytes(char* pBits,DWORD dwWidth,DWORD dwHeight,DWORD dwPitch,WORD	wBorderColor)
{
	DWORD		y;
	char*		p = pBits;


	memset(pBits,wBorderColor,2*dwWidth);
	memset(pBits + dwPitch*(dwHeight-1),wBorderColor,2*dwWidth);


	p = pBits;
	for (y=0; y<dwHeight; y++)
	{
		*(WORD*)p = wBorderColor;
		p += dwPitch;
	}
	
	p = pBits + (dwWidth-1)*2;
	for (y=0; y<dwHeight; y++)
	{
		*(WORD*)p = wBorderColor;
		p += dwPitch;
	}
}


BOOL IsCompressed(D3DFORMAT format)
{
	BOOL	bResult = TRUE;
	if (format == D3DFMT_DXT1)
		goto lb_return;
	
	if (format == D3DFMT_DXT2)
		goto lb_return;

	if (format == D3DFMT_DXT3)
		goto lb_return;

	if (format == D3DFMT_DXT4)
		goto lb_return;

	if (format == D3DFMT_DXT5)
		goto lb_return;

	bResult = FALSE;
lb_return:
	return bResult;
}

CD3DResourceManager::CD3DResourceManager()
{
	m_pD3DDevice = NULL;
	m_dwItemNum = 0;
	m_pHashResource = NULL;
	m_pRCDescPool = NULL;

}
BOOL CD3DResourceManager::Initialize(CoD3DDevice* pRenderer,IDirect3DDevice8* pDevice,DWORD dwMaxItemNum)
{
	m_pRenderer = pRenderer,
	m_pD3DDevice = pDevice;
	m_dwMaxItemNum = dwMaxItemNum;

	m_pRCDescPool = CreateStaticMemoryPool();
	InitializeStaticMemoryPool(m_pRCDescPool,sizeof(D3DRESOURCE_DESC),DEFAULT_D3DRESOURCE_NUM,m_dwMaxItemNum);

	m_pHashResource = QBHCreate();
	QBHInitialize(m_pHashResource,64,m_dwMaxItemNum);

	


	return TRUE;
}
HRESULT	CD3DResourceManager::Release(IUnknown* pResource)
{
	D3DRESOURCE_DESC*	pDesc;

#ifdef _DEBUG
	if (!m_dwItemNum)
	{
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,"CD3DResourceManager::Release() m_dwItemNum=0");
		
	}
#endif
	void*	pHashHandle;
	if (!QBHSelect(m_pHashResource,&pHashHandle,(DWORD*)&pDesc,1,(DWORD)pResource))
	{
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,"CD3DResourceManager::Release() Specify Resource not exist");
	}


	QBHDelete(m_pHashResource,pDesc->pHashHandle);
	LALFree(m_pRCDescPool,pDesc);
	
	m_dwItemNum--;
	
	return pResource->Release();
}
HRESULT CD3DResourceManager::CreateVertexBuffer(
								UINT Length,
								DWORD Usage,
								DWORD FVF,
								D3DPOOL Pool,
								IDirect3DVertexBuffer8** ppVertexBuffer
								)
{
	HRESULT		hr;
	D3DRESOURCE_DESC*	pDesc = (D3DRESOURCE_DESC*)LALAlloc(m_pRCDescPool);
	
	if (!pDesc)
		goto lb_return;

	hr = m_pD3DDevice->CreateVertexBuffer(Length,Usage,FVF,Pool,ppVertexBuffer);

	if (hr != D3D_OK)
	{
		LALFree(m_pRCDescPool,pDesc);
		OutputErrorMsg(m_pRenderer,hr,"CreateVertexBuffer");
		goto lb_return;
	}
	pDesc->pool = Pool;
	pDesc->pResource = *ppVertexBuffer;
	pDesc->type = D3DRESOURCE_TYPE_VERTEXBUFFER;
	pDesc->pHashHandle = QBHInsert(m_pHashResource,(DWORD)pDesc,(DWORD)pDesc->pResource);
	if (!pDesc->pHashHandle)
	{
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,"CD3DResourceManager::CreateVertexBuffer() Fail to QBHInsert");
		
	}

	m_dwItemNum++;

lb_return:
	return hr;
}

HRESULT CD3DResourceManager::CreateIndexBuffer(
								UINT Length,
								DWORD Usage,
								D3DFORMAT Format,
								D3DPOOL Pool,
								IDirect3DIndexBuffer8** ppIndexBuffer
								)
{
	
	HRESULT		hr;
	D3DRESOURCE_DESC*	pDesc = (D3DRESOURCE_DESC*)LALAlloc(m_pRCDescPool);
	
	if (!pDesc)
		goto lb_return;
	
	hr = m_pD3DDevice->CreateIndexBuffer(Length,Usage,Format,Pool,ppIndexBuffer);

	if (hr != D3D_OK)
	{
		LALFree(m_pRCDescPool,pDesc);
		OutputErrorMsg(m_pRenderer,hr,"CreateIndexBuffer");
		goto lb_return;
	}
	pDesc->pool = Pool;
	pDesc->pResource = *ppIndexBuffer;
	pDesc->type = D3DRESOURCE_TYPE_INDEXBUFFER;
	pDesc->pHashHandle = QBHInsert(m_pHashResource,(DWORD)pDesc,(DWORD)pDesc->pResource);
	if (!pDesc->pHashHandle)
	{
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,"CD3DResourceManager::CreateIndexBuffer() Fail to QBHInsert");
	}

	m_dwItemNum++;

lb_return:
	return hr;
}

HRESULT CD3DResourceManager::CreateTexture(
								UINT Width,
								UINT Height,
								UINT  Levels,
								DWORD Usage,
								D3DFORMAT Format,
								D3DPOOL Pool,
								IDirect3DTexture8** ppTexture
								)
{
	HRESULT		hr;
	D3DRESOURCE_DESC*	pDesc;

	hr = m_pD3DDevice->CreateTexture(Width,Height,Levels,Usage,Format,Pool,ppTexture);
	if (hr != D3D_OK)
	{
		OutputErrorMsg(m_pRenderer,hr,"CreateTexture");
		goto lb_return;
	}
	
	pDesc = (D3DRESOURCE_DESC*)LALAlloc(m_pRCDescPool);

	if (!pDesc)
	{
		(*ppTexture)->Release();

#ifdef _DEBUG

		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,"CD3DResourceManager::CreateTexture() Fail to LALAlloc");
#endif

		goto lb_return;
	}


	pDesc->pool = Pool;
	pDesc->pResource = *ppTexture;
	pDesc->type = D3DRESOURCE_TYPE_TEXTURE;
	pDesc->pHashHandle = QBHInsert(m_pHashResource,(DWORD)pDesc,(DWORD)pDesc->pResource);
	if (!pDesc->pHashHandle)
	{
#ifdef _DEBUG
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,"CD3DResourceManager::CreateTexture() Fail to QBHInsert");
#endif
	}

	m_dwItemNum++;

lb_return:
	return hr;
}

HRESULT CD3DResourceManager::CreateFont(
								CONST LOGFONT* pLogFont,
								LPD3DXFONT* ppFont
								)
{
	HRESULT		hr;
	D3DRESOURCE_DESC*	pDesc = (D3DRESOURCE_DESC*)LALAlloc(m_pRCDescPool);

	if (!pDesc)
		goto lb_return;

	hr = D3DXCreateFontIndirect(m_pD3DDevice,pLogFont,ppFont);
		
	if (hr != D3D_OK)
	{
		LALFree(m_pRCDescPool,pDesc);
		OutputErrorMsg(m_pRenderer,hr,"CreateFont");
		goto lb_return;
	}
	pDesc->pool = D3DPOOL_SYSTEMMEM;
	pDesc->pResource = *(IDirect3DResource8**)ppFont;			// 이 부분은 예외처리를 해야한다.FONT는 따로 처리.
	pDesc->type = D3DRESOURCE_TYPE_D3DXFONT;
	pDesc->pHashHandle = QBHInsert(m_pHashResource,(DWORD)pDesc,(DWORD)pDesc->pResource);
	if (!pDesc->pHashHandle)
	{
#ifdef _DEBUG
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,"CD3DResourceManager::CreateD3DXFont() Fail to QBHInsert");
#endif
	}

	m_dwItemNum++;

lb_return:
	return hr;
}
void CD3DResourceManager::OnLostFont()
{
	DWORD	dwNum;
	D3DRESOURCE_DESC**	ppRestoreItem = new D3DRESOURCE_DESC*[m_dwMaxItemNum];
	dwNum = QBHGetAllItem(m_pHashResource,(DWORD*)ppRestoreItem,m_dwMaxItemNum);

#ifdef _DEBUG
	if (m_dwItemNum != dwNum)
	{
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,"CD3DResourceManager::OnLostFont() m_dwItemNum != dwNum");
	}
#endif

	ID3DXFont*	pFont;
	for (DWORD i=0; i<dwNum; i++)
	{
		if (ppRestoreItem[i]->type == D3DRESOURCE_TYPE_D3DXFONT)
		{
			pFont = (ID3DXFont*)ppRestoreItem[i]->pResource;
			pFont->OnLostDevice();
		}
	}
	delete [] ppRestoreItem;

}

void CD3DResourceManager::OnResetFont()
{
	DWORD	dwNum;
	D3DRESOURCE_DESC**	ppRestoreItem = new D3DRESOURCE_DESC*[m_dwMaxItemNum];
	dwNum = QBHGetAllItem(m_pHashResource,(DWORD*)ppRestoreItem,m_dwMaxItemNum);

#ifdef _DEBUG
	if (m_dwItemNum != dwNum)
	{
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,"CD3DResourceManager::OnResetFont() m_dwItemNum!=dwItem");
	}
#endif

	ID3DXFont*	pFont;
	for (DWORD i=0; i<dwNum; i++)
	{
		if (ppRestoreItem[i]->type == D3DRESOURCE_TYPE_D3DXFONT)
		{
			pFont = (ID3DXFont*)ppRestoreItem[i]->pResource;
			pFont->OnResetDevice();
		}
	}
	delete [] ppRestoreItem;

}

#ifdef _DEBUG
void CD3DResourceManager::Check()
{
	DWORD	dwNum;
	D3DRESOURCE_DESC**	ppRestoreItem = new D3DRESOURCE_DESC*[m_dwMaxItemNum];
	dwNum = QBHGetAllItem(m_pHashResource,(DWORD*)ppRestoreItem,m_dwMaxItemNum);

	if (m_dwItemNum != dwNum)
	{
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,"CD3DResourceManager::Check() m_dwItemNum != dwItemNum");
	}


	for (DWORD i=0; i<dwNum; i++)
	{
		if (ppRestoreItem[i]->pool == D3DPOOL_DEFAULT)
		{		
			DWORD	dwAddr;
			GetEIP(&dwAddr);
			g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,"CD3DResourceManager::Check() pool==D3DPOOL_DEFAULT");
	//		ppRestoreItem[i]->pResource->Restore();
		}
	}
	delete [] ppRestoreItem;
}
#endif

BOOL CD3DResourceManager::CreateTextureWithBitmap(LPDIRECT3DTEXTURE8* ppTex,DWORD dwUsage,D3DFORMAT format,D3DPOOL pool,char* pBits,DWORD dwWidth,DWORD dwHeight,BOOL bUseMipmap,BOOL bCompress)
{
	BOOL		bResult = FALSE;
	HRESULT		hr;


	DWORD	dwBytesPerPixel = 2;
	if (format == D3DFMT_A8R8G8B8)
		dwBytesPerPixel = 4;



	char*	pTemp = new char[dwWidth*dwHeight*dwBytesPerPixel];
	memset(pTemp,0,dwWidth*dwHeight*dwBytesPerPixel);
			
	DWORD	miplevel = 0;
	DWORD	width = dwWidth;
	DWORD	height = dwHeight;

	LPDIRECT3DTEXTURE8	pTex = NULL;
	

	if (dwUsage || (!bUseMipmap))
	{
		miplevel = 1;
		goto lb_create;
	}
	while((width /= 2)> MAX_MIPMAP_SIZE )
	{
		miplevel++;
	}
	if (!miplevel)
		miplevel = 1;

lb_create:
	D3DFORMAT	texfmt;

	// 디바이스가 압축텍스쳐를 지원하는지 확인해야한다.
	texfmt = format;
	if (bCompress)
	{
		if (m_pRenderer->IsCanUseCompressedTexture() && format == D3DFMT_R5G6B5)
		{
			if (0==(dwWidth%4) && (0==(dwHeight%4)) )
				texfmt = m_pRenderer->GetDefaultCopmressedTextureFormat();
		}
	}
		
	
	hr = CreateTexture(
		dwWidth,
		dwHeight,
		miplevel,
		dwUsage,
		texfmt,
		D3DPOOL_MANAGED,
		&pTex);
	
	if (D3D_OK != hr)
	{
		goto lb_return;
	}
	

	DWORD	div_const;
	DWORD	i,j;

///	if (miplevel > 2)
//		miplevel = 2;

	for (i=0; i<miplevel; i++)
	{
		char*		pImg = pBits;
		div_const =1;
		if (!i)
			goto lb_set_pos;
		
		for (j=0; j<i; j++)
			div_const *= 2;

lb_set_pos:
		width = dwWidth/div_const;
		height = dwHeight/div_const;

		
		if (div_const > 1)
		{
			// 밉맵에 텍스쳐를 바른다..
			pImg = pTemp;
			if (D3DFMT_A4R4G4B4 == format)
			{
				ScaleDownTexture4444(
					(WORD*)pBits,
					(WORD*)pImg,
					dwWidth,
					dwHeight,
					div_const);
			}
			else if (D3DFMT_A8R8G8B8 == format)
			{
				ScaleDownTexture8888(
					(DWORD*)pBits,
					(DWORD*)pImg,
					dwWidth,
					dwHeight,
					div_const);

			}
			else
			{
				ScaleDownTexture565(
					(WORD*)pBits,
					(WORD*)pImg,
					dwWidth,
					dwHeight,
					div_const);
			}
		}		
		SetSurfaceWithBitmap(i,pTex,pImg,width,height,format);
	}

	*ppTex = pTex;
	bResult = TRUE;

lb_return:
	delete [] pTemp;
	return bResult;
	
}
/*
BOOL CD3DResourceManager::CreateBumpMap(LPDIRECT3DTEXTURE8* ppBumpMap,LPDIRECT3DTEXTURE8 pSrc)
{
	// 디바이스가 범프맵을 지원하지 않으면 무조건 실패하게 해야한다.
	BOOL	bResult = FALSE;
	*ppBumpMap = NULL;

	
//	IDirect3DSurface8*	pSurfaceDest;
	IDirect3DTexture8*	pBumpDest;
	

	DWORD	dwLevelCount = pSrc->GetLevelCount();

	D3DFORMAT	formatBump;
	formatBump = D3DFMT_V8U8;

	D3DSURFACE_DESC    ddsd;
	pSrc->GetLevelDesc(0,&ddsd);

	// Create the bumpmap's surface and texture objects
	if (D3D_OK != CreateTexture( ddsd.Width, ddsd.Height,dwLevelCount,0,formatBump, D3DPOOL_MANAGED, &pBumpDest))
    	goto lb_return;

	DWORD	i;
	for (i=0; i<dwLevelCount; i++)
	{
		pSrc->GetLevelDesc(i,&ddsd);

		IDirect3DSurface8*	pSurfaceTemp;
		pSrc->GetSurfaceLevel(i,&pSurfaceTemp);

		IDirect3DSurface8*	pBumpSrc;
		HRESULT hr = m_pD3DDevice->CreateImageSurface(ddsd.Width,ddsd.Height,D3DFMT_A8R8G8B8,&pBumpSrc);

		
		D3DXLoadSurfaceFromSurface(pBumpSrc,NULL,NULL,pSurfaceTemp,NULL,NULL,D3DX_FILTER_NONE ,0x00000000);
		pSurfaceTemp->Release();

		
		// Fill the bits of the new texture surface with bits from
		// a private format.

		D3DLOCKED_RECT     d3dlr;
		pBumpSrc->LockRect(&d3dlr, 0, 0 );

		DWORD dwSrcPitch,dwDstPitch;
		BYTE* pSrcTopRow;
		BYTE* pDstTopRow;
		BYTE* pSrcCurRow;
		BYTE* pDstCurRow;
		BYTE* pSrcBotRow;
		BYTE* pDstBotRow;


		dwSrcPitch = (DWORD)d3dlr.Pitch;
		pSrcTopRow = (BYTE*)d3dlr.pBits;
		pSrcCurRow = pSrcTopRow;
		pSrcBotRow = pSrcTopRow + (dwSrcPitch * (ddsd.Height - 1) );

		pBumpDest->LockRect( i, &d3dlr, 0, 0 );
		dwDstPitch = (DWORD)d3dlr.Pitch;
		pDstTopRow = (BYTE*)d3dlr.pBits;
		pDstCurRow = pDstTopRow;
		pDstBotRow = pDstTopRow + (dwDstPitch * (ddsd.Height - 1) );

		DWORD	y;
		for(y=0; y<ddsd.Height; y++ )
		{
			BYTE* pSrcB0; // addr of current pixel
			BYTE* pSrcB1; // addr of pixel below current pixel, wrapping to top if necessary
			BYTE* pSrcB2; // addr of pixel above current pixel, wrapping to bottom if necessary
			BYTE* pDstT;  // addr of dest pixel;

			pSrcB0 = pSrcCurRow;

			if( y == ddsd.Height - 1)
				pSrcB1 = pSrcTopRow;
			else
				pSrcB1 = pSrcCurRow + dwSrcPitch;

			if( y == 0 )
				pSrcB2 = pSrcBotRow;
			else
				pSrcB2 = pSrcCurRow - dwSrcPitch;

			pDstT = pDstCurRow;

			 
			DWORD x;
			for(x=0; x<ddsd.Width; x++ )
			{
				LONG v00; // Current pixel
				LONG v01; // Pixel to the right of current pixel, wrapping to left edge if necessary
				LONG vM1; // Pixel to the left of current pixel, wrapping to right edge if necessary
				LONG v10; // Pixel one line below.
				LONG v1M; // Pixel one line above.

				v00 = *(pSrcB0+0);
            
				if( x == ddsd.Width - 1 )
					v01 = *(pSrcCurRow);
				else
					v01 = *(pSrcB0+4);
            
				if( x == 0 )
					vM1 = *(pSrcCurRow + (4 * (ddsd.Width - 1)));
				else
					vM1 = *(pSrcB0-4);
				v10 = *(pSrcB1+0);
				v1M = *(pSrcB2+0);

				LONG iDu = (vM1-v01); // The delta-u bump value
				LONG iDv = (v1M-v10); // The delta-v bump value

				// The luminance bump value (land masses are less shiny)
				WORD uL = ( v00>1 ) ? 63 : 127;

				switch(formatBump)
				{
					case D3DFMT_V8U8:
						*pDstT++ = (BYTE)iDu;
						*pDstT++ = (BYTE)iDv;
						break;

					case D3DFMT_L6V5U5:
						*(WORD*)pDstT  = (WORD)( ( (iDu>>3) & 0x1f ) <<  0 );
						*(WORD*)pDstT |= (WORD)( ( (iDv>>3) & 0x1f ) <<  5 );
						*(WORD*)pDstT |= (WORD)( ( ( uL>>2) & 0x3f ) << 10 );
						pDstT += 2;
						break;

					case D3DFMT_X8L8V8U8:
						*pDstT++ = (BYTE)iDu;
						*pDstT++ = (BYTE)iDv;
						*pDstT++ = (BYTE)uL;
						*pDstT++ = (BYTE)0L;
						break;
				}

				// Move one pixel to the right (src is 32-bpp)
				pSrcB0+=4;
				pSrcB1+=4;
				pSrcB2+=4;
			}

			// Move to the next line
			pSrcCurRow += dwSrcPitch;
			pDstCurRow += dwDstPitch;
		}
		
		pBumpDest->UnlockRect(i);
		pBumpSrc->UnlockRect();
		pBumpSrc->Release();
	}
	*ppBumpMap = pBumpDest;
	bResult = TRUE;


lb_return:
	return bResult;

}
*/



BOOL CD3DResourceManager::CreateBumpMap(LPDIRECT3DTEXTURE8* ppBumpMap,LPDIRECT3DTEXTURE8 pSrc)
{    
/*
	LPDIRECT3DTEXTURE8 pColorMap;
    if(FAILED(D3DXCreateTextureFromFile(m_pRenderer->GetDevice(),"earth.bmp",&pColorMap)))
        return E_FAIL;

    LPDIRECT3DTEXTURE8 pHeightMap = NULL;
	if(FAILED(D3DXCreateTextureFromFile(m_pRenderer->GetDevice(),"earthbump.bmp",&pHeightMap)))
        return E_FAIL;

	D3DSURFACE_DESC    desc;
    pSrc->GetLevelDesc(0,&desc);
	CreateTexture(desc.Width, desc.Height, 0, 0, D3DFMT_A8R8G8B8,D3DPOOL_MANAGED, ppBumpMap);
	D3DXComputeNormalMap(*ppBumpMap,pHeightMap,NULL,0,D3DX_CHANNEL_RED,10);*/

	D3DSURFACE_DESC    desc;
    pSrc->GetLevelDesc(0,&desc);
	DWORD	dwMipLevelNum = pSrc->GetLevelCount();
	CreateTexture(desc.Width, desc.Height,dwMipLevelNum, 0, D3DFMT_A8R8G8B8,D3DPOOL_MANAGED, ppBumpMap);

    D3DXComputeNormalMap(*ppBumpMap,pSrc,NULL,0,D3DX_CHANNEL_RED,5);

	return TRUE;
}

/*
BOOL CD3DResourceManager::CreateTextureWithDDS(IDirect3DTexture8** ppTexture,char* szFileName)
{
	BOOL	bResult = FALSE;

	char	dds[] = ".dds";
	char	szDDSFileName[_MAX_PATH];
	memset(szDDSFileName,0,sizeof(szDDSFileName));
	RemoveExt(szDDSFileName,szFileName);
	lstrcat(szDDSFileName,dds);
	
	HRESULT		hr;
	D3DRESOURCE_DESC*	pDesc;
	
	hr = D3DXCreateTextureFromFile(m_pD3DDevice,szDDSFileName,ppTexture);

	if (hr != D3D_OK)
	{
	//	OutputErrorMsg(m_pRenderer,hr,"D3DXCreateTextureFromFile");
		goto lb_return;
	}
	
	pDesc = (D3DRESOURCE_DESC*)LALAlloc(m_pRCDescPool);
	if (!pDesc)
	{
		(*ppTexture)->Release();
		
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,"CD3DResourceManager::CreateTextureWithDDS() Fail to LALAlloc()");
		goto lb_return;
	}
	
	
	
	pDesc->pool = D3DPOOL_MANAGED;
	pDesc->pResource = *ppTexture;
	pDesc->type = D3DRESOURCE_TYPE_TEXTURE;
	pDesc->pHashHandle = QBHInsert(m_pHashResource,(DWORD)pDesc,(DWORD)pDesc->pResource);
	if (!pDesc->pHashHandle)
	{
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,"CD3DResourceManager::CreateTextureWithDDS() Fail to QBHInsert");

	}


	m_dwItemNum++;

	bResult = TRUE;

lb_return:
	return bResult;
}*/
BOOL CD3DResourceManager::SetSurfaceWithBitmap(DWORD dwSurfIndex,LPDIRECT3DTEXTURE8 pTex,char* pBits,DWORD dwWidth,DWORD dwHeight,D3DFORMAT format)
{
	BOOL			bResult = FALSE;
	HRESULT			hr;
	
	D3DLOCKED_RECT	rect;
	DWORD			y;
	char*			p;

	D3DSURFACE_DESC		ddsc;
	DWORD			dwBytesPerPixel = 2;
	if (D3DFMT_A8R8G8B8 == format)
		dwBytesPerPixel = 4;

	

	if (!pBits)
		return FALSE;

	IDirect3DSurface8*	pSurface = NULL;
	IDirect3DSurface8*	pSurfaceTemp = NULL;

	pTex->GetSurfaceLevel(dwSurfIndex,&pSurface);

	memset(&ddsc,0,sizeof(ddsc));
	pSurface->GetDesc(&ddsc);


	if (IsCompressed(ddsc.Format))
		m_pD3DDevice->CreateImageSurface(ddsc.Width,ddsc.Height,format,&pSurfaceTemp);
	else
		pSurfaceTemp = pSurface;


	hr = pSurfaceTemp->LockRect(&rect,NULL,0);
	
	p = (char*)rect.pBits;

	for (y=0; y<dwHeight; y++)
	{
		memcpy(p,pBits+y*dwWidth*dwBytesPerPixel,dwWidth*dwBytesPerPixel);
		p += rect.Pitch;
	}
	
	pSurfaceTemp->UnlockRect();

	if (pSurfaceTemp != pSurface)
	{
		D3DXLoadSurfaceFromSurface(pSurface,NULL,NULL,pSurfaceTemp,NULL,NULL,D3DX_FILTER_NONE ,0x00000000);
		pSurfaceTemp->Release();
	}
	pSurface->Release();

	bResult = TRUE;

lb_return:
	return bResult;
}

BOOL CD3DResourceManager::CreateBitmapWithSurface(char** ppBits,DWORD* pdwWidth,DWORD* pdwHeight,DWORD* pdwBytesPerPixel,IDirect3DSurface8* pSurface)
{
	// 565포맷만 됨...압축텍스쳐는 됨...

	BOOL	bResult = FALSE;

	D3DSURFACE_DESC	ddsc;
	IDirect3DSurface8*	pSurfaceTemp = NULL;

	D3DLOCKED_RECT	rect;
	DWORD y;
	char*	pSrc;
	char*	pDest;

	pSurface->GetDesc(&ddsc);

	DWORD	dwBytesPerPixel;

	if (IsCompressed(ddsc.Format))
	{
		m_pD3DDevice->CreateImageSurface(ddsc.Width,ddsc.Height,D3DFMT_R5G6B5,&pSurfaceTemp);
		D3DXLoadSurfaceFromSurface(pSurfaceTemp,NULL,NULL,pSurface,NULL,NULL,D3DX_FILTER_NONE ,0x00000000);
		dwBytesPerPixel = 2;
	
	}
	else
	{

		if (ddsc.Format == D3DFMT_A8R8G8B8)
			dwBytesPerPixel = 4;
		else
			dwBytesPerPixel = 2;

		// 압축 텍스쳐가 아니면..
		m_pD3DDevice->CreateImageSurface(ddsc.Width,ddsc.Height,ddsc.Format,&pSurfaceTemp);
	}
	*pdwWidth = ddsc.Width;
	*pdwHeight = ddsc.Height;
	*pdwBytesPerPixel = dwBytesPerPixel;

	D3DXLoadSurfaceFromSurface(pSurfaceTemp,NULL,NULL,pSurface,NULL,NULL,D3DX_FILTER_NONE ,0x00000000);


	pSurfaceTemp->LockRect(&rect,NULL,0);

	pDest = *ppBits = new char[ddsc.Width*ddsc.Height*dwBytesPerPixel];
	pSrc = (char*)rect.pBits;

	for (y=0; y<ddsc.Height; y++)
	{
		memcpy(pDest,pSrc,ddsc.Width*dwBytesPerPixel);
		pDest += ddsc.Width*dwBytesPerPixel;
		pSrc += rect.Pitch;
	}

	pSurfaceTemp->UnlockRect();
	pSurfaceTemp->Release();

	bResult = TRUE;

lb_return:
	return bResult;

}


BOOL CD3DResourceManager::DuplicateTexture(LPDIRECT3DTEXTURE8* ppTexDest,D3DPOOL pool,LPDIRECT3DTEXTURE8 pTexSrc,DWORD Flag)
{
	IDirect3DSurface8*		pSrc;
	IDirect3DSurface8*		pDest;

	BOOL				bResult = FALSE;
	DWORD				i;
	HRESULT hr;

	D3DSURFACE_DESC		ddsc;
	memset(&ddsc,0,sizeof(ddsc));
	pTexSrc->GetLevelDesc(0,&ddsc);
	
	DWORD	miplevelnum = pTexSrc->GetLevelCount();

	hr = CreateTexture(
		ddsc.Width,
		ddsc.Height,
		miplevelnum,
		ddsc.Usage,
		ddsc.Format,
		pool,
		ppTexDest);

	if (D3D_OK != hr)
	{
		OutputErrorMsg(m_pRenderer,hr,"DuplicateTexture");
		goto lb_return;
	}

	for (i=0; i<miplevelnum; i++)
	{

		pTexSrc->GetSurfaceLevel(i,&pSrc);
		(*ppTexDest)->GetSurfaceLevel(i,&pDest);
		
		m_pD3DDevice->CopyRects(pSrc,NULL,0,pDest,NULL);

		pSrc->Release();
		pDest->Release();

	}
	bResult = TRUE;
lb_return:
	return bResult;

}


BOOL CD3DResourceManager::CopyTexture(LPDIRECT3DTEXTURE8 pTexDest,LPDIRECT3DTEXTURE8 pTexSrc)
{
	IDirect3DSurface8*		pSrc;
	IDirect3DSurface8*		pDest;

	BOOL				bResult = FALSE;
	DWORD				i;


	D3DSURFACE_DESC		ddscSrc,ddscDest;
	memset(&ddscSrc,0,sizeof(ddscSrc));
	memset(&ddscDest,0,sizeof(ddscDest));

	pTexSrc->GetLevelDesc(0,&ddscSrc);
	pTexDest->GetLevelDesc(0,&ddscDest);
	
	DWORD	miplevelnum = pTexSrc->GetLevelCount();

#ifdef _DEBUG
	if (pTexSrc->GetLevelCount() != pTexDest->GetLevelCount())
		__asm int 3

	if (ddscSrc.Width != ddscDest.Width)
		__asm int 3

	if (ddscSrc.Height != ddscDest.Height)
		__asm int 3
#endif
	
	for (i=0; i<miplevelnum; i++)
	{

		pTexSrc->GetSurfaceLevel(i,&pSrc);
		pTexDest->GetSurfaceLevel(i,&pDest);

		D3DXLoadSurfaceFromSurface(pDest,NULL,NULL,pSrc,NULL,NULL,D3DX_FILTER_NONE ,0x00000000);

		pSrc->Release();
		pDest->Release();

	}
	bResult = TRUE;
lb_return:
	return bResult;

}
void CD3DResourceManager::ReleaseBitmapWithSurface(char* pBits)
{
	delete [] pBits;
}


BOOL CD3DResourceManager::ConvertCompressedTexture(char* szFileName,DWORD dwFlag)
{
	BOOL	bResult = FALSE;
	HRESULT				hr;
	CTexture			texHandle;
	char		ext[] = ".dds";
	char		szOutFileName[_MAX_PATH];
	
	
	if (!m_pRenderer->IsCanUseCompressedTexture())
		goto lb_return;

	if (!m_pRenderer->AllocTexture(&texHandle,szFileName,0,TRUE,TRUE,TEXTURE_MAP_TYPE_DIFFUSE))
		goto lb_return;
		
	memset(szOutFileName,0,sizeof(szOutFileName));
	RemoveExt(szOutFileName,szFileName);
	lstrcat(szOutFileName,ext);

	hr = D3DXSaveTextureToFile(szOutFileName,D3DXIFF_DDS,texHandle.GetTexture(), NULL);
	
	if (hr != D3D_OK)
	{

		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_D3D_ERROR,2,(void*)dwAddr,"Fail to D3DXSaveTextureToFile()");
		goto lb_return;
	}

	bResult = TRUE;

lb_return:
	return bResult;

}


BOOL CD3DResourceManager::SetTextureBorder(LPDIRECT3DTEXTURE8 pTex,DWORD dwBorderColor)
{
	BOOL			bResult = FALSE;
	HRESULT			hr;


	DWORD				dwWidth,dwHeight;
	D3DSURFACE_DESC		ddsc;
	IDirect3DTexture8*	pTexTemp = NULL;
	

	IDirect3DSurface8*	pSurfaceSrc = NULL;
	IDirect3DSurface8*	pSurfaceDest = NULL;

	DWORD dwSurfaceNum = pTex->GetLevelCount();
	pTex->GetLevelDesc(0,&ddsc);

	dwWidth = ddsc.Width;
	dwHeight = ddsc.Height;

	hr = CreateTexture(ddsc.Width,ddsc.Height,dwSurfaceNum,0,D3DFMT_A8R8G8B8,D3DPOOL_SCRATCH,&pTexTemp);	
	if (D3D_OK != hr)
		goto lb_return;

	int div_const;
	div_const = 1;
	int width,height;
	DWORD	i;
	D3DLOCKED_RECT	lockRect;

	for (i=0; i<dwSurfaceNum; i++)
	{
		width = dwWidth/div_const;
		height = dwHeight/div_const;

		pTex->GetSurfaceLevel(i,&pSurfaceSrc);
		pTexTemp->GetSurfaceLevel(i,&pSurfaceDest);
		pSurfaceSrc->GetDesc(&ddsc);

		D3DXLoadSurfaceFromSurface(pSurfaceDest,NULL,NULL,pSurfaceSrc,NULL,NULL,D3DX_FILTER_NONE ,0x00000000);

		pSurfaceDest->LockRect(&lockRect,NULL,0);

		SetOutLineBorder4Bytes((char*)lockRect.pBits,ddsc.Width,ddsc.Height,lockRect.Pitch,dwBorderColor);
			
		pSurfaceDest->UnlockRect();

		pSurfaceDest->Release();
		pSurfaceSrc->Release();

		div_const*= 2;


	}
	CopyTexture(pTex,pTexTemp);
	Release(pTexTemp);
	bResult = TRUE;

lb_return:
	return bResult;
}

BOOL CD3DResourceManager::IsValid(IUnknown* pResource)
{
	D3DRESOURCE_DESC*	pDesc;
	BOOL				bResult;
	void*				pHashHandle;

	if (QBHSelect(m_pHashResource,&pHashHandle,(DWORD*)&pDesc,1,(DWORD)pResource))
		bResult = TRUE;
	else 
		bResult = FALSE;

	return bResult;
}
CD3DResourceManager::~CD3DResourceManager()
{
#ifdef	_DEBUG
	if (m_dwItemNum)
	{
		D3DRESOURCE_DESC*	pDescList[4096];
		DWORD dwNum = QBHGetAllItem(m_pHashResource,(DWORD*)&pDescList,4096);

		char	txt[512];
		char	rctype[32];
		for (DWORD i=0; i<dwNum; i++)
		{
			memset(txt,0,512);
			memset(rctype,0,32);
			if (pDescList[i]->type == D3DRESOURCE_TYPE_TEXTURE)
			{
				wsprintf(rctype,"TEXTURE");
			}
			else if (pDescList[i]->type == D3DRESOURCE_TYPE_VERTEXBUFFER)
			{
				wsprintf(rctype,"VERTEXBUFFER");
			}
			else if (pDescList[i]->type == D3DRESOURCE_TYPE_INDEXBUFFER)
			{
				wsprintf(rctype,"INDEXBUFFER");
			}
			else if (pDescList[i]->type == D3DRESOURCE_TYPE_D3DXFONT)
			{
				wsprintf(rctype,"D3DXFONT");
			}
			
			wsprintf(txt,"Renderer detected D3DResource leak!!( %s )\n",rctype);
			OutputDebugString(txt);
		}
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_RESOURCE_LEAK,0,(void*)dwAddr,"CD3DResourceManager::CD3DResourceManager() ResourceLeak!!!");
	}
#endif
	if (m_pHashResource)
	{
		QBHRelease(m_pHashResource);
		m_pHashResource = NULL;
	}
	if (m_pRCDescPool)
	{
		ReleaseStaticMemoryPool(m_pRCDescPool);
		m_pRCDescPool = NULL;
	}

}

