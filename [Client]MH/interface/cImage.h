// cImage.h: interface for the cImage class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _cIMAGE_H_
#define _cIMAGE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cWindowDef.h"
#include "../Engine/GraphicEngine.h"
#include "../4DyuchiGRX_common/IGeometry.h"
//#include "../4DyuchiGRX_common/math.inl"

/*!
 * cImage class is updated at 2003/10/14
 * coded by taiyo
 */

class cImageBase 
{
	friend class cImage;
public:
	cImageBase();
	virtual ~cImageBase();
	//void LoadSprite(char * szFilename);
	BOOL IsNull() const { return (m_pSurface==NULL); }
	void SetSpriteObject(IDISpriteObject * obj)	{	m_pSurface = obj;	}
protected:
	inline void RenderSprite( VECTOR2* pv2Scaling, VECTOR2* pv2RotCenter, float fRot,
								VECTOR2* pv2Trans, RECT* pRect, DWORD dwColor )
	{		
		if(m_pSurface)
		{
			g_pExecutive->GetGeometry()->RenderSprite(m_pSurface,pv2Scaling,fRot,pv2Trans,pRect,dwColor,0,RENDER_TYPE_DISABLE_TEX_FILTERING);
		}
	}
	IDISpriteObject	*		m_pSurface;
};

class cImage : public cImageBase
{
public:	
	cImage();
	virtual ~cImage();
	void RenderSprite( VECTOR2* pv2Scaling, VECTOR2* pv2RotCenter, float fRot,
								VECTOR2* pv2Trans, DWORD dwColor );

	void SetImageSrcSize(const cImageSize * sz)	{	memcpy( &m_srcImageSize, sz, sizeof(cImageSize) );	}
	void SetImageSrcRect(const cImageRect * rt)	{	memcpy( &m_srcImageRect, rt, sizeof(cImageRect) );	}
	
	cImageSize* GetImageSrcSize() { return &m_srcImageSize; }
protected:
	cImageRect m_srcImageRect;
	cImageSize m_srcImageSize;
};

#endif // _cIMAGE_H_


/* 이전 코드 살려 두었음... 윤호씨 참조
class cImage  
{
public:

	cImage();
	virtual ~cImage();
	BOOL LoadSprite(char * szFilename);
	void RenderSprite(VECTOR2* pv2Scaling,VECTOR2* pv2RotCenter,float fRot,VECTOR2* pv2Trans,DWORD dwColor,int iZOrder,DWORD dwFlag);
	void RenderSpriteEx(VECTOR2* pv2Scaling,VECTOR2* pv2RotCenter,float fRot,VECTOR2* pv2Trans,DWORD dwColor,int iZOrder,DWORD dwFlag);
	void RenderSpritePart( VECTOR2* pv2Scaling, VECTOR2* pv2RotCenter, float fRot, VECTOR2* pv2Trans, RECT * imgSrcRect, DWORD dwColor );
	//void SetXY(LONG x, LONG y){ m_absPos.x=(float)x; m_absPos.y=(float)y; }
	void SetWH(LONG wid, LONG hei){ m_width=(WORD)wid; m_height=(WORD)hei; }

	void SetRect(RECT * rect) { CopyRect(&m_ImgRect, rect); }
	IDISpriteObject * m_pSurface;
	
	//VECTOR2 m_absPos;
	WORD m_width;
	WORD m_height;
	
	RECT m_ImgRect;

	I4DyuchiGXRenderer* m_pRenderer;
	I4DyuchiGXGeometry* m_pGeometry;
};
*/