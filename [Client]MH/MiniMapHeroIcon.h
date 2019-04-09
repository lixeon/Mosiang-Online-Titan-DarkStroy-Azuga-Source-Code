// MiniMapHeroIcon.h: interface for the CMiniMapHeroIcon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MINIMAPHEROICON_H__43A3DD7C_E763_406B_901F_8DBCDE0C3AB2__INCLUDED_)
#define AFX_MINIMAPHEROICON_H__43A3DD7C_E763_406B_901F_8DBCDE0C3AB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MiniMapIcon.h"

class CMiniMapHeroIcon : public CMiniMapIcon  
{
	float m_HeroImageAngle;

//카메라 표시를 위해서
	float m_fCameraImageAngle;
	CMiniMapIconImage* m_pCameraIconImage;

public:
	CMiniMapHeroIcon();
	virtual ~CMiniMapHeroIcon();

	void SetCameraImage(CMiniMapIconImage* pCameraImage) { m_pCameraIconImage = pCameraImage; }

	virtual void Update(DWORD MinimapImageWidth,DWORD MinimapImageHeight);
	virtual void Render(VECTOR2* pMapStartPixel);
};

#endif // !defined(AFX_MINIMAPHEROICON_H__43A3DD7C_E763_406B_901F_8DBCDE0C3AB2__INCLUDED_)
