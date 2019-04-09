#include "camera_obj.h"
#include "model.h"
#include "../4DyuchiGXGFunc/global.h"
#include "global_variable.h"

CCameraObject::CCameraObject()
{
	m_dwTargetIndex = 0xffffffff;
	m_pTarget = NULL;
	m_objType = OBJECT_TYPE_CAMERA;
//	m_cameraFlag = 0;
}


DWORD CCameraObject::ReadFile(void* pFP,DWORD dwVersion)
{
	FILE_CAMERA_HEADER* pHeader = (FILE_CAMERA_HEADER*)&(this->m_dwTargetIndex);

	DWORD	dwLen = 0;

	dwLen += CBaseObject::ReadFile(pFP,dwVersion);

	dwLen += g_pFileStorage->FSRead(pFP,pHeader,sizeof(FILE_CAMERA_HEADER));
	
	return dwLen;

}
void CCameraObject::GetCameraMatrix(MATRIX4* pMatView,DWORD dwRefIndex)
{
	MATRIX4* pFrom = m_pModel->GetObjectResultMatrix(dwRefIndex,GetIndex());
	MATRIX4* pTo = m_pModel->GetObjectResultMatrix(dwRefIndex,m_pTarget->GetIndex());

	VECTOR3	v3Up;
	v3Up.x = 0.0f;
	v3Up.y = 1.0f;
	v3Up.z = 0.0f;

	::SetViewMatrix(pMatView,(VECTOR3*)&pFrom->_41,(VECTOR3*)&pTo->_41,&v3Up);
}

BOOL CCameraObject::Initialize(CoModel* pModel)
{
	m_pModel = pModel;
	m_pTarget = (CCameraObject*)pModel->SelectObject(m_dwTargetIndex);
	m_pTarget->SetModelPtr(pModel);

	return TRUE;
}


CCameraObject::~CCameraObject()
{
	CBaseObject::~CBaseObject();

}
