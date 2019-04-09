#ifndef D3DSPRITE_H
#define D3DSPRITE_H


#include "../4DyuchiGRX_common/IGeometry.h"
#include "VertexBuffer.h"
#include "CTexture.h"
#include "IndexBuffer.h"

class CoD3DDevice;
class CTexture;

struct TRANSFORM_STATUS
{
	VECTOR2			v2Pos;
	VECTOR2			v2Scale;
	float			fRot;
	RECT			rect;
	DWORD			dwColor;
};
class C3DSprite : public IDISpriteObject
{
	
	DWORD			m_dwRefCount;
	CoD3DDevice*	m_pRenderer;
	CTexture		m_Texture;
	TRANSFORM_STATUS	m_TrStatus;
	VECTOR4			m_v4RectSrc[4];
	VECTOR4			m_v4RectDest[4];
	IMAGE_DESC*		m_pImageDesc;		
	DWORD			m_dwImageDescNum;
	VECTOR2			m_v2Center;
	
	float			m_fUlength;
	float			m_fVlength;

	TVERTEX			m_tvRect[4];
	DWORD			m_dwWidth;
	DWORD			m_dwHeight;
	
	CVertexBuffer	m_VB;
	CIndexBuffer	m_IB;

	BOOL			m_bIsLockable;

	void			SetVertexList(VECTOR4* pv4Rect,DWORD dwColor);
public:
	STDMETHODIMP			QueryInterface(REFIID, PPVOID);
	STDMETHODIMP_(ULONG)	AddRef(void);
    STDMETHODIMP_(ULONG)	Release(void);

	BOOL		__stdcall	Draw(VECTOR2* pv2Scaling,float fRot,VECTOR2* pv2Trans,RECT* pRect,DWORD dwColor,DWORD dwFlag);
	BOOL		__stdcall	Resize(float fWidth,float fHeight);
	BOOL		__stdcall	GetImageHeader(IMAGE_HEADER* pImgHeader,DWORD dwFrameIndex);

	BOOL		__stdcall	LockRect(LOCKED_RECT* pOutLockedRect, RECT* pRect, TEXTURE_FORMAT TexFormat);
	BOOL		__stdcall	UnlockRect();

//	BOOL					Initialize(CoD3DDevice* pRenderer,char* szFileName,DWORD dwColor,DWORD dwFlag);
	BOOL					Initialize(CoD3DDevice* pRenderer,char* szFileName,DWORD dwXPos,DWORD dwYPos,DWORD dwWidth,DWORD dwHeight,DWORD dwColor,DWORD dwFlag);
	BOOL					InitializeNameless(CoD3DDevice* pRenderer, DWORD dwWidth,DWORD dwHeight, TEXTURE_FORMAT TexFormat, DWORD dwFlag);


	C3DSprite();
	~C3DSprite();

};


#endif
