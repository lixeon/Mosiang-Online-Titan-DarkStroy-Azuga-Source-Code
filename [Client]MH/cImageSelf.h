// cImageSelf.h: interface for the cImageSelf class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CIMAGESELF_H__A2129806_587E_4BE6_8AA3_B8C595E738F6__INCLUDED_)
#define AFX_CIMAGESELF_H__A2129806_587E_4BE6_8AA3_B8C595E738F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "interface/cWindowDef.h"
#include "../Engine/GraphicEngine.h"

class cImageSelf
{  
public:
	cImageSelf();
	virtual ~cImageSelf();

	BOOL LoadSprite(char * szFilename);
	BOOL IsNull() const { return (m_pSurface==NULL); }
	
	void Release();
	
	
	void RenderSprite( VECTOR2* pv2Scaling, VECTOR2* pv2RotCenter, float fRot,
								VECTOR2* pv2Trans, DWORD dwColor );

	void SetImageSrcRect(const cImageRect * rt);

	cImageRect* GetImageRect()	{	return &m_srcImageRect;	}

	BOOL GetImageOriginalSize(VECTOR2* pOutSize);

protected:
	IDISpriteObject	*		m_pSurface;
	
	cImageRect m_srcImageRect;
	cImageSize m_srcImageSize;

};

#endif // !defined(AFX_CIMAGESELF_H__A2129806_587E_4BE6_8AA3_B8C595E738F6__INCLUDED_)
