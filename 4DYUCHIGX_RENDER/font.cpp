#include "font.h"
#include "CoD3DDevice.h"

CFontBuffer::CFontBuffer()
{
	memset((char*)this+4,0,sizeof(CFontBuffer)-4);
}


void __stdcall CFontBuffer::BeginRender()
{
	m_pD3DXFont->Begin();
}
void __stdcall CFontBuffer::EndRender()
{
	m_pD3DXFont->End();
}

INT __stdcall CFontBuffer::DrawText(TCHAR* str,DWORD dwLen,RECT* pRect,DWORD dwColor,CHAR_CODE_TYPE type,DWORD dwFlag)
{
	INT			iResult = 0xffffffff;

	LPDIRECT3DDEVICE8 pDevice = m_pRenderer->GetDevice();
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE ,FALSE);
	pDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_FALSE);

	DWORD dwAlpha = (dwColor & 0xff000000)>>24;
	BeginAlphaMeshObject(pDevice,0,255,dwAlpha);

	if (type == CHAR_CODE_TYPE_UNICODE)
	{
		iResult = m_pD3DXFont->DrawTextW((LPCWSTR)str,dwLen,pRect,DT_LEFT | DT_TOP | DT_NOCLIP ,dwColor);
	//	iResult = m_pD3DXFont->DrawTextW((LPCWSTR)str,dwLen,pRect,DT_LEFT | DT_TOP ,dwColor);
		
	}
	else
	{
		iResult = m_pD3DXFont->DrawTextA((LPCSTR)str,dwLen,pRect,DT_LEFT | DT_TOP | DT_NOCLIP ,dwColor);
	//	iResult = m_pD3DXFont->DrawTextA((LPCSTR)str,dwLen,pRect,DT_LEFT | DT_TOP,dwColor);
	}
	EndAlphaMeshObject(pDevice);
	
	pDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE ,TRUE);

	return iResult;
}
BOOL __stdcall CFontBuffer::CreateFont(CoD3DDevice* pRenderer,LOGFONT* pLogFont)
{
	BOOL		bResult = FALSE;

	//m_pFont = CreateFontObject(-12,0,0,0,0,"±¼¸²Ã¼");
	LOGFONT		font;

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
		lstrcpy(font.lfFaceName,"¿¬Ìå_GB2312");
	}

	//CreateFont(-12,0,0,0,FW_BOLD,0,0,0,DEFAULT_CHARSET,0,0,PROOF_QUALITY,0,"±¼¸²Ã¼");

	m_pRenderer = pRenderer;
	HRESULT hr = m_pRenderer->GetD3DResourceManager()->CreateFont(pLogFont,&m_pD3DXFont);
	
	if (hr != D3D_OK)
		goto lb_return;

	bResult = TRUE;

lb_return:
	return bResult;
}

CFontBuffer::~CFontBuffer()
{
	if (m_pD3DXFont)
	{
		m_pRenderer->GetD3DResourceManager()->Release(m_pD3DXFont);
		m_pD3DXFont = NULL;
	}

}

