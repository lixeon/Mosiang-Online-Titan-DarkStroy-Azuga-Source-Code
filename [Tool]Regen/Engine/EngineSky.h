// EngineSky.h: interface for the CEngineSky class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENGINESKY_H__4D70F0DA_D55A_46CF_9FC6_B4C1C0600431__INCLUDED_)
#define AFX_ENGINESKY_H__4D70F0DA_D55A_46CF_9FC6_B4C1C0600431__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GraphicEngine.h"

class CEngineSky  
{
	ISkyMesh*	m_pSkyMesh;
public:
	CEngineSky();
	virtual ~CEngineSky();

	BOOL CreateSky(char* filename,char* anifile);
	void ChangeSkyAni(int ani);
	void IncreaseSkyAniFrame(int frame)
	{
		m_pSkyMesh->IncreaseFrame(frame);
	}
	void RenderSky();
};

#endif // !defined(AFX_ENGINESKY_H__4D70F0DA_D55A_46CF_9FC6_B4C1C0600431__INCLUDED_)
