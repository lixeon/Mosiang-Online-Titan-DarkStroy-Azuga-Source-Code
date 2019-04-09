// ItemGround.cpp: implementation of the CItemGround class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemGround.h"

#include ".\Engine\EngineObject.h"
#include "AppearanceManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemGround::CItemGround(ITEMBASE* pInfo,VECTOR3* pPos)
{
	memcpy(&m_ItemBaseInfo,pInfo,sizeof(ITEMBASE));
	m_Pos = *pPos;
	
	m_pEngineObject = new CEngineObject;

//	APPEARANCEMGR->InitItemAppearance(this);
	m_pEngineObject->SetEngObjPosition(pPos);
}

CItemGround::~CItemGround()
{
	SAFE_DELETE(m_pEngineObject);
}

void CItemGround::Process()
{
	
}
