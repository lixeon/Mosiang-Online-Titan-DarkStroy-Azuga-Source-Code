#include "FontObject.h"
#include "FontCache.h"
#include "CoD3DDevice.h"
#include "global_variable.h"

CFontObject::CFontObject()
{
	memset((char*)this+4,0,sizeof(CFontObject)-4);
}

void __stdcall CFontObject::BeginRender()
{
}
void __stdcall CFontObject::EndRender()
{
}
BOOL __stdcall CFontObject::CreateFont(CoD3DDevice* pRenderer,LOGFONT* pLogFont)
{
	BOOL		bResult = FALSE;

	//m_pFont = CreateFontObject(-12,0,0,0,0,"±¼¸²Ã¼");
	LOGFONT		font;

//	pLogFont = 0;
	if (!pLogFont)
	{
		pLogFont = &font;
			
		font.lfHeight = -12;
		font.lfWidth = 0;
		font.lfEscapement = 0;
		font.lfOrientation = 0;
		font.lfWeight = FW_BOLD;
		font.lfItalic = 0;
		font.lfUnderline = 0;
		font.lfStrikeOut = 0;
		
		font.lfCharSet = DEFAULT_CHARSET;
		font.lfOutPrecision = 0;
		font.lfClipPrecision = 0;
		font.lfQuality =  PROOF_QUALITY;//NONANTIALIASED_QUALITY;//PROOF_QUALITY;
		font.lfPitchAndFamily = 0;
		lstrcpy(font.lfFaceName,"±¼¸²Ã¼");
	}

	//CreateFont(-12,0,0,0,FW_BOLD,0,0,0,DEFAULT_CHARSET,0,0,PROOF_QUALITY,0,"±¼¸²Ã¼");

	m_pRenderer = pRenderer;
	m_pFontCache = m_pRenderer->GetFontCache();

	m_hFont = CreateFontIndirect(pLogFont);
	if (!m_hFont)
		goto lb_return;


	m_propertyDesc.hFont = m_hFont;
	m_propertyDesc.iHeight = pLogFont->lfHeight;

	if (m_propertyDesc.iHeight < 0)
		m_propertyDesc.iHeight *= -1;

	m_propertyDesc.iWidth = m_propertyDesc.iHeight / 2;
	
	

	
	
	m_propertyDesc.pszString = NULL;
	m_propertyDesc.dwStrLen = 0;
	m_propertyDesc.dwColor = 0xffffffff;
	
	bResult = TRUE;

lb_return:
	return bResult;
}
INT __stdcall CFontObject::DrawText(TCHAR* str,DWORD dwLen,RECT* pRect,DWORD dwColor,CHAR_CODE_TYPE type,DWORD dwFlag)
{
	INT			iResult = 0xffffffff;
	BOOL		bHasCRLF = RemoveCRLF(str,dwLen);
	
	
#ifdef	_DEBUG
	char	txt[2048];
	DWORD	dwAddr;

	if (bHasCRLF)
	{
		memset(txt,0,sizeof(txt));
		wsprintf(txt,"Fail to RenderFont(), string has CR or LF or HT!! %s\n",str);
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_PARAMETER_INVALID,1,(void*)dwAddr,txt);
	}
	
	if (0 == pRect->right - pRect->left)
	{
		memset(txt,0,sizeof(txt));
		wsprintf(txt,"Fail to RenderFont(), rect's width zero!! %s\n",str);
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_PARAMETER_INVALID,0,(void*)dwAddr,txt);
	}
	if (0 == pRect->bottom - pRect->top)
	{
		memset(txt,0,sizeof(txt));
		wsprintf(txt,"Fail to RenderFont(), rect's height zero!! %s\n",str);
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_PARAMETER_INVALID,0,(void*)dwAddr,txt);
	}
	if (pRect->right - pRect->left > m_pFontCache->GetMaxWidth())
	{
		memset(txt,0,sizeof(txt));
		wsprintf(txt,"Fail to RenderFont(), rect's width too large!! %s\n",str);
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_PARAMETER_INVALID,0,(void*)dwAddr,txt);
	}
	if (pRect->bottom - pRect->top > m_pFontCache->GetMaxWidth())
	{
		memset(txt,0,sizeof(txt));
		wsprintf(txt,"Fail to RenderFont(), rect's height too large!! %s\n",str);
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_PARAMETER_INVALID,0,(void*)dwAddr,txt);
	}
#endif

	if (!dwLen)
		goto lb_return;


	LPDIRECT3DDEVICE8 pDevice;
	
	pDevice = m_pRenderer->GetDevice();
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE ,FALSE);
	pDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_FALSE);
	pDevice->SetTextureStageState(0,D3DTSS_MAGFILTER,D3DTEXF_POINT);
	pDevice->SetTextureStageState(0,D3DTSS_MINFILTER,D3DTEXF_POINT);

	
	VECTOR2	v2Pos;
	v2Pos.x = (float)pRect->left;
	v2Pos.y = (float)pRect->top;


	pDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX,0);
	pDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU,D3DTADDRESS_WRAP );
	pDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV,D3DTADDRESS_WRAP );
	pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE);

	pDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	pDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE  );
	
	CVertexBuffer*	pVB;
	CIndexBuffer*	pIB;
	LPDIRECT3DTEXTURE8	pTex;
	
	m_propertyDesc.pszString = str;
	m_propertyDesc.dwStrLen = dwLen;
	m_propertyDesc.dwColor = dwColor;

	m_pFontCache->GetResource(&pTex,&pVB,&pIB,pRect,&m_propertyDesc);
	pDevice->SetVertexShader(pVB->GetVertexShader());
	pDevice->SetStreamSource( 0,pVB->GetVertexBuffer(),pVB->GetVertexSize());

	pDevice->SetTexture(0,pTex);
	 
	DWORD dwAlpha;
	dwAlpha = (dwColor & 0xff000000)>>24;

	
	//trustpak
	EnableAlphaSetting(pDevice, 0, dwAlpha);	
	//BeginAlphaMeshObject(pDevice,0,255,dwAlpha);		
	
	//pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE );
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	
	
	//trustpak
		
//	pDevice->SetRenderState(D3DRS_FILLMODE,RENDER_MODE_WIREFRAME);

	pDevice->SetIndices(pIB->GetIndexBuffer(),pVB->GetStartIndex());
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,4,pIB->GetStartIndex(),2);
	
	EndAlphaMeshObject(pDevice);	
	
//	pDevice->SetRenderState(D3DRS_FILLMODE,RENDER_MODE_SOLID);




	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE );

	pDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE ,TRUE);
	pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );

	pDevice->SetTextureStageState(0,D3DTSS_MAGFILTER,D3DTEXF_LINEAR);
	pDevice->SetTextureStageState(0,D3DTSS_MINFILTER,D3DTEXF_LINEAR);

//	__asm
//	{
//		rdtsc
//		pop		ecx
//		pop		ebx
//		sub		edx,ecx
//		sub		eax,ebx
//		nop
//	}
lb_return:
	return iResult;
}

CFontObject::~CFontObject()
{
	
	if (m_hFont)
	{
		DeleteObject(m_hFont);
		m_hFont = NULL;
	}
}
