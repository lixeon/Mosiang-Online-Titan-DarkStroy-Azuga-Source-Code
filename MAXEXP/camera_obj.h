#pragma once

#include "object.h"
#include "../4DyuchiGRX_Common/mesh_flag.h"


#pragma pack(push,4)
struct FILE_CAMERA_HEADER
{
	DWORD				dwTargetIndex;
};
#pragma pack(pop)


class CCameraObject : public CBaseObject
{
	DWORD				m_dwTargetIndex;
	CCameraFlag			m_cameraFlag;
	
public:
	void				SetTargetIndex(DWORD dwIndex) {m_dwTargetIndex = dwIndex;}
	DWORD				WriteFile(FILE* fp);
	
	CCameraObject();
	~CCameraObject();
};


