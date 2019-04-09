#pragma once

#include "../4DyuchiGRX_common/IRenderer.h"
#include "object.h"
class CoModel;


#pragma pack(push,4)
struct FILE_CAMERA_HEADER
{
	DWORD				dwTargetIndex;
};
#pragma pack(pop)


class CCameraObject : public CBaseObject
{
	DWORD					m_dwTargetIndex;
	
	CCameraObject*			m_pTarget;
	CCameraFlag				m_cameraFlag;
	CoModel*				m_pModel;
public:
	
	void					GetCameraMatrix(MATRIX4* pMatView,DWORD dwRefIndex);
	CCameraFlag				GetCameraFlag()			{return m_cameraFlag;}
	CCameraObject*			GetTargetObject()		{return m_pTarget;}
	DWORD					ReadFile(void* pFP,DWORD dwVersion);
	BOOL					Initialize(CoModel* pModel);

	CCameraObject();
	~CCameraObject();
};


