// MovePoint.cpp: implementation of the CMovePoint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MovePoint.h"
#include "GameResourceManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CMovePoint)
CMovePoint::CMovePoint()
{
	m_bInited = FALSE;
}

CMovePoint::~CMovePoint()
{

}

BOOL CMovePoint::Init()
{
	//trustpak 2005/06/22
	//BOOL rt = m_PointModel.Init(GAMERESRCMNGR->m_GameDesc.MovePoint,NULL,eEngineObjectType_Effect,0,-1);
	BOOL rt = m_PointModel.Init(GAMERESRCMNGR->m_GameDesc.MovePoint,NULL,eEngineObjectType_GameSystemObject,0,-1);
	///
	if(rt == FALSE)
		return FALSE;
	m_PointModel.ApplyHeightField(TRUE);
	m_bInited = TRUE;
	return TRUE;
}

void CMovePoint::Release()
{
	m_PointModel.Release();
}

void CMovePoint::SetPoint(VECTOR3* pPos)
{
	if(m_bInited == FALSE)
		return;

	m_PointModel.SetEngObjPosition(pPos);
	m_PointModel.Show();
}