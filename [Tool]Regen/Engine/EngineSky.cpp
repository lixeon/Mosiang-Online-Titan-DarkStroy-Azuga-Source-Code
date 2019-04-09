// EngineSky.cpp: implementation of the CEngineSky class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EngineSky.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEngineSky::CEngineSky()
{
	m_pSkyMesh = NULL;

}

CEngineSky::~CEngineSky()
{
	if(g_pExecutive)
		SAFE_RELEASE(m_pSkyMesh);

}

BOOL CEngineSky::CreateSky(char* filename,char* anifile)
{
	g_pExecutive->GetGeometry()->CreateSkyMesh(&m_pSkyMesh,filename,0);
	m_pSkyMesh->SetViewportIndex(0);
	VECTOR3	v3SkyOffset;
	v3SkyOffset.x = 0.0f;
	v3SkyOffset.y = 0.0f;
	v3SkyOffset.z = 0.0f;
	
	m_pSkyMesh->SetOffset(&v3SkyOffset);
	m_pSkyMesh->CreateMotionList(1);
	m_pSkyMesh->AddMotion(anifile,0);

	return TRUE;
}

void CEngineSky::ChangeSkyAni(int ani)
{
	m_pSkyMesh->SetCurrentMotionIndex(ani);
}
void CEngineSky::RenderSky()
{
	g_pExecutive->GetGeometry()->DisableFog(0);

	g_pExecutive->GetGeometry()->BeginRender(0,g_bColor,0);
	g_pExecutive->GetGeometry()->RenderSkyMesh(m_pSkyMesh,0);
	g_pExecutive->GetGeometry()->EndRender();

	g_pExecutive->GetGeometry()->EnableFog(0);
}